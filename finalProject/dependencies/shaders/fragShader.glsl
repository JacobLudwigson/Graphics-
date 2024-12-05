#version 330 core
const int NUMLIGHTS  = 1;
const int NUMDIRLIGHTS  = 2;

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
    for (int i = 0; i < NUMLIGHTS; i++){
        vec3 ambient = lights[i].ambientComponent * lights[i].lightColor * mat.ambientReflect;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].lightPos - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffusel = diff * lights[i].diffuseComponent *  lights[i].lightColor * mat.diffuseReflect;

        vec3 viewDir = normalize(viewPos - FragPos);
        // Half Phong
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), lights[i].specularConstant);

        vec3 specular = lights[i].specularComponent * spec * lights[i].lightColor * mat.specularReflect;

        //lighting += ambient;
        lighting += (ambient + diffusel + specular) * mat.objectColor;
    }

    //This spotlight lighting formula is made following this tutorial (with some modifications): https://opentk.net/learn/chapter2/5-light-casters.html 
    //NOTE: maybe tweak the attenuation terms to adjust some brightness problems
    for (int i = 0; i < NUMDIRLIGHTS; i++) {
        vec3 spotlightPos = dirLights[i].position;
        vec3 spotlightDir = normalize(-dirLights[i].direction); // Spotlight direction (toward the object)

        vec3 ambient = dirLights[i].ambientComponent * dirLights[i].lightColor * mat.ambientReflect;

        vec3 lightDir = normalize(spotlightPos - FragPos);
        float distance = length(spotlightPos - FragPos);

        float theta = dot(lightDir, spotlightDir);
        float epsilon = dirLights[i].innerCutoff - dirLights[i].outerCutoff;
        float spotlightIntensity = clamp((theta - dirLights[i].outerCutoff) / epsilon, 0.0, 1.0);

        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * dirLights[i].diffuseComponent * dirLights[i].lightColor * mat.diffuseReflect;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // 32 is the shininess
        vec3 specular = spec * dirLights[i].specularComponent * dirLights[i].lightColor * mat.specularReflect;

        //1.0 constAt + 0.09 linAt + 0.032 quadAt looks pretty mean but maybe not bright enough
        float constAt = 0.25;
        float linearAt = 0.05;
        float quadAt = 0.032;
        float attenuation = 1.0 / (constAt + linearAt * distance + quadAt * (distance * distance)); // Quadratic attenuation 
        vec3 spotlight = (ambient + diffuse + specular) * spotlightIntensity * attenuation;

        lighting += spotlight * mat.objectColor;
    }
    vec4 texColor = texture(image, texCoordOut);
    vec3 result = lighting * texColor.rgb;

    FragColor = vec4(result, texColor.a);
}
