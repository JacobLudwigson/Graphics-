#include "cube.h"
float cube[] = {
    // Front face (z = 1)
    -1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f,  0.0f,  1.0f, 1.0f, 0.0f,

    // Back face (z = -1)
    -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,0.0f,  0.0f,  -1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,

    // Left face (x = -1)
    -1.0f, -1.0f, -1.0f,-1.0f,  0.0f,  0.0f,0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,-1.0f,  0.0f,  0.0f,0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,-1.0f,  0.0f,  0.0f,1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,-1.0f,  0.0f,  0.0f,0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,-1.0f,  0.0f,  0.0f,1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,-1.0f,  0.0f,  0.0f,1.0f, 0.0f,

    // Right face (x = 1)
     1.0f, -1.0f, -1.0f,1.0f,  0.0f,  0.0f,0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,1.0f,  0.0f,  0.0f,0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,1.0f,  0.0f,  0.0f,1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,1.0f,  0.0f,  0.0f,0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,1.0f,  0.0f,  0.0f,1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,1.0f,  0.0f,  0.0f,1.0f, 0.0f,

    // Top face (y = 1)
    -1.0f,  1.0f, -1.0f,0.0f,  1.0f,  0.0f,0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,0.0f,  1.0f,  0.0f,0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,0.0f,  1.0f,  0.0f,1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,0.0f,  1.0f,  0.0f,0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,0.0f,  1.0f,  0.0f,1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,0.0f,  1.0f,  0.0f,1.0f, 0.0f,

    // Bottom face (y = -1)
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f,0.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f,0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f,  0.0f,0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, -1.0f,  0.0f,1.0f, 0.0f
};
GLuint VBO, VAO;
void setupCube() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
/*
    drawCube(GLuint shader,float x, float y, float z, float scaleX, float scaleY, float scaleZ, float ph, float th)
        MUST BE CALLED **AFTER** setupCube() otherwise vertex buffer and attribute data are not bound
        Draws a cube to the screen using the VAO and VBO defined in the setupCube() function
        Cube will be translated to (x,y,z) and scaled by (scaleX,scaleY,scaleZ) in the shader provided (see shaderOps for more info)
        NOTE this will share a vertex buffer attribute object with other cube calls...I ~think this is good
*/
void drawCube(GLuint shader,glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture) {
    glUseProgram(shader);
    GLint textureLoc = glGetUniformLocation(shader, "image");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLoc, 0);

    //This will update our "model" uniform in the given shader, so Im not sure how this will work with many objects, stay tuned! (it will either draw cube as expected or modify the matrix for all vertex objects!)
    model(shader, position, ph,th, scalar);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, NUMVERTICESCUBE/3);
    glBindVertexArray(0);
}