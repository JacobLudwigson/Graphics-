#include "../shaderOps/shaderOps.h"
#define NUMVERTICESCUBE 108
void setupCube();
void drawCube(GLuint shader,glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture);