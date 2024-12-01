#include "../materials/material.h"
#define degreesToRadians(x) x*(3.141592f/180.0f)

char* readShaderSource(const char* shaderFile);
GLuint compileShader(GLenum shaderType, const char* source);
GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
void model(GLuint shader,glm::vec3 translation, float ph, float th, glm::vec3 scale);
void projection(GLuint shader,float fov, float near, float far, float asp);
void orthoProjection(GLuint shader, float left, float right, float bottom, float top, float near, float far);
void setViewMatrix(GLuint shader, glm::vec3 playerPosition, int angleX, int angleY);
void initLightingUniforms(GLuint shader);
void initDirLightingUniforms(GLuint shader);
void setFragLightingUniforms(GLuint shader,glm::vec3 playerPos);
void setMaterialUniforms(GLuint shaderProgram, Material& material);