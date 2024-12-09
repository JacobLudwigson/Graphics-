#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 texCoordIn;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoordOut;

void main() {
    FragPos = vec3(model * vec4(aPos,1.0));
    //Note if things get slow, we might be able to skip the transpose(inverse(model)) - depending on the matrix
    Normal = aNormal;
    
    gl_Position = model * vec4(aPos, 1.0);
    texCoordOut = texCoordIn;
}