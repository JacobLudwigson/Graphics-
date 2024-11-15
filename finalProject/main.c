#include "dependencies/linker.h"
#define WIDTH 1920.0f
#define HEIGHT 1080.0f
float angleX = 0.0f;
float angleY = 0.0f;

// Handle key input for rotation
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        angleX -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        angleX += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        angleY -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        angleY += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}
// void drawScene(){
//     drawCube(shaderProgram, 0,0,0,0.1,0.1,0.1,angleY,angleX);
//     drawCube(shaderProgram, 10,10,10,10,0.1,0.1,angleY,angleX);
// }

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW version to 3.3 for OpenGL 3.3 Core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle Window", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Compile and link shaders into a program
    GLuint shaderProgram = createShaderProgram("dependencies/shaders/vertexShader.glsl", "dependencies/shaders/fragShader.glsl");
    if (shaderProgram == 0) {
        fprintf(stderr, "Failed to create shader program\n");
        return -1;
    }

    // Vertex data for a triangle
    // float vertices[] = {
    //      0.0f,  0.5f, 0.0f,  // top vertex
    //     -0.5f, -0.5f, 0.0f,  // bottom left vertex
    //      0.5f, -0.5f, 0.0f   // bottom right vertex
    // };

    float aspectRatio = WIDTH/HEIGHT;
    glEnable(GL_DEPTH_TEST);
    // Render loop

    setupCube();
    while (!glfwWindowShouldClose(window)) {

        processInput(window);
        projection(shaderProgram, 60.0f, 0.1f, 100.0f, aspectRatio);
        setViewMatrix(shaderProgram);
        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // drawScene();
        // GLuint shader,float x, float y, float z, float scaleX, float scaleY, float scaleZ, float ph, float th
        drawCube(shaderProgram,0,0,0,1,1,1,angleY,angleX);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
