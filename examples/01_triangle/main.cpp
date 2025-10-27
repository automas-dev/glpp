#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
using namespace std;

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 color;
void main() {
    gl_Position = vec4(aPos, 1.0);
    color = aCol;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;
void main() {
    FragColor = vec4(color, 1.0);
})";

void error_callback(int error, const char * description) {
    cerr << "Error: " << description << endl;
}

static void key_callback(
    GLFWwindow * window, int key, int scancode, int action, int mods) {

    float val = 0.5;
    if (mods & GLFW_MOD_CONTROL) {
        val += 0.25;
    }
    if (mods & GLFW_MOD_SHIFT) {
        val -= 0.25;
    }
    switch (key) {
        case GLFW_KEY_R:
            glClearColor(val, 0.0, 0.0, val);
            break;
        case GLFW_KEY_G:
            glClearColor(0.0, val, 0.0, val);
            break;
        case GLFW_KEY_B:
            glClearColor(0.0, 0.0, val, val);
            break;
        case GLFW_KEY_M:
            glClearColor(val, 0.0, val, val);
            break;
        case GLFW_KEY_C:
            glClearColor(0.0, val, val, val);
            break;
        case GLFW_KEY_Y:
            glClearColor(val, val, 0.0, val);
            break;
        case GLFW_KEY_K:
            glClearColor(0.0, 0.0, 0.0, val);
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:
            break;
    }
}

static void framebuffer_size_callback(GLFWwindow * window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow * window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW Window" << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw runtime_error("Could not initialize GLEW");
    }

    glpp::extra::initDebug();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    Shader shader(vertexShaderSource, fragmentShaderSource);

    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f, -0.5f, 0.0f,  // Bottom Right
        0.0f, 0.5f, 0.0f    // Top Center
    };

    const float colors[] = {
        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0  // Top Center
    };

    const unsigned int indices[] = {
        0, 1, 2, // First Triangle
    };

    Buffer::Attribute a0(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    Buffer::Attribute a1(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    BufferArray array(vector<vector<Buffer::Attribute>> {{a0}, {a1}});
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(colors), colors);
    array.bufferElements(sizeof(indices), indices);
    array.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        array.drawArrays(Buffer::Triangles, 0, 3);
        array.drawElements(Buffer::Triangles, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}