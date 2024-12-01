#include "shaderOps.h"
// Read shader source from file
std::vector<Light> lightsInScene(NUMLIGHTSINSCENE);
std::vector<dirLight> dirLightsInScene(NUMDIRLIGHTSINSCENE);

char* readShaderSource(const char* shaderFile) {
    FILE* file = fopen(shaderFile, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s\n", shaderFile);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer) {
        fread(buffer, 1, fileSize, file);
        buffer[fileSize] = '\0';
    }
    fclose(file);
    return buffer;
}

// Compile shader
GLuint compileShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        char* log = (char*)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, NULL, log);
        fprintf(stderr, "Shader compilation failed: %s\n", log);
        free(log);

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

// Create shader program
GLuint createShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
    char* vertexSource = readShaderSource(vertexShaderPath);
    char* fragmentSource = readShaderSource(fragmentShaderPath);
    if (!vertexSource || !fragmentSource) {
        return 0;
    }

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    free(vertexSource);
    free(fragmentSource);

    if (!vertexShader || !fragmentShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        char* log = (char*)malloc(logLength);
        glGetProgramInfoLog(program, logLength, NULL, log);
        fprintf(stderr, "Program linking failed: %s\n", log);
        free(log);

        glDeleteProgram(program);
        return 0;
    }

    return program;
}
/*
    void mdoel(GLuint shader,float x, float y, float z, float th, float ph):
        Defines a uniform in the vertex shader to rotate vertices in the scene by (th,ph) and translate by (x,y,z)
*/
void model(GLuint shader,glm::vec3 translation, float ph, float th, glm::vec3 scale) {
    /*
        Rotate by where user is looking and pass that rotation matrix to our shader
    */
    GLint transformLoc = glGetUniformLocation(shader, "model");

    mat4 transform = GLM_MAT4_IDENTITY_INIT; // Start with identity matrix
    glm_translate(transform, &translation[0]);
    glm_rotate_x(transform, degreesToRadians(ph), transform);
    glm_rotate_y(transform, degreesToRadians(th), transform);
    glm_scale(transform, &scale[0]);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);
}
/*
    void projection(GLuint shader,float fov, float near, float far, float asp):
        Defines a uniform in the vertex shader to transform the scene based on the perspective projection
    FOV = Degrees 
    near = Near clipping plane (suggested 0.1f)
    far = Near clipping plane (suggested 0.1f)
    asp = Aspect Ratio, calculated as float(width)/float(height)
*/
void projection(GLuint shader,float fov, float near, float far, float asp) {
    /*
        Rotate by where user is looking and pass that rotation matrix to our shader
    */
    GLint projectionLoc = glGetUniformLocation(shader, "projection");
    if(projectionLoc == -1){
        printf("FAILURE!");
    }
    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    float radfov = glm_rad(fov);
    glm_perspective(radfov, asp, near, far, projection);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
}
void orthoProjection(GLuint shader, float left, float right, float bottom, float top, float near, float far) {
    GLint projectionLoc = glGetUniformLocation(shader, "projection");
    mat4 projection;
    glm_ortho(left, right, bottom, top, near, far, projection); // Use an orthographic projection
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
}
void setViewMatrix(GLuint shader, glm::vec3 playerPosition, int angleX, int angleY) {
    GLint viewLoc = glGetUniformLocation(shader, "view");

    glm::vec3 direction;
    direction.x = -Sin(angleX) * Cos(angleY);
    direction.y = Sin(angleY);
    direction.z = Cos(angleX) * Cos(angleY);

    direction = glm::normalize(direction);
    glm::vec3 eye = playerPosition;
    glm::vec3 center = eye + direction;
    glm::vec3 up = {0.0f, 1.0f, 0.0f};

    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix)); 
}
GLuint ubo;
void initLightingUniforms(GLuint shader){
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Light) * NUMLIGHTSINSCENE, lightsInScene.data(), GL_STATIC_DRAW);
    GLuint blockIndex = glGetUniformBlockIndex(shader, "LightBlock");
    glUniformBlockBinding(shader, blockIndex, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo);
}
GLuint uboD;
void initDirLightingUniforms(GLuint shader){
    glGenBuffers(1, &uboD);
    glBindBuffer(GL_UNIFORM_BUFFER, uboD);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dirLight) * NUMDIRLIGHTSINSCENE, dirLightsInScene.data(), GL_STATIC_DRAW);
    GLuint blockIndex = glGetUniformBlockIndex(shader, "DirLightBlock");
    glUniformBlockBinding(shader, blockIndex, 1);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboD);
}
void setFragLightingUniforms(GLuint shader, glm::vec3 playerPos){
    GLint viewPosLoc = glGetUniformLocation(shader, "viewPos"); 
    glUniform3fv(viewPosLoc, 1, &playerPos[0]);
    //To be totally honest I am not quite sure how this works, but it works! Uniform buffer objects!
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Light) * NUMLIGHTSINSCENE, lightsInScene.data());
    glBindBuffer(GL_UNIFORM_BUFFER, uboD);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(dirLight) * NUMDIRLIGHTSINSCENE, dirLightsInScene.data());
}
void setMaterialUniforms(GLuint shaderProgram, Material& material) {
    GLint ambientLoc = glGetUniformLocation(shaderProgram, "mat.ambientReflect");
    glUniform3fv(ambientLoc, 1, glm::value_ptr(material.ambientReflect));
    // if (ambientLoc == -1) {
    //     // std::cerr << "Error: 'mat.ambientReflect' uniform not found!" << std::endl;
    // }
    // else{
    //     printf("AMBIENT LOC FOUND & SET!\n");
    // }
    glUniform3fv(glGetUniformLocation(shaderProgram, "mat.diffuseReflect"), 1, glm::value_ptr(material.diffuseReflect));
    glUniform3fv(glGetUniformLocation(shaderProgram, "mat.specularReflect"), 1, glm::value_ptr(material.specularReflect));
    glUniform3fv(glGetUniformLocation(shaderProgram, "mat.objectColor"), 1, glm::value_ptr(material.objectColor));
}