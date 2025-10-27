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
layout (location = 2) in float aScale;
layout (location = 3) in vec2 aOffset;
out vec3 color;
void main() {
    vec3 pos = (aPos * aScale);
    pos += vec3(aOffset, 0.0);
    gl_Position = vec4(pos, 1.0);
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

    switch (key) {
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
        0.0, 0.0, 0.0, //
        0.0, 1.0, 0.0, //
        1.0, 0.0, 0.0, //
        1.0, 1.0, 0.0, //
    };

    const float colors[] = {
        1.0, 0.0, 0.0, //
        0.0, 1.0, 0.0, //
        0.0, 0.0, 1.0, //
        1.0, 0.0, 1.0, //
    };

    const float iScale[] = {
        0.1, //
        0.2, //
        0.1, //
        0.1, //
        0.1, //
    };

    const float iOffset[] = {
        -1.0, -1.0, //
        0.0, 0.0,   //
        -1.0, 0.8,  //
        -0.8, 0.8,  //
        -0.6, 0.8,  //
    };

    Buffer::Attribute a0(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    Buffer::Attribute a1(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    Buffer::Attribute a2(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), nullptr, 1);
    Buffer::Attribute a3(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr, 1);

    BufferArray array(vector<vector<Buffer::Attribute>> {{a0}, {a1}, {a2}, {a3}});
    array.drawArrays(Buffer::LineStrip, 0, 4);
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(colors), colors);
    array.bufferData(2, sizeof(iScale), iScale);
    array.bufferData(3, sizeof(iOffset), iOffset);
    array.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        array.drawArraysInstanced(Buffer::TriangleStrip, 0, 4, 5);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}