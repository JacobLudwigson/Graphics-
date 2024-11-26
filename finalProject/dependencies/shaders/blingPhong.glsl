#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoordOut;

out vec4 FragColor;

uniform vec3 lightPos;          // Light position in world space
uniform vec3 viewPos;           // Viewer position in world space
uniform vec3 lightColor;        // Light color
uniform vec3 objectColor;       // Material color of the object

uniform float ambientStrength;  // Ambient lighting strength
uniform float specularStrength; // Specular lighting strength
uniform float specularConstant; // Specular shininess constant
uniform float diffuseStrength;  // Diffuse lighting strength

uniform sampler2D image;        // Texture sampler

void main() {
    // Calculate lighting vectors
    vec3 lightDir = normalize(lightPos - FragPos);  // Direction from fragment to light
    vec3 viewDir = normalize(viewPos - FragPos);   // Direction from fragment to viewer
    vec3 halfVector = normalize(lightDir + viewDir); // Halfway vector for specular

    // Normalize fragment normal
    vec3 norm = normalize(Normal);

    // Ambient component
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse component
    float N_L = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = N_L * diffuseStrength * lightColor;

    // Specular component
    float N_H = max(dot(norm, halfVector), 0.0);
    vec3 specular = pow(N_H, specularConstant) * specularStrength * lightColor;

    // Combine lighting components
    vec3 lighting = ambient + diffuse + specular;

    // Apply texture color
    vec4 texColor = texture(image, texCoordOut);
    vec3 result = lighting * texColor.rgb;

    // Final fragment color
    FragColor = vec4(result, 1.0);
}
