#include "material.h"

 Material polishedMetal = {
    glm::vec3(0.25f, 0.25f, 0.25f), 
    glm::vec3(0.4f, 0.4f, 0.4f),    
    glm::vec3(0.77f, 0.77f, 0.77f), 
    glm::vec3(0.8f, 0.8f, 0.8f)    
};
 Material darkMetal = {
    glm::vec3(0.25f, 0.25f, 0.25f),  
    glm::vec3(0.4f, 0.4f, 0.4f),    
    glm::vec3(0.77f, 0.77f, 0.77f), 
    glm::vec3(0.2f, 0.2f, 0.2f)    
};
 Material matteSurface = {
    glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.6f, 0.6f, 0.6f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.5f, 0.4f, 0.4f)
};
 Material glossyPlastic = {
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(0.1f, 0.6f, 0.8f)
};
 Material brushedMetal = {
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.5f, 0.5f, 0.5f),
    glm::vec3(0.3f, 0.3f, 0.3f),
    glm::vec3(0.7f, 0.7f, 0.7f)
};
 Material glass = {
    glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.2f, 0.2f, 0.2f),
    glm::vec3(0.9f, 0.9f, 0.9f),
    glm::vec3(0.5f, 0.8f, 0.9f)
};
 Material rubber = {
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.02f, 0.02f, 0.02f),
    glm::vec3(0.1f, 0.1f, 0.1f)
};
Material whitePadding = {
    glm::vec3(0.05f, 0.05f, 0.05f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.02f, 0.02f, 0.02f),
    glm::vec3(1.0f, 1.0f, 1.0f)
};
Material gold = {
    glm::vec3(0.247f, 0.199f, 0.074f),
    glm::vec3(0.751f, 0.606f, 0.226f),
    glm::vec3(0.628f, 0.555f, 0.366f),
    glm::vec3(1.0f, 0.85f, 0.57f)
};
Material fire = {
    glm::vec3(0.847f, 0.099f, 0.074f),
    glm::vec3(0.751f, 0.006f, 0.026f),
    glm::vec3(0.628f, 0.055f, 0.066f),
    glm::vec3(1.0f, 0.05f, 0.07f)
};
Material LightMaterial ={
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f)
};
