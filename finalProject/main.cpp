#include "dependencies/linker.h"

#define WIDTH 1920.0f
#define HEIGHT 1080.0f

#define NUMTEXTURES 10

int angleX = 0;
int angleY = 0;
float tick = 0;
float playerX = 0.0;
float playerY = 5.0;
float playerZ = 85;
float angle = 0;
int mode = 0;
unsigned int textures[NUMTEXTURES];
float lastMouseX = WIDTH / 2.0f;
float lastMouseY = HEIGHT / 2.0f;
bool firstMouse = false; // Ignore initial large delta on mouse movement
float mouseSensitivity = 0.2f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
    }

    // Calculate offset
    float xoffset = (xpos - lastMouseX) * mouseSensitivity;
    float yoffset = (lastMouseY - ypos) * mouseSensitivity; // Reversed y-coordinates
    lastMouseX = xpos;
    lastMouseY = ypos;

    // Update angles
    angleX += xoffset;
    angleY += yoffset;

    // Constrain pitch angle to prevent flipping
    if (angleY > 89.0f)
        angleY = 89.0f;
    if (angleY < -89.0f)
        angleY = -89.0f;
}
GLuint loadTexture(const char* filepath) {
    // Initialize FreeImage
    FreeImage_Initialise();

    // Load image from file
    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filepath, 0), filepath);
    if (!bitmap) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return 0;
    }

    // Convert image to 32-bit format (RGBA)
    FIBITMAP* img32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    unsigned char* data = FreeImage_GetBits(img32);
    unsigned width = FreeImage_GetWidth(img32);
    unsigned height = FreeImage_GetHeight(img32);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Upload the texture to OpenGL (RGBA format)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(img32);
    FreeImage_DeInitialise();

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
    if (mode == 0){
        playerY += r2_y_1 * stepSize;
    }
    
    playerZ += r2_z_1 * stepSize;
}
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        updatePlayerCords(0.5,angleX, angleY); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        updatePlayerCords(-0.5,angleX-90,180);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        updatePlayerCords(-0.5,angleX,angleY);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        updatePlayerCords(-0.5,angleX + 90,180);
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        mode += 1;
        mode %= 2;
    }
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
    glm::vec3 lightPos2 = glm::vec3(0,1, 93);

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
    lightsInScene[2] = l2;
    drawSphere(shader, lightPos, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
    drawSphere(shader, lightPos2, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
    drawSphere(shader, lightPos1, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
}
void drawChair(GLuint shader, glm::vec3 pos){
    //Pole & cylinder & cube on top of pole
    setMaterialUniforms(shader, polishedMetal);
    drawCylinder(shader, pos + glm::vec3(0,0.5,0),glm::vec3(0.05, 0.4, 0.05),0,0, textures[1]);
    drawCylinder(shader, pos + glm::vec3(0,0.9,0),glm::vec3(0.2, 0.05, 0.2),0,0, textures[1]);
    drawCube(shader, pos + glm::vec3(0,0.95,0),glm::vec3(0.3, 0.03, 0.3),0,0, textures[1]);
    drawCube(shader, pos + glm::vec3(0,1.35,0.30),glm::vec3(0.3, 0.03, 0.3),90,0, textures[1]);

    drawCube(shader, pos + glm::vec3(0,1.0,0.28),glm::vec3(0.03, 0.07, 0.03),15,0, textures[1]);
    // drawCube(shader, pos + glm::vec3(0,1.0,0.25),glm::vec3(0.03, 0.1, 0.03),0,0, textures[1]);
    // Base
    setMaterialUniforms(shader,matteSurface);
    drawSphere(shader, pos, glm::vec3(0.5,0.25,0.5), 0,0,textures[0]);
    //Seat
    setMaterialUniforms(shader,rubber);
    drawCube(shader, pos + glm::vec3(0,0.98,0), glm::vec3(0.25,0.02,0.25), 0,0,textures[0]);
    drawCube(shader, pos + glm::vec3(0,1.35,0.25), glm::vec3(0.25,0.02,0.25), 90,0,textures[0]);
}
void drawConnectingPassage(GLuint shader, glm::vec3 pos){
    glm::vec3 whereToDraw;
    /*
        Truthfully, we probably ought to precompute all of these positions since its a Sin & Cos,
        but we will see if performance needs to be improved. This is fine for now
    */
    for (int i = 0; i < 15; i++){
        whereToDraw = pos + glm::vec3(4.5*Cos(24*i), -4.5*Sin(24*i), 0);
        drawCylinder(shader,whereToDraw, glm::vec3(1,6,1), 90, 0, textures[2]);
    }
}
void drawBridge(GLuint shader, glm::vec3 pos, int ph, int th, unsigned int bridgeTex){
    drawCube(shader,pos,glm::vec3(5,1,5),ph, th, textures[2]);
    drawCube(shader,pos + glm::vec3(10,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(-10,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(0,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(10,0,0),glm::vec3(5,5,1),ph+270, th+180, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(-10,0,0),glm::vec3(5,5,1),ph+270, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(-2.5,0,-7.5),glm::vec3(2.5,2.5,1),ph+270, th, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(2.5,0,-7.5),glm::vec3(2.5,2.5,1),ph+270, th+180, textures[bridgeTex]);

    pos += glm::vec3(0,0,30);
    drawCube(shader,pos,glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(10,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(-10,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(0,0,-10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(10,0,0),glm::vec3(5,5,1),ph+270, th+180, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(-10,0,0),glm::vec3(5,5,1),ph+270, th, textures[bridgeTex]);


    pos += glm::vec3(0,0,30);
    drawCube(shader,pos+ glm::vec3(0,0,20),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(10,0,0),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(-10,0,0),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(0,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(10,0,10),glm::vec3(5,5,1),ph+90, th+180, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(-10,0,10),glm::vec3(5,5,1),ph+90, th, textures[bridgeTex]);

    pos += glm::vec3(0,0,30);
    drawCube(shader,pos+ glm::vec3(0,0,10),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(10,0,0),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(-10,0,0),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);
    drawCube(shader,pos + glm::vec3(0,0,0),glm::vec3(5,1,5),ph, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(10,0,10),glm::vec3(5,5,1),ph+90, th+180, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(-10,0,10),glm::vec3(5,5,1),ph+90, th, textures[bridgeTex]);

    drawHalfCube(shader,pos + glm::vec3(-2.5,0,17.5),glm::vec3(2.5,2.5,1),ph+90, th, textures[bridgeTex]);
    drawHalfCube(shader,pos + glm::vec3(2.5,0,17.5),glm::vec3(2.5,2.5,1),ph+90, th+180, textures[bridgeTex]);
}
void drawWalls(GLuint shader, glm::vec3 pos, int ph, int th, float height, unsigned int wallTex) {
    //Slanted Walls in front
    drawCube(shader,pos + glm::vec3(-7.5,4,-3),glm::vec3(0.5,height,10.8),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(7.5,4,-3),glm::vec3(0.5,height,10.8),ph+ 180, th-45, textures[wallTex]);

    //Other Front walls
    drawCube(shader,pos + glm::vec3(15,4,10),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-15,4,10),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    drawCube(shader,pos + glm::vec3(10,4,15),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-10,4,15),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);

    //First hallway Walls
    drawCube(shader,pos + glm::vec3(5,4,20),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-5,4,20),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //First slanted hallway Walls
    drawCube(shader,pos + glm::vec3(-10,4,30),glm::vec3(0.5,height,7),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(10,4,30),glm::vec3(0.5,height,7),ph+ 180, th-45, textures[wallTex]);

    //First Pre Cylinder Bridge Walls left
    drawCube(shader,pos + glm::vec3(15,4,40),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(5,4,40),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //First Pre Cylinder Bridge Walls Right
    drawCube(shader,pos + glm::vec3(-15,4,40),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-5,4,40),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //Second Pre Cylinder Bridge Walls left
    drawCube(shader,pos + glm::vec3(15,4,60),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(5,4,60),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //Second Pre Cylinder Bridge Walls Right
    drawCube(shader,pos + glm::vec3(-15,4,60),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-5,4,60),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);



    // drawCube(shader,pos + glm::vec3(-5,4,60),glm::vec3(0.5,height,5),ph, th, textures[2]);


    //Second slanted hallway Walls
    drawCube(shader,pos + glm::vec3(10,4,70),glm::vec3(0.5,height,7),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-10,4,70),glm::vec3(0.5,height,7),ph+ 180, th-45, textures[wallTex]);

    //Second hallway Walls
    drawCube(shader,pos + glm::vec3(5,4,80),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-5,4,80),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //Back Main Cooridor walls
    drawCube(shader,pos + glm::vec3(10,4,85),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-10,4,85),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);

    drawCube(shader,pos + glm::vec3(15,4,90),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-15,4,90),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //Back Tip Slanted Walls
    drawCube(shader,pos + glm::vec3(7.5,4,103),glm::vec3(0.5,height,10.8),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-7.5,4,103),glm::vec3(0.5,height,10.8),ph+ 180, th-45, textures[wallTex]);
}
void drawTransparentObjects(GLuint shader, glm::vec3 pos, int ph, int th, float height, unsigned int windowTex, unsigned int glassTex){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setMaterialUniforms(shader, gold);
    drawCube(shader,pos + glm::vec3(0,4,35),glm::vec3(0.5,height,5),ph, th+90, textures[3]);
    drawCube(shader,pos + glm::vec3(0,4,65),glm::vec3(0.5,height,5),ph, th+90, textures[3]);
    setMaterialUniforms(shader, glass);
    drawCylinder(shader, pos + glm::vec3(0,4,93), glm::vec3(2, height/0.99, 2), ph, th, textures[glassTex]);
    glDisable(GL_BLEND);
}
void drawRedMatterHold(GLuint shader, glm::vec3 pos, int ph, int th, float height, unsigned int glassTex, unsigned int shipMatTex){
    // drawCylinder(shader, pos + glm::vec3(0,height/2,93), glm::vec3(2, height, 2), ph, th, textures[glassTex]);
    setMaterialUniforms(shader, polishedMetal);
    drawHalfCube(shader, pos + glm::vec3(2.6,1.5,93), glm::vec3(0.5,0.8,0.2),ph, th+180, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(-2.6,1.5,93), glm::vec3(0.5,0.8,0.2),ph, th, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(0,1.5,95.6), glm::vec3(0.5,0.8,0.2),ph, th+90, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(0,1.5,90.4), glm::vec3(0.5,0.8,0.2),ph, th+270, textures[shipMatTex]);

    drawHalfCube(shader, pos + glm::vec3(2.6,6.5,93), glm::vec3(0.5,0.8,0.2),ph+180, th+180, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(-2.6,6.5,93), glm::vec3(0.5,0.8,0.2),ph+180, th, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(0,6.5,95.6), glm::vec3(0.5,0.8,0.2),ph+180, th+270, textures[shipMatTex]);
    drawHalfCube(shader, pos + glm::vec3(0,6.5,90.4), glm::vec3(0.5,0.8,0.2),ph+180, th+90, textures[shipMatTex]);

    drawCube(shader, pos + glm::vec3(0,4,91),glm::vec3(0.2, 4, 0.2), ph,th, textures[shipMatTex]);
    drawCube(shader, pos + glm::vec3(0,4,95),glm::vec3(0.2, 4, 0.2), ph,th, textures[shipMatTex]);
    drawCube(shader, pos + glm::vec3(2,4,93),glm::vec3(0.2, 4, 0.2), ph,th, textures[shipMatTex]);
    drawCube(shader, pos + glm::vec3(-2,4,93),glm::vec3(0.2, 4, 0.2), ph,th, textures[shipMatTex]);
}
// void rotateLight()
void drawShip(GLuint shader, GLuint redMatterShader, glm::vec3 shipPos){
    glm::vec3 origin = shipPos;
    drawLightsInScene(shader);

    setMaterialUniforms(shader, polishedMetal);
    drawBridge(shader, origin, 0, 0,2);
    drawBridge(shader, origin + glm::vec3(0,8,0), 0, 0, 2);
    drawConnectingPassage(shader, glm::vec3(10,4,50));
    drawConnectingPassage(shader, glm::vec3(-10,4,50));
    setMaterialUniforms(shader, whitePadding);
    drawWalls(shader, origin, 0, 0 ,5, 4);
    drawRedMatterHold(shader, origin, 0,0, 4, 3, 2);
    setMaterialUniforms(shader, polishedMetal);
    drawParticles(shader, textures[6], origin + glm::vec3(0,4,93));
    drawTransparentObjects(shader, origin, 0, 0, 5, 3, 3);


   


    // drawHalfCube(shader,origin,glm::vec3(1,1,1),0, 0, textures[2]);


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
    GLuint shaderProgram1 = createShaderProgram("dependencies/shaders/vertexShader.glsl", "dependencies/shaders/fragShader.glsl");

    if (shaderProgram == 0) {
        fprintf(stderr, "Failed to create shader program\n");
        return -1;
    }
    float aspectRatio = WIDTH/HEIGHT;
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);  // or GL_FRONT depending on your setup
    // glFrontFace(GL_CCW);  // Counter-clockwise is the default winding order in OpenGL


    textures[0] = loadTexture("dependencies/textures/futuristicMosaic.bmp");
    textures[1] = loadTexture("dependencies/textures/funkyTileTexture.bmp");
    textures[2] = loadTexture("dependencies/textures/futuristicTex.bmp");
    textures[3] = loadTexture("dependencies/textures/windowTexture.bmp");
    textures[4] = loadTexture("dependencies/textures/WhitePaddedWall.png");
    textures[5] = loadTexture("dependencies/textures/BlackTriTile.png");
    textures[6] = loadTexture("dependencies/textures/redBlobNoDot.png");




    if (!textures[0] || !textures[1] || !textures[2] || !textures[3]) {
        printf("Texture error:\n");
        return 0;
    }


    // textures[3] = LoadTexBMP("dependencies/textures/windowTexture.bmp");
    // if (textures[0] == 0 || textures[1] == 0 || textures[2] == 0){
    //     printf("Couldnt Load Tex");
    //     return 0;
    // }
    glfwSetCursorPosCallback(window, mouse_callback);

    // Capture the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setupHalfCube();
    setupCube();
    setupCylinder(10);
    setupSphere(10);
    glm::vec3 shipPos = glm::vec3(0,0,0);
    // void setParamsParticles(glm::vec3 topBoundingVec_, glm::vec3 lowBoundingVec_,float boundingRadius_,
    //                     float particleRadius_, float distanceThresholdToCombine_, float breakApartThreshold_,
    //                     float randomVelocityThreshold_,float startingSize_)
    setParamsParticles(glm::vec3(0,3,0), glm::vec3(0,-3,0), 1.90, 0.5, 0.0005, 0.70, 2.0f, 1.90f);
    initParticles();

    initLightingUniforms(shaderProgram);
    while (!glfwWindowShouldClose(window)) {
        glm::vec3 playerPos = glm::vec3(playerX, playerY, playerZ);
        processInput(window);
        projection(shaderProgram, 60.0f, 0.1f, 200.0f, aspectRatio);
        setViewMatrix(shaderProgram, playerPos, angleX, angleY);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawShip(shaderProgram,shaderProgram1, shipPos);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
