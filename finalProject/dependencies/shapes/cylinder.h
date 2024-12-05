#include "cube.h"

void generateCylinder(int segments);
void setupCylinder(int segments);
void drawCylinder(GLuint shader, glm::vec3 position, glm::vec3 scalar, float ph, float th, unsigned int texture);