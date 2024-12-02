#include "sphere.h"
int numIndicies;
int numVerts;
float* sphere;
// void generateSphere(int segments){
//     const float PI = 3.14159265359f;
//     numVerts = segments * 6; // 2 triangles per segment
//     numIndicies = numVerts * 8; // x,y,z,nx,ny,nz,u,v for each vertex
//     float angleEachSeg = 2 * PI /((float) segments);
//     sphere = (float*)malloc(numIndicies * sizeof(float));

//     float cosEachAngleSeg = cos(angleEachSeg);
//     float sinEachAngleSeg = sin(angleEachSeg);
//     float p1_x = 1;
//     float p1_y = 0;
//     float p1_z = 0;

//     float p2_x = cosEachAngleSeg;
//     float p2_y = 0;
//     float p2_z = -sinEachAngleSeg;

//     float u1 = (atan2(p1_z, p1_x) + PI) / (2 * PI);
//     float v1 = 0.5f;

//     float u2 = (atan2(p2_z, p2_x) + PI) / (2 * PI);
//     float v2 = 0.5f;

//     for (int i = 0; i < segments; i++){
//         p1_x = cosEachAngleSeg + (sinEachAngleSeg * p1_x);
//         u1 = (atan2(p1_z, p1_x) + PI) / (2 * PI);
//         //p1_y = 0
//         p1_z = (cosEachAngleSeg * p1_z) - sinEachAngleSeg;

//         p2_x = cosEachAngleSeg + (sinEachAngleSeg * p2_x);
//         //p2_y = 0
//         p2_z = (cosEachAngleSeg * p2_z) - sinEachAngleSeg;
//         u2 = (atan2(p2_z, p2_x) + PI) / (2 * PI);

