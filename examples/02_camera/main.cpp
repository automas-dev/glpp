#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glpp/extra/Camera.hpp>
using namespace glpp::extra;

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
using namespace std;

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
uniform mat4 vp;
uniform mat4 model;
out vec3 color;
void main() {
    gl_Position = vp * model * vec4(aPos, 1.0);
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

Camera * scam;

static void cursor_pos_callback(GLFWwindow * window, double x, double y) {
    const double s = 0.01;
    static double lastX = x;
    static double lastY = y;
    double dx = x - lastX;
    double dy = y - lastY;
    lastX = x;
    lastY = y;
    scam->rotateDolly({dy * s, dx * s, 0});
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
    int width = 640;
    int height = 480;
    GLFWwindow * window = glfwCreateWindow(width, height, "Camera", NULL, NULL);
    if (!window) {
        cerr << "Failed to create GLFW Window" << endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        throw runtime_error("Could not initialize GLEW");
    }

    glpp::extra::initDebug();

    Camera camera({width, height}, Camera::Perspective, 83.0f, {0, 0.1, 1});
    scam = &camera;

    Shader shader(vertexShaderSource, fragmentShaderSource);
    auto vpUniform = shader.uniform("vp");
    auto modelUniform = shader.uniform("model");

    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f, -0.5f, 0.0f,  // Bottom Right
        0.0f, 0.5f, 0.0f,   // Top Center

        0.0f, -0.5f, 0.0f, //
        1.0f, -0.5f, 0.0f, //
        1.0f, -0.5f, 1.0f, //

        0.5f, 0.0f, 0.0f, //
        0.5f, 1.0f, 0.0f, //
        0.5f, 1.0f, 1.0f, //
    };

    const float colors[] = {
        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center

        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center

        1.0, 0.0, 0.0, // Bottom Left
        0.0, 1.0, 0.0, // Bottom Right
        0.0, 0.0, 1.0, // Top Center
    };

    Buffer::Attribute a0(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    Buffer::Attribute a1(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    BufferArray array(vector<vector<Buffer::Attribute>> {{a0}, {a1}});
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(colors), colors);
    array.unbind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        {
            int w = glfwGetKey(window, GLFW_KEY_W);
            int s = glfwGetKey(window, GLFW_KEY_S);
            int a = glfwGetKey(window, GLFW_KEY_A);
            int d = glfwGetKey(window, GLFW_KEY_D);
            int q = glfwGetKey(window, GLFW_KEY_Q);
            int e = glfwGetKey(window, GLFW_KEY_E);

            if (w || s || a || d || q || e) {
                float dx = (d - a) * 0.1f;
                float dy = (e - q) * 0.1f;
                float dz = (s - w) * 0.1f;
                camera.moveDolly({dx, dy, dz});
            }
        }

        shader.bind();
        vpUniform.setMat4(camera.projMatrix() * camera.viewMatrix());
        modelUniform.setMat4(glm::mat4(1));
        array.drawArrays(Buffer::Triangles, 0, 9);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}