#include "cylinder.h"

float* cylinderVertices = NULL; // Pointer for cylinder vertices
size_t numVertices = 0;         // Total number of vertices

GLuint VBOC, VAOC;

/*
    This code is a combination of a few different tutorials + GPT + my own modifications
    Tutorial 1:https://www.songho.ca/opengl/gl_cylinder.html

*/
void generateCylinder(int segments) {
    const float PI = 3.14159265359f;
    float angleStep = (2.0f * PI) / segments;

    // Each segment contributes 2 triangles for the base, 2 for the top, and 4 for the side (2 triangles per quad)
    numVertices = segments * (12); // 6 vertices for base and top, 12 for sides
    size_t numFloats = numVertices * 8; // Each vertex has 6 floats (position + normal)

    // Allocate memory for vertices
    cylinderVertices = (float*)malloc(numFloats * sizeof(float));

    size_t offset = 0;

    for (int i = 0; i < segments; ++i) {
        float angle = i * angleStep;
        float nextAngle = (i + 1) * angleStep;

        // Base center - we are building a triangle fan, so all of the triangles on the bottom will share this vertex
        float bx = 0.0f, by = -1.0f, bz = 0.0f;
        float baseCenterUV[] = {0.5f, 0.5f};

        // Top center
        float tx = 0.0f, ty = 1.0f, tz = 0.0f;
        float topCenterUV[] = {0.5f, 0.5f};

        // Points on the base and top
        float b1x = cosf(angle), b1z = sinf(angle);
        float b2x = cosf(nextAngle), b2z = sinf(nextAngle);
        float t1x = b1x, t1z = b1z;
        float t2x = b2x, t2z = b2z;

        // Base UVs
        float b1UV[] = {0.5f + b1x * 0.5f, 0.5f + b1z * 0.5f};
        float b2UV[] = {0.5f + b2x * 0.5f, 0.5f + b2z * 0.5f};

        // Top UVs
        float t1UV[] = {0.5f + t1x * 0.5f, 0.5f + t1z * 0.5f};
        float t2UV[] = {0.5f + t2x * 0.5f, 0.5f + t2z * 0.5f};

        // Side UVs
        float sideUV1[] = {float(i) / segments, 0.0f};
        float sideUV2[] = {float(i + 1) / segments, 0.0f};
        float sideUV3[] = {float(i) / segments, 1.0f};
        float sideUV4[] = {float(i + 1) / segments, 1.0f};

        // Base triangle
        float base[] = {
            bx, by, bz, 0.0f, -1.0f, 0.0f, baseCenterUV[0], baseCenterUV[1],
            b1x, by, b1z, 0.0f, -1.0f, 0.0f, b1UV[0], b1UV[1],
            b2x, by, b2z, 0.0f, -1.0f, 0.0f, b2UV[0], b2UV[1],
        };

        // Top triangle
        float top[] = {
            tx, ty, tz, 0.0f, 1.0f, 0.0f, topCenterUV[0], topCenterUV[1],
            t1x, ty, t1z, 0.0f, 1.0f, 0.0f, t1UV[0], t1UV[1],
            t2x, ty, t2z, 0.0f, 1.0f, 0.0f, t2UV[0], t2UV[1],
        };

        // Side quads (split into two triangles)
        float side[] = {
            b1x, by, b1z, b1x, 0.0f, b1z, sideUV1[0], sideUV1[1],
            t1x, ty, t1z, b1x, 0.0f, b1z, sideUV3[0], sideUV3[1],
            b2x, by, b2z, b2x, 0.0f, b2z, sideUV2[0], sideUV2[1],

            t1x, ty, t1z, b1x, 0.0f, b1z, sideUV3[0], sideUV3[1],
            t2x, ty, t2z, b2x, 0.0f, b2z, sideUV4[0], sideUV4[1],
            b2x, by, b2z, b2x, 0.0f, b2z, sideUV2[0], sideUV2[1],
        };

        // Copy data to the main buffer
        memcpy(&cylinderVertices[offset], base, sizeof(base));
        offset += sizeof(base) / sizeof(float);

        memcpy(&cylinderVertices[offset], top, sizeof(top));
        offset += sizeof(top) / sizeof(float);

        memcpy(&cylinderVertices[offset], side, sizeof(side));
        offset += sizeof(side) / sizeof(float);
    }

}

void setupCylinder(int segments) {
    generateCylinder(segments);

    glGenVertexArrays(1, &VAOC);
    glGenBuffers(1, &VBOC);

    glBindVertexArray(VAOC);
    glBindBuffer(GL_ARRAY_BUFFER, VBOC);

    glBufferData(GL_ARRAY_BUFFER, numVertices * 8 * sizeof(float), cylinderVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    free(cylinderVertices);
    cylinderVertices = NULL;
}

void drawCylinder(GLuint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture) {
    glUseProgram(shader);
    GLint textureLoc = glGetUniformLocation(shader, "image");

    model(shader, position, ph, th, scalar);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(textureLoc, 0);

    glBindVertexArray(VAOC);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}
