#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <FreeImage.h>
#include <time.h>
#include <algorithm>
#include <random>
#include <set>
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/io.hpp>//give me print operator overloading for vec3 types please!
#define Cos(x) (cos((x)*3.14159265/180))
#define Sin(x) (sin((x)*3.14159265/180))
#define NUMLIGHTSINSCENE 1
#define NUMDIRLIGHTSINSCENE 13
struct Material {
    glm::vec3 ambientReflect;
    glm::vec3 diffuseReflect;
    glm::vec3 specularReflect;
    glm::vec3 objectColor;
};

struct Light {
    glm::vec3 ambientComponent = glm::vec3(0,0,0);
    float pad;
    glm::vec3 diffuseComponent = glm::vec3(0,0,0);
    float pad1;
    glm::vec3 specularComponent = glm::vec3(0,0,0);
    float specularConstant = 0;
    glm::vec3 lightColor = glm::vec3(0,0,0);
    float pad2;
    glm::vec3 lightPos = glm::vec3(0,0,0);
    float pad3;
};
struct dirLight {
    glm::vec3 position= glm::vec3(0,0,0);
    float pad4;
    glm::vec3 direction= glm::vec3(0,0,0);
    float pad5;
    glm::vec3 ambientComponent= glm::vec3(0,0,0);
    float pad;
    glm::vec3 diffuseComponent= glm::vec3(0,0,0);
    float pad1;
    glm::vec3 specularComponent = glm::vec3(0,0,0);
    float innerCutoff;
    glm::vec3 lightColor= glm::vec3(0,0,0);
    float outerCutoff;
};
extern std::vector<Light> lightsInScene;
extern std::vector<dirLight> dirLightsInScene;