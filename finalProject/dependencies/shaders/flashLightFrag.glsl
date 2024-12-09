#version 330 core
const int NUMLIGHTS  = 1;
const int NUMDIRLIGHTS  = 13;

struct Light {
    vec3 ambientComponent;
    float pad;
    vec3 diffuseComponent;
    float pad1;    
    vec3 specularComponent;
    float specularConstant;
    vec3 lightColor;
    float pad2;
    vec3 lightPos;
    float pad3;
};

struct dirLight {
    vec3 position;
    float pad4;
    vec3 direction;
    float pad5;
    vec3 ambientComponent;
    float pad;
    vec3 diffuseComponent;
    float pad1;
    vec3 specularComponent;
    float innerCutoff;
    vec3 lightColor;
    float outerCutoff;
};

//We need to define the standard layout of this memory block because we are updating it directly (Dont let the system pack this differently than expected)
layout(std140) uniform LightBlock {
    Light lights[NUMLIGHTS];
};

layout(std140) uniform DirLightBlock {
    dirLight dirLights[NUMDIRLIGHTS];
};

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoordOut;

out vec4 FragColor;

uniform vec3 viewPos;
uniform sampler2D image;

struct Material {
    vec3 ambientReflect;
    vec3 diffuseReflect;
    vec3 specularReflect;
    vec3 objectColor;
};

uniform Material mat;

void main() {
    vec3 lighting = vec3(0,0,0);   
    lighting += vec3(1.0,1.0,1.0);
    vec4 texColor = texture(image, texCoordOut);
    vec3 result = lighting * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}
