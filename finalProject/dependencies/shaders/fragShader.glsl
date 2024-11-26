#version 330 core
const int NUMLIGHTS  = 2;
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

//We need to define the standard layout of this memory block because we are updating it directly (Dont let the system pack this differently than expected)
layout(std140) uniform LightBlock {
    Light lights[NUMLIGHTS];
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
    vec3 lightPos;    
    for (int i = 0; i < NUMLIGHTS; i++){
        lightPos = lights[i].lightPos;
        vec3 ambient = lights[i].ambientComponent * lights[i].lightColor * mat.ambientReflect;

        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].lightPos - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffusel = diff * lights[i].diffuseComponent *  lights[i].lightColor * mat.diffuseReflect;

        // Specular lighting
        vec3 viewDir = normalize(viewPos - FragPos);
        // Half Phong
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), lights[i].specularConstant);

        vec3 specular = lights[i].specularComponent * spec * lights[i].lightColor * mat.specularReflect;

        // Combine all lighting components
        //lighting += ambient;
        lighting += (ambient + diffusel + specular) * mat.objectColor;
    }
    vec4 texColor = texture(image, texCoordOut);

    vec3 result = lighting * texColor.rgb;

    FragColor = vec4(result, 1.0);
}