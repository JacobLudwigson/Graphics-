#include "../headers.h"

char* readShaderSource(const char* shaderFile);
GLuint compileShader(GLenum shaderType, const char* source);
GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
void model(GLuint shader,vec3 translation, float ph, float th, vec3 scale);
void projection(GLuint shader,float fov, float near, float far, float asp);
void orthoProjection(GLuint shader, float left, float right, float bottom, float top, float near, float far);
void setViewMatrix(GLuint shader);