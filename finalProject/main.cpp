#include "dependencies/linker.h"

#define WIDTH 600.0f
#define HEIGHT 600.0f
#define SCENEXWIDTH 36
#define SCENEZWIDTH 120
#define NUMTEXTURES 15

int angleX = 180;
int angleY = 0;
float tick = 0;
float playerX = 0.0;
float playerY = 5.0;
float playerZ = 100;
float angle = 0;
int mode = 1;
unsigned int textures[NUMTEXTURES];
float lastMouseX = WIDTH / 2.0f;
float lastMouseY = HEIGHT / 2.0f;
bool firstMouse = false; 
float mouseSensitivity = 0.2f;
bool walkabilityBitmap[SCENEZWIDTH][SCENEXWIDTH];
GLuint* shaderPtr;
float aspectRatio;

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
/*
    This function was written following this stack overflow post: 
    https://stackoverflow.com/questions/17125843/how-do-i-load-textures-to-opengl-using-freeimage-library
*/
GLuint loadTexture(const char* filepath) {
    FreeImage_Initialise();

    FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filepath, 0), filepath);
    if (!bitmap) {
        std::cerr << "Failed to load image: " << filepath << std::endl;
        return 0;
    }

    //If not 32 bit, convert it
    FIBITMAP* img32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    unsigned char* data = FreeImage_GetBits(img32);
    unsigned width = FreeImage_GetWidth(img32);
    unsigned height = FreeImage_GetHeight(img32);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(img32);
    FreeImage_DeInitialise();

    return texture;
}

void generateWalkabilityBitmap(glm::vec3 pos) {
    //We will define a "walkability" vector that can be indexed by X value ~ 40 indices
    //Theres probably a more optimal solution here, but this opts to sacrifice the time to prepoluate an array instead of doing a dynamic check on each movement.
    //This solution is O(1) per player movement since its direct array access (movement across z and x is probably not cache friendly but....)
    for (int z = 0; z < SCENEZWIDTH; z++) {
        for (int x = 0; x < SCENEXWIDTH; x++){
            walkabilityBitmap[z][x] = false;
        }
    }
    
    //I dont know if this is the best way of doing this, but it works!
    for (int z = 9; z < SCENEZWIDTH; z++) {
        for (int x = 0; x < SCENEXWIDTH; x++){
            if (z >= 10 && z < 16){
                if (x > 26 - z && x < 14 + z){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 16 && z < 25){
                if (x > 6 && x < 35){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 25 && z < 37){
                if (x > 16 && x < 25){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            //This part is janky, fix if noticeable
            if (z >= 35 && z < 46){
                if (x > 52 - z && x < -12 + z){
                    walkabilityBitmap[z][x] = true;
                } 
                if (z > 43 && (x < 27 && x > 14)){
                    walkabilityBitmap[z][x] = false;
                }
            }
            if (z >= 46 && z < 54){
                if ((x < 15 && x > 6) || (x < 35 && x > 26)){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 54 && z < 68){
                if ((x < 13 && x > 8) || (x < 33 && x > 28)){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 68 && z < 76){
                if ((x < 15 && x > 6) || (x < 35 && x > 26)){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 76 && z < 85){
                if (x < ((37+72) - z) && x > ((4 - 72) + z)){
                    walkabilityBitmap[z][x] = true;
                } 
                if (z < 78 && (x < 27 && x > 14)){
                    walkabilityBitmap[z][x] = false;
                }
            }
            if (z >= 85 && z < 97){
                if (x > 16 && x < 25){
                    walkabilityBitmap[z][x] = true;
                } 
            }
            if (z >= 97 && z < 105){
                if ((x > 6 && x < 35)){
                    walkabilityBitmap[z][x] = true;
                } 
                if (z > 100 && (x < (21 - 100 + z) && (x > (20 + 100 - z)))){
                    walkabilityBitmap[z][x] = false;
                }
            }
            if (z >= 105 && z < 118){
                if (x < ((34+105) - z) && x > ((6 - 105) + z)){
                    walkabilityBitmap[z][x] = true;
                } 
                if (z < 107 && (x < (24 + 105 - z) && (x > (17 - 105 + z)))){
                    walkabilityBitmap[z][x] = false;
                }
            }
        }
    }
}
void printMyStupidPlayerCollisions(){
    for (int z = 0; z < SCENEZWIDTH; z++) {
        for (int x = 0; x < SCENEXWIDTH; x++){
            printf("%d ",(int)walkabilityBitmap[z][x]);
        }
        printf("\n");
    }
}
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

    if (mode == 1){
        if (walkabilityBitmap[(int)ceil(playerZ+ 10 +r2_z_1 * stepSize)] [(int)ceil(playerX + 20 + r2_x_1 * stepSize)]){
            playerX += r2_x_1 * stepSize;
            playerZ += r2_z_1 * stepSize;
        }
    }
    else {
        playerX += r2_x_1 * stepSize;
        playerY += r2_y_1 * stepSize;
        playerZ += r2_z_1 * stepSize;
    }
}
void processInput(GLFWwindow *window) {
   if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        angleY += 2;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        angleY -= 2;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        angleX -= 2;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        angleX += 2;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        updatePlayerCords(0.3,angleX, angleY); 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        updatePlayerCords(-0.3,angleX-90,180);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        updatePlayerCords(-0.3,angleX,angleY);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        updatePlayerCords(-0.3,angleX + 90,180);
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
        mode += 1;
        mode %= 2;
    }
}
void drawLightsInScene(GLuint shader){
    float radius = 50;
    glm::vec3 playerPos = {playerX,playerY,playerZ};
    // playerPos = {0,0,0};

    setFragLightingUniforms(shader,playerPos);
    float specularConst = 100.0f;
    setMaterialUniforms(shader, LightMaterial);
    glm::vec3 lightPos = glm::vec3(radius * Cos(tick), 10,50 +  radius * Sin(tick));
    glm::vec3 lightPos2 = glm::vec3(0,7, 93);

    Light l1 = {
        .ambientComponent = glm::vec3(1.0,1.0,1.0),
        .diffuseComponent = glm::vec3(1.0,1.0,1.0),
        .specularComponent = glm::vec3(1.0,1.0,1.0),
        .specularConstant = specularConst,
        .lightColor = glm::vec3(1.0,1.0,1.0),
        .lightPos = lightPos
    };
    dirLight l4 = {
        .position = lightPos2,
        .direction = glm::vec3(0.0,-1.0,0.0),
        .ambientComponent = glm::vec3(1.0,1.0,1.0),
        .diffuseComponent = glm::vec3(1.0,1.0,1.0),
        .specularComponent = glm::vec3(1.0,1.0,1.0),
        .innerCutoff = glm::cos(glm::radians(45.0f)), // Inner cone angle
        .lightColor = glm::vec3(1.0,0.0,0.0),
        .outerCutoff = glm::cos(glm::radians(90.0f))
    };
    dirLight tvScreen = {
        .position = glm::vec3(0,5,-5.9),
        .direction = glm::vec3(0.0,0.0,1.0),
        .ambientComponent = glm::vec3(1.0,1.0,1.0),
        .diffuseComponent = glm::vec3(1.0,1.0,1.0),
        .specularComponent = glm::vec3(1.0,1.0,1.0),
        .innerCutoff = glm::cos(glm::radians(100.0f)), // Inner cone angle
        .lightColor = glm::vec3(1.0,1.0,1.0),
        .outerCutoff = glm::cos(glm::radians(180.0f))
    };
    lightsInScene[0] = l1;
    dirLightsInScene[0] = l4;
    dirLightsInScene[1] = tvScreen;


    drawSphere(shader, lightPos, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
    // drawSphere(shader, lightPos1, glm::vec3(1.0f, 1.0f, 1.0f), 0, 0, textures[1]);
    setMaterialUniforms(shader, fire);
    drawSphere(shader, lightPos2, glm::vec3(2.0f, 0.1f, 2.0f), 0, 0, textures[1]);
}
void drawChair(GLuint shader, glm::vec3 pos, float scaleFactor, int ph, int th){
    // Pole & cylinder & cube on top of pole
    setMaterialUniforms(shader, polishedMetal);
    drawCylinder(shader, pos + glm::vec3(0, 0.5 * scaleFactor, 0), glm::vec3(0.05 * scaleFactor, 0.4 * scaleFactor, 0.05 * scaleFactor),ph, th, textures[7]);
    
    drawCylinder(shader, pos + glm::vec3(0, 0.9 * scaleFactor, 0), glm::vec3(0.2 * scaleFactor, 0.05 * scaleFactor, 0.2 * scaleFactor), ph, th, textures[7]);
    
    drawCube(shader, pos + glm::vec3(0, 0.95 * scaleFactor, 0), glm::vec3(0.3 * scaleFactor, 0.03 * scaleFactor, 0.3 * scaleFactor), ph, th, textures[7]);
    
    drawCube(shader, pos + glm::vec3(0, 1.35 * scaleFactor, 0.30 * scaleFactor), glm::vec3(0.3 * scaleFactor, 0.3 * scaleFactor, 0.03 * scaleFactor), ph, th, textures[7]);

    drawCube(shader, pos + glm::vec3(0, 1.0 * scaleFactor, 0.28 * scaleFactor), glm::vec3(0.03 * scaleFactor, 0.07 * scaleFactor, 0.03 * scaleFactor),ph + 15, th, textures[7]);

    setMaterialUniforms(shader, rubber);
    drawSphere(shader, pos, glm::vec3(0.5 * scaleFactor, 0.25 * scaleFactor, 0.5 * scaleFactor), ph, th, textures[7]);
    
    drawCube(shader, pos + glm::vec3(0, 0.98 * scaleFactor, 0), glm::vec3(0.25 * scaleFactor, 0.02 * scaleFactor, 0.25 * scaleFactor), ph, th, textures[7]);
    // drawCube(shader, 
    //      pos + glm::vec3(0.25 * scaleFactor / sqrt(2), 1.35 * scaleFactor, 0.25 * scaleFactor / sqrt(2)), 
    //      glm::vec3(0.25 * scaleFactor, 0.25 * scaleFactor, 0.02 * scaleFactor), 
    //      ph + 45, th + 90, 
    //      textures[7]);

    drawCube(shader, pos + glm::vec3(0, 1.35 * scaleFactor, 0.25 * scaleFactor), glm::vec3(0.25 * scaleFactor, 0.25 * scaleFactor, 0.02 * scaleFactor), ph, th, textures[7]);
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
    drawCube(shader,pos + glm::vec3(15,4,15),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);
    drawCube(shader,pos + glm::vec3(15,4,5),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);


    drawCube(shader,pos + glm::vec3(15,0.6,10),glm::vec3(0.5,1.5,5),ph, th, textures[2]);
    drawCube(shader,pos + glm::vec3(15,7.4,10),glm::vec3(0.5,1.5,5),ph, th, textures[2]);

    drawCube(shader,pos + glm::vec3(-15,4,15),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-15,4,5),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);

    drawCube(shader,pos + glm::vec3(-15,0.6,10),glm::vec3(0.5,1.5,5),ph, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-15,7.4,10),glm::vec3(0.5,1.5,5),ph, th, textures[2]);

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

    //Middle Window Sills
    drawCube(shader,pos + glm::vec3(0,0.6,34),glm::vec3(0.5,1.5,5),ph, th+90, textures[2]);
    drawCube(shader,pos + glm::vec3(0,7.4,34),glm::vec3(0.5,1.5,5),ph, th+90, textures[2]);

    drawCube(shader,pos + glm::vec3(5.1,4,34.5),glm::vec3(0.5,0.99,5),ph + 90, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-5.1,4,34.5),glm::vec3(0.5,0.99,5),ph + 90, th, textures[2]);

    drawCube(shader,pos + glm::vec3(0,0.6,66),glm::vec3(0.5,1.5,5),ph, th+90, textures[2]);
    drawCube(shader,pos + glm::vec3(0,7.4,66),glm::vec3(0.5,1.5,5),ph, th+90, textures[2]);

    drawCube(shader,pos + glm::vec3(5.1,4,65.5),glm::vec3(0.5,0.99,5),ph + 90, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-5.1,4,65.5),glm::vec3(0.5,0.99,5),ph + 90, th, textures[2]);

    //Second slanted hallway Walls
    drawCube(shader,pos + glm::vec3(10,4,70),glm::vec3(0.5,height,7),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-10,4,70),glm::vec3(0.5,height,7),ph+ 180, th-45, textures[wallTex]);

    //Second hallway Walls
    drawCube(shader,pos + glm::vec3(5,4,80),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-5,4,80),glm::vec3(0.5,height,5),ph, th, textures[wallTex]);

    //Back Main Cooridor walls
    drawCube(shader,pos + glm::vec3(10,4,85),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-10,4,85),glm::vec3(0.5,height,5),ph, th+90, textures[wallTex]);

    drawCube(shader,pos + glm::vec3(15,4,95),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);
    drawCube(shader,pos + glm::vec3(15,4,85),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);

    drawCube(shader,pos + glm::vec3(15,0.6,90),glm::vec3(0.5,1.5,5),ph, th, textures[2]);
    drawCube(shader,pos + glm::vec3(15,7.4,90),glm::vec3(0.5,1.5,5),ph, th, textures[2]);

    drawCube(shader,pos + glm::vec3(-15,4,95),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-15,4,85),glm::vec3(0.6,height,0.4),ph+ 180, th, textures[2]);

    drawCube(shader,pos + glm::vec3(-15,0.6,90),glm::vec3(0.5,1.5,5),ph, th, textures[2]);
    drawCube(shader,pos + glm::vec3(-15,7.4,90),glm::vec3(0.5,1.5,5),ph, th, textures[2]);

    //Back Tip Slanted Walls
    drawCube(shader,pos + glm::vec3(7.5,4,103),glm::vec3(0.5,height,10.8),ph+ 180, th+45, textures[wallTex]);
    drawCube(shader,pos + glm::vec3(-7.5,4,103),glm::vec3(0.5,height,10.8),ph+ 180, th-45, textures[wallTex]);
}
void drawTransparentObjects(GLuint shader, glm::vec3 pos, int ph, int th, float height, unsigned int windowTex, unsigned int glassTex){
    setMaterialUniforms(shader, polishedMetal);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setMaterialUniforms(shader, gold);
    drawCube(shader,pos + glm::vec3(0,4,34),glm::vec3(0.25,height * 0.99,5),ph, th+90, textures[windowTex]);
    drawCube(shader,pos + glm::vec3(0,4,66),glm::vec3(0.25,height * 0.99,5),ph, th+90, textures[windowTex]);

    drawCube(shader,pos + glm::vec3(15,4,10),glm::vec3(0.25,height * 0.99,5),ph, th, textures[windowTex]);
    drawCube(shader,pos + glm::vec3(-15,4,10),glm::vec3(0.25,height * 0.99,5),ph, th, textures[windowTex]);

    drawCube(shader,pos + glm::vec3(15,4,90),glm::vec3(0.25,height * 0.99,5),ph, th, textures[windowTex]);
    drawCube(shader,pos + glm::vec3(-15,4,90),glm::vec3(0.25,height * 0.99,5),ph, th, textures[windowTex]);
    setMaterialUniforms(shader, glass);
    drawCylinder(shader, pos + glm::vec3(0,4,93), glm::vec3(2, height/0.99, 2), ph, th, textures[glassTex]);
    glDisable(GL_BLEND);
}
void drawRedMatterHold(GLuint shader, glm::vec3 pos, int ph, int th, float height, unsigned int glassTex, unsigned int shipMatTex){
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
void drawShipInterior(GLuint shader, glm::vec3 pos, int ph, int th){
    setMaterialUniforms(shader, darkMetal);
    drawCube(shader,pos + glm::vec3(0,5,-6),glm::vec3(0.2,2,4),ph+ 180, th + 90, textures[7]);
    drawCube(shader,pos + glm::vec3(-7.5,5,-2),glm::vec3(0.1,2,7),ph+ 180, th + 45, textures[7]);
    drawCube(shader,pos + glm::vec3(7.5,5,-2),glm::vec3(0.1,2,7),ph+ 180, th - 45, textures[7]);


    setMaterialUniforms(shader, polishedMetal);

    drawCube(shader,pos + glm::vec3(0,5,-5.8),glm::vec3(0.1,1.8,3.5),ph+ 180, th + 90, textures[8]);
    setMaterialUniforms(shader, rubber);
    drawCube(shader,pos + glm::vec3(6.0,5,-3.4),glm::vec3(0.1,1.8,2.5),ph+ 180, th - 45, textures[8]);
    drawCube(shader,pos + glm::vec3(10,5,0.6),glm::vec3(0.1,1.8,2.5),ph+ 180, th - 45, textures[8]);


    drawCube(shader,pos + glm::vec3(-6.0,5,-3.4),glm::vec3(0.1,1.8,2.5),ph+ 180, th + 45, textures[8]);
    drawCube(shader,pos + glm::vec3(-10,5,0.6),glm::vec3(0.1,1.8,2.5),ph+ 180, th + 45, textures[8]);


    setMaterialUniforms(shader, brushedMetal);
    drawCube(shader,pos + glm::vec3(7.5,3,-1.5),glm::vec3(3,0.1,8),ph+ 180, th - 45, textures[5]);
    drawCube(shader,pos + glm::vec3(-7.5,3,-1.5),glm::vec3(3,0.1,8),ph+ 180, th + 45, textures[5]);
    setMaterialUniforms(shader, darkMetal);

    drawCylinder(shader,pos + glm::vec3(0,3.0001,-6),glm::vec3(4,0.1,4),ph+ 180, th, textures[7]);




    drawChair(shader,pos + glm::vec3(0,1,2), 2, 0, 0);

}
void drawShip(GLuint shader, glm::vec3 shipPos){
    glm::vec3 origin = shipPos;
    drawLightsInScene(shader);


    setMaterialUniforms(shader, glossyPlastic);
    drawSphere(shader, glm::vec3(0,0,50), glm::vec3(100,100,100), 0,0, textures[9]);
    setMaterialUniforms(shader, polishedMetal);

    drawBridge(shader, origin, 0, 0,2);
    drawBridge(shader, origin + glm::vec3(0,8,0), 0, 0, 2);
    drawConnectingPassage(shader, glm::vec3(10,4,50));
    drawConnectingPassage(shader, glm::vec3(-10,4,50));
    setMaterialUniforms(shader, whitePadding);
    drawWalls(shader, origin, 0, 0 ,5, 4);
    drawRedMatterHold(shader, origin, 0,0, 4, 3, 2);
    setMaterialUniforms(shader, fire);
    drawParticles(shader, textures[6], origin + glm::vec3(0,4,93));
    
    drawShipInterior(shader, origin, 0, 0);
    drawTransparentObjects(shader, origin, 0, 0, 5, 3, 10);


   


    // drawHalfCube(shader,origin,glm::vec3(1,1,1),0, 0, textures[2]);


    // drawCube(shader, 10,10,10,10,0.1,0.1,angleY,angleX);
    tick += 0.1;
}
void render(GLuint shaderProgram){
    glm::vec3 playerPos = glm::vec3(playerX, playerY, playerZ);
    projection(shaderProgram, 60.0f, 0.1f, 200.0f, aspectRatio);
    setViewMatrix(shaderProgram, playerPos, angleX, angleY);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawShip(shaderProgram, glm::vec3(0,0,0));
}
void window_size_callback(GLFWwindow* window, int width, int height)
{
    aspectRatio = (height>0) ? (double)width/height : 1;
    //Window callbacks wont let me pass the shader as a param, so we have to work around.
    GLuint shaderProgram = *shaderPtr;
    glViewport(0,0, width, height);
    projection(shaderProgram, 60.0f, 0.1f, 200.0f, aspectRatio);

    glfwSetWindowSize(window, width, height);
    render(shaderProgram);
    glfwSwapBuffers(window);
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Window", NULL, NULL);
    
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, window_size_callback);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }


    

    GLuint shaderProgram = createShaderProgram("dependencies/shaders/vertexShader.glsl", "dependencies/shaders/fragShader.glsl");
    shaderPtr = &shaderProgram;

    if (shaderProgram == 0) {
        fprintf(stderr, "Failed to create shader program\n");
        return -1;
    }
    aspectRatio = WIDTH/HEIGHT;
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);


    textures[0] = loadTexture("dependencies/textures/futuristicMosaic.bmp");
    textures[1] = loadTexture("dependencies/textures/funkyTileTexture.bmp");
    textures[2] = loadTexture("dependencies/textures/futuristicTex.bmp");
    textures[3] = loadTexture("dependencies/textures/lowOpWindowTex.bmp");
    textures[4] = loadTexture("dependencies/textures/WhitePaddedWall.png");
    textures[5] = loadTexture("dependencies/textures/BlackTriTile.png");
    textures[6] = loadTexture("dependencies/textures/redThingBlob.png");
    textures[7] = loadTexture("dependencies/textures/R.jpg");
    textures[8] = loadTexture("dependencies/textures/tvStatic.png");
    textures[9] = loadTexture("dependencies/textures/4kStars.jpg");
    textures[10] = loadTexture("dependencies/textures/windowTexture.bmp");

    if (!textures[0] || !textures[1] || !textures[2] || !textures[3] ||
        !textures[4] || !textures[5] || !textures[6] || !textures[7] ||
        !textures[8] || !textures[9] || !textures[10]) {
        printf("Texture error:\n");
        return 0;
    }

    // glfwSetCursorPosCallback(window, mouse_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setupHalfCube();
    setupCube();
    setupCylinder(20);
    setupSphere(40);

    setParamsParticles(glm::vec3(0,3,0), glm::vec3(0,-3,0), 1.80, 0.2, 0.0, 0.90, 2.0f, 1.80f, 0.994);
    initParticles();
    generateWalkabilityBitmap(glm::vec3(50,0,50));

    initLightingUniforms(shaderProgram);
    initDirLightingUniforms(shaderProgram);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        render(shaderProgram);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
