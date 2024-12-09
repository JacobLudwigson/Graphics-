#include "light.h"
Light l1 = {
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .specularConstant = 100.0f,
    .lightColor = glm::vec3(0.0,0.0,0.0),
    .lightPos = glm::vec3(0,7, 93),
};
dirLight l4 = {
    .position = glm::vec3(0,7, 93),
    .direction = glm::vec3(0.0,-1.0,0.0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(45.0f)),
    .lightColor = glm::vec3(1.0,0.0,0.0),
    .outerCutoff = glm::cos(glm::radians(90.0f))
};
dirLight powerLight1 = {
    .position = glm::vec3(0,0.6,50),
    .direction = glm::vec3(0.0,1.0,0.0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(45.0f)),
    .lightColor = glm::vec3(1.0,0.0,0.0),
    .outerCutoff = glm::cos(glm::radians(90.0f))
};
dirLight powerLight2 = {
    .position = glm::vec3(0,7.4,50),
    .direction = glm::vec3(0.0,-1.0,0.0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(45.0f)),
    .lightColor = glm::vec3(1.0,0.0,0.0),
    .outerCutoff = glm::cos(glm::radians(90.0f))
};
dirLight tvScreen = {
    .position = glm::vec3(0,5,-5.9),
    .direction = glm::vec3(0.0,0.0,1.0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(100.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(180.0f))
};
dirLight hallwayLight1 = {
    .position = glm::vec3(-10,6.5,86),
    .direction = glm::vec3(0.0,-0.5,0.5),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight2 = {
    .position = glm::vec3(10,6.5,86),
    .direction = glm::vec3(0.0,-0.5,0.5),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight3 = {
    .position = glm::vec3(-10,6.5,14),
    .direction = glm::vec3(0.0,-0.5,-0.5),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight4 = {
    .position = glm::vec3(10,6.5,14),
    .direction = glm::vec3(0.0,-0.5,-0.5),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight5 = {
    .position = glm::vec3(-14.2,6.3,60),
    .direction = glm::vec3(0.5,-0.5,0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight6 = {
    .position = glm::vec3(14.2,6.3,60),
    .direction = glm::vec3(-0.5,-0.5,0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight7 = {
    .position = glm::vec3(-14.2,6.3,40),
    .direction = glm::vec3(0.5,-0.5,0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
dirLight hallwayLight8 = {
    .position = glm::vec3(14.2,6.3,40),
    .direction = glm::vec3(-0.5,-0.5,0),
    .ambientComponent = glm::vec3(1.0,1.0,1.0),
    .diffuseComponent = glm::vec3(1.0,1.0,1.0),
    .specularComponent = glm::vec3(1.0,1.0,1.0),
    .innerCutoff = glm::cos(glm::radians(60.0f)),
    .lightColor = glm::vec3(1.0,1.0,1.0),
    .outerCutoff = glm::cos(glm::radians(30.0f))
};
