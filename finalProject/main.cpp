#include "dependencies/linker.h"

#define WIDTH 1920.0f
#define HEIGHT 1080.0f

#define NUMTEXTURES 3

int angleX = 0;
int angleY = 0;
float tick = 0;
float playerX = 0.0;
float playerY = 0.0;
float playerZ = 0.0;
float angle = 0;
unsigned int textures[NUMTEXTURES];
/*
    This function is written by chatGPT, used it to convert the BMP loader from CSCIx229 to
    a version that does not rely on GLEW.
*/
unsigned int LoadTexBMP(const char* file) {
    // Open the BMP file
    FILE* f = fopen(file, "rb");
    if (!f) {
        fprintf(stderr, "Error: Cannot open file %s\n", file);
        return 0;
    }

    // Verify BMP file magic number
    unsigned short magic;
    if (fread(&magic, 2, 1, f) != 1) {
        fprintf(stderr, "Error: Cannot read BMP magic number from %s\n", file);
        fclose(f);
        return 0;
    }
    if (magic != 0x4D42) {
        fprintf(stderr, "Error: File is not a valid BMP: %s\n", file);
        fclose(f);
        return 0;
    }

    // Read BMP header information
    unsigned int dx, dy, offset, compression;
    unsigned short planes, bpp;
    fseek(f, 8, SEEK_CUR);  // Skip reserved fields
    fread(&offset, 4, 1, f);   // Offset to pixel data
    fseek(f, 4, SEEK_CUR);     // Skip header size
    fread(&dx, 4, 1, f);       // Width
    fread(&dy, 4, 1, f);       // Height
    fread(&planes, 2, 1, f);   // Number of planes
    fread(&bpp, 2, 1, f);      // Bits per pixel
    fread(&compression, 4, 1, f);  // Compression type

    // Validate BMP properties
    if (planes != 1 || bpp != 24 || compression != 0) {
        fprintf(stderr, "Error: Unsupported BMP format in %s\n", file);
        fclose(f);
        return 0;
    }

    // Allocate memory for pixel data
    unsigned int size = 3 * dx * dy;
    unsigned char* image = (unsigned char*)malloc(size);
    if (!image) {
        fprintf(stderr, "Error: Unable to allocate memory for BMP image\n");
        fclose(f);
        return 0;
    }

    // Read pixel data from file
    fseek(f, offset, SEEK_SET);
    if (fread(image, size, 1, f) != 1) {
        fprintf(stderr, "Error: Unable to read pixel data from %s\n", file);
        free(image);
        fclose(f);
        return 0;
    }
    fclose(f);

    // Convert BGR to RGB
    for (unsigned int i = 0; i < size; i += 3) {
        unsigned char temp = image[i];
        image[i] = image[i + 2];
        image[i + 2] = temp;
    }

    // Generate OpenGL texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dx, dy, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Free pixel data and return texture ID
    free(image);
    return texture;
}
// Handle key input for rotation
void updatePlayerCords(double stepSize, int tempTH, int tempPH){
    //if we take a Z unit vector and rotate it around the x axis counterclockwise we can calculate how our elevation angle will affect the direction of our movement: | 1   0         0   | |0| = |    0   |
    //                                                                                                                                                                | 0 cos(ph)  sin(ph)| |0|   |Sin(ph)|
    //                                                                                                                                                                | 0 -sin(ph) cos(ph)| |1|   |Cos(ph)|

    //Now we can take the resulting vector and counterclockwise rotate it around the Y axis, so as to calculate how our azimuth will change the direction of movement | cos(th)    0 -sin(th) | |   0   |= |-Sin(th) * Cos(ph)|
    //                                                                                                                                                                |    0       1     0    | |Sin(ph)|  |       Sin(ph)    |
    //                                                                                                                                                                |  sin(th)   0  cos(th) | |Cos(ph)|  | Cos(th) * Cos(ph)|
    float r2_x_1 = -Sin(tempTH) * Cos(tempPH);
    float r2_y_1 = Sin(tempPH);
    float r2_z_1 = Cos(tempTH) * Cos(tempPH);
    // Update player coordinates based on the direction vector and step size
    playerX += r2_x_1 * stepSize;
    playerY += r2_y_1 * stepSize;
    playerZ += r2_z_1 * stepSize;
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        angleY += 1;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        angleY -= 1;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        angleX -= 1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        angleX += 1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        updatePlayerCords(0.02,angleX, angleY); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        updatePlayerCords(-0.02,angleX + 90,180);  
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        updatePlayerCords(-0.02,angleX,angleY);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        updatePlayerCords(-0.02,angleX-90,180);
    angleX %= 360;
    angleY %= 360;
    // printf("ANGLEX:%d\n", angleX);
}
void drawLightsInScene(GLuint shader){
    float radius = 50;
    glm::vec3 playerPos = {playerX,playerY,playerZ};
    setFragLightingUniforms(shader,playerPos);
    float specularConst = 100.0f;
    Material LightMaterial ={
        glm::vec3(1.0f, 1.0f, 1.0f),  // ambientReflect: subdued but noticeable ambient lighting
        glm::vec3(1.0f, 1.0f, 1.0f),    // diffuseReflect: reflects light well but not as much as specular
        glm::vec3(1.0f, 1.0f, 1.0f), // specularReflect: strong reflective highlights
        glm::vec3(1.0f, 1.0f, 1.0f)     // objectColor: near-white metallic color
    };
    setMaterialUniforms(shader, LightMaterial);
    glm::vec3 lightPos = glm::vec3(radius * Cos(tick), 10, radius * Sin(tick));
    glm::vec3 lightPos1 = glm::vec3(radius * Sin(tick), 10, radius * Cos(tick));
    Light l1 = {
        .ambientComponent = glm::vec3(1.0,1.0,1.0),
        .diffuseComponent = glm::vec3(1.0,1.0,1.0),
        .specularComponent = glm::vec3(1.0,1.0,1.0),
        .specularConstant = specularConst,
        .lightColor = glm::vec3(1,1,1),
        .lightPos = lightPos
    };
    Light l2 = {
        .ambientComponent = glm::vec3(1.0,1.0,1.0),
        .diffuseComponent = glm::vec3(1.0,1.0,1.0),
        .specularComponent = glm::vec3(1.0,1.0,1.0),
        .specularConstant = specularConst,
        .lightColor = glm::vec3(1,1,1),
        .lightPos = lightPos1
    };
    lightsInScene[0] = l1;
    lightsInScene[1] = l2;
    drawSphere(shader, lightPos, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
    drawSphere(shader, lightPos1, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
}
void drawChair(GLuint shader, glm::vec3 pos){
    setMaterialUniforms(shader, polishedMetal);
    drawCylinder(shader, pos + glm::vec3(0,0.5,0),glm::vec3(0.05, 0.4, 0.05),0,0, textures[1]);
    drawCylinder(shader, pos + glm::vec3(0,0.9,0),glm::vec3(0.2, 0.05, 0.2),0,0, textures[1]);
    drawCube(shader, pos + glm::vec3(0,0.95,0),glm::vec3(0.3, 0.03, 0.3),0,0, textures[1]);
    setMaterialUniforms(shader,matteSurface);
    drawSphere(shader, pos, glm::vec3(0.5,0.25,0.5), 0,0,textures[0]);
    setMaterialUniforms(shader,rubber);
    drawCube(shader, pos + glm::vec3(0,1,0), glm::vec3(0.25,0.02,0.25), 0,0,textures[0]);
}
// void rotateLight()
void drawShip(GLuint shader){
    // Define the floor grid dimensions
    int gridWidth = 10;  // Number of cubes along the X-axis
    int gridDepth = 10;  // Number of cubes along the Z-axis
    float spacing = 2.0f;  // Space between cubes

    // printf("lightsInScene : %d\n", lightsInScene.size());
    // printf("LightPos:%f,%f,%f\n",lightPos.x,lightPos.y,lightPos.z);
    drawLightsInScene(shader);

    setMaterialUniforms(shader, polishedMetal);
    // drawCylinder(shader, cylPos, glm::vec3(5,1,5), 0, 0, textures[2]);
    // for (int j = 0; j < 5; j++){
    // int angleRot = 0;
    // for (int i = 0; i < 10; i++){
    //     drawCylinder(shader, glm::vec3(3*Cos(36*i), -3*Sin(36*i), 0), glm::vec3(1,4,1), 90, 0, textures[2]);
    // }
    // drawCube(shader, glm::vec3(7*Cos(18*i), 0, -7*Sin(18*i)+20), glm::vec3(1,4,1), 0,angleRot, textures[2]);
        // setMaterialUniforms(shader, fire);
    drawChair(shader, glm::vec3(0,0,0));


    // drawCube(shader, 10,10,10,10,0.1,0.1,angleY,angleX);
    tick += 0.1;
}
int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW version to 3.3 for OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Window", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }


    

    // Compile and link shaders into a program
    GLuint shaderProgram = createShaderProgram("dependencies/shaders/vertexShader.glsl", "dependencies/shaders/fragShader.glsl");
    if (shaderProgram == 0) {
        fprintf(stderr, "Failed to create shader program\n");
        return -1;
    }

    // Vertex data for a triangle
    // float vertices[] = {
    //      0.0f,  0.5f, 0.0f,  // top vertex
    //     -0.5f, -0.5f, 0.0f,  // bottom left vertex
    //      0.5f, -0.5f, 0.0f   // bottom right vertex
    // };

    float aspectRatio = WIDTH/HEIGHT;
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);  // or GL_FRONT depending on your setup
    // glFrontFace(GL_CCW);  // Counter-clockwise is the default winding order in OpenGL

    // Render loop
    textures[0] = LoadTexBMP("dependencies/textures/futuristicMosaic.bmp");
    textures[1] = LoadTexBMP("dependencies/textures/funkyTileTexture.bmp");
    textures[2] = LoadTexBMP("dependencies/textures/futuristicTex.bmp");
    if (textures[0] == 0 || textures[1] == 0 || textures[2] == 0){
        printf("Couldnt Load Tex");
        return 0;
    }
    setupCube();
    setupCylinder(10);
    setupSphere(50);
    initLightingUniforms(shaderProgram);
    while (!glfwWindowShouldClose(window)) {
        // angle += 0.02f; // Adjust the speed of the orbit
        glm::vec3 playerPos = glm::vec3(playerX, playerY, playerZ);
        processInput(window);
        projection(shaderProgram, 60.0f, 0.1f, 200.0f, aspectRatio);
        // orthoProjection(shaderProgram, )
        setViewMatrix(shaderProgram, playerPos, angleX, angleY);
        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawShip(shaderProgram);
        // GLuint shader,float x, float y, float z, float scaleX, float scaleY, float scaleZ, float ph, float th
        // drawCube(shaderProgram,0,0,0,1,1,1,angleY,angleX);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