//         float top [] = {
//             0,1,0, 0,1,0, 0.5f, 1.0f,
//             p2_x, p2_y, p2_z,p2_x, p2_y, p2_z, u2, v2,
//             p1_x, p1_y, p1_z,p1_x, p1_y, p1_z, u1, v1
//         };
//         float bottom [] = {
//             0,-1,0, 0,-1,0, 0.5f, 0.0f,
//             p1_x, p1_y, p1_z, p1_x, p1_y, p1_z, u1, v1,
//             p2_x, p2_y, p2_z, p2_x, p2_y, p2_z, u2, v2
//         };
//         memcpy(&sphere[48 * i], top, 24 * sizeof(float));
//         memcpy(&sphere[(48 * i) + 24], bottom, 24 * sizeof(float));
//     }
// }
/*
    This function is not my own work. Written by GPT using my non working code above as a starting point.
*/
void generateSphere(int segments) {
    const float PI = 3.14159265359f;

    // Total number of vertices
    numVerts = segments * segments * 6; // 2 triangles per quad
    numIndicies = numVerts * 8; // x, y, z, nx, ny, nz, u, v for each vertex
    sphere = (float*)malloc(numIndicies * sizeof(float));

    // Angles for segments (longitude) and stacks (latitude)
    float angleEachSeg = 2 * PI / ((float)segments);
    float angleEachStack = PI / ((float)segments);

    int index = 0; // Keeps track of where to write vertex data in the array

    for (int stack = 0; stack < segments; ++stack) {
        float phi1 = stack * angleEachStack;        // Current latitude
        float phi2 = (stack + 1) * angleEachStack;  // Next latitude

        for (int seg = 0; seg < segments; ++seg) {
            float theta1 = seg * angleEachSeg;        // Current longitude
            float theta2 = (seg + 1) * angleEachSeg;  // Next longitude

            // Vertex positions for the quad
            float x1 = sin(phi1) * cos(theta1);
            float y1 = cos(phi1);
            float z1 = sin(phi1) * sin(theta1);

            float x2 = sin(phi2) * cos(theta1);
            float y2 = cos(phi2);
            float z2 = sin(phi2) * sin(theta1);

            float x3 = sin(phi2) * cos(theta2);
            float y3 = cos(phi2);
            float z3 = sin(phi2) * sin(theta2);

            float x4 = sin(phi1) * cos(theta2);
            float y4 = cos(phi1);
            float z4 = sin(phi1) * sin(theta2);

            // Texture coordinates (u, v)
            float u1 = (float)seg / (float)segments;
            float v1 = (float)stack / (float)segments;

            float u2 = (float)seg / (float)segments;
            float v2 = (float)(stack + 1) / (float)segments;

            float u3 = (float)(seg + 1) / (float)segments;
            float v3 = (float)(stack + 1) / (float)segments;

            float u4 = (float)(seg + 1) / (float)segments;
            float v4 = (float)stack / (float)segments;

            // First triangle (x1, y1, z1) -> (x2, y2, z2) -> (x3, y3, z3)
            sphere[index++] = x1; sphere[index++] = y1; sphere[index++] = z1;
            sphere[index++] = x1; sphere[index++] = y1; sphere[index++] = z1; // Normal
            sphere[index++] = u1; sphere[index++] = v1;

            sphere[index++] = x2; sphere[index++] = y2; sphere[index++] = z2;
            sphere[index++] = x2; sphere[index++] = y2; sphere[index++] = z2; // Normal
            sphere[index++] = u2; sphere[index++] = v2;

            sphere[index++] = x3; sphere[index++] = y3; sphere[index++] = z3;
            sphere[index++] = x3; sphere[index++] = y3; sphere[index++] = z3; // Normal
            sphere[index++] = u3; sphere[index++] = v3;

            // Second triangle (x1, y1, z1) -> (x3, y3, z3) -> (x4, y4, z4)
            sphere[index++] = x1; sphere[index++] = y1; sphere[index++] = z1;
            sphere[index++] = x1; sphere[index++] = y1; sphere[index++] = z1; // Normal
            sphere[index++] = u1; sphere[index++] = v1;

            sphere[index++] = x3; sphere[index++] = y3; sphere[index++] = z3;
            sphere[index++] = x3; sphere[index++] = y3; sphere[index++] = z3; // Normal
            sphere[index++] = u3; sphere[index++] = v3;

            sphere[index++] = x4; sphere[index++] = y4; sphere[index++] = z4;
            sphere[index++] = x4; sphere[index++] = y4; sphere[index++] = z4; // Normal
            sphere[index++] = u4; sphere[index++] = v4;
        }
    }
}
GLuint VBOS, VAOS;
void setupSphere(int segments) {
    generateSphere(segments);
    glGenVertexArrays(1, &VAOS);
    glGenBuffers(1, &VBOS);

    glBindVertexArray(VAOS);
    glBindBuffer(GL_ARRAY_BUFFER, VBOS);
    glBufferData(GL_ARRAY_BUFFER, numIndicies * sizeof(float), sphere, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(sphere);
    sphere = NULL;
}
/*
    drawCube(GLuint shader,float x, float y, float z, float scaleX, float scaleY, float scaleZ, float ph, float th)
        MUST BE CALLED **AFTER** setupCube() otherwise vertex buffer and attribute data are not bound
        Draws a cube to the screen using the VAO and VBO defined in the setupCube() function
        Cube will be translated to (x,y,z) and scaled by (scaleX,scaleY,scaleZ) in the shader provided (see shaderOps for more info)
        NOTE this will share a vertex buffer attribute object with other cube calls...I ~think this is good
*/
void drawSphere(GLuint shader,glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture) {
    glUseProgram(shader);
    GLint textureLoc = glGetUniformLocation(shader, "image");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLoc, 0);

    //This will update our "model" uniform in the given shader, so Im not sure how this will work with many objects, stay tuned! (it will either draw cube as expected or modify the matrix for all vertex objects!)
    model(shader, position, ph,th, scalar);
    
    glBindVertexArray(VAOS);
    glDrawArrays(GL_TRIANGLES, 0, numVerts);
    glBindVertexArray(0);
}