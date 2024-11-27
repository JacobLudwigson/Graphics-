#include "material.h"

 Material polishedMetal = {
    glm::vec3(0.25f, 0.25f, 0.25f),  // ambientReflect: subdued but noticeable ambient lighting
    glm::vec3(0.4f, 0.4f, 0.4f),    // diffuseReflect: reflects light well but not as much as specular
    glm::vec3(0.77f, 0.77f, 0.77f), // specularReflect: strong reflective highlights
    glm::vec3(0.8f, 0.8f, 0.8f)     // objectColor: near-white metallic color
};
 Material matteSurface = {
    glm::vec3(0.1f, 0.1f, 0.1f),    // ambientReflect: minimal ambient contribution
    glm::vec3(0.6f, 0.6f, 0.6f),    // diffuseReflect: strong diffuse component
    glm::vec3(0.0f, 0.0f, 0.0f),    // specularReflect: no specular highlights
    glm::vec3(0.5f, 0.4f, 0.4f)     // objectColor: muted reddish tone
};
 Material glossyPlastic = {
    glm::vec3(0.2f, 0.2f, 0.2f),    // ambientReflect: noticeable ambient light
    glm::vec3(0.4f, 0.4f, 0.4f),    // diffuseReflect: moderate diffuse reflection
    glm::vec3(0.5f, 0.5f, 0.5f),    // specularReflect: noticeable highlights
    glm::vec3(0.1f, 0.6f, 0.8f)     // objectColor: cyan-blue tone
};
 Material brushedMetal = {
    glm::vec3(0.2f, 0.2f, 0.2f),    // ambientReflect: subdued lighting
    glm::vec3(0.5f, 0.5f, 0.5f),    // diffuseReflect: moderately diffuse
    glm::vec3(0.3f, 0.3f, 0.3f),    // specularReflect: subtle highlights
    glm::vec3(0.7f, 0.7f, 0.7f)     // objectColor: light gray metallic color
};
 Material glass = {
    glm::vec3(0.1f, 0.1f, 0.1f),    // ambientReflect: minimal ambient contribution
    glm::vec3(0.2f, 0.2f, 0.2f),    // diffuseReflect: minimal diffuse reflection
    glm::vec3(0.9f, 0.9f, 0.9f),    // specularReflect: strong highlights for smoothness
    glm::vec3(0.5f, 0.8f, 0.9f)     // objectColor: light bluish tint
};
 Material rubber = {
    glm::vec3(0.05f, 0.05f, 0.05f), // ambientReflect: low ambient reflection
    glm::vec3(0.4f, 0.4f, 0.4f),    // diffuseReflect: moderate diffuse component
    glm::vec3(0.02f, 0.02f, 0.02f), // specularReflect: minimal specular highlights
    glm::vec3(0.1f, 0.1f, 0.1f)     // objectColor: near-black
};
Material whitePadding = {
    glm::vec3(0.05f, 0.05f, 0.05f), // ambientReflect: low ambient reflection
    glm::vec3(0.4f, 0.4f, 0.4f),    // diffuseReflect: moderate diffuse component
    glm::vec3(0.02f, 0.02f, 0.02f), // specularReflect: minimal specular highlights
    glm::vec3(1.0f, 1.0f, 1.0f)     // objectColor: near-black
};
Material gold = {
    glm::vec3(0.247f, 0.199f, 0.074f), // ambientReflect: gold-colored ambient lighting
    glm::vec3(0.751f, 0.606f, 0.226f), // diffuseReflect: rich gold diffuse reflection
    glm::vec3(0.628f, 0.555f, 0.366f), // specularReflect: strong specular highlights
    glm::vec3(1.0f, 0.85f, 0.57f)      // objectColor: bright gold
};
Material fire = {
    glm::vec3(0.847f, 0.099f, 0.074f), // ambientReflect: gold-colored ambient lighting
    glm::vec3(0.751f, 0.006f, 0.026f), // diffuseReflect: rich gold diffuse reflection
    glm::vec3(0.628f, 0.055f, 0.066f), // specularReflect: strong specular highlights
    glm::vec3(1.0f, 0.05f, 0.07f)      // objectColor: bright gold
};
