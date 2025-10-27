#include <glpp/Buffer.hpp>
#include <glpp/FrameBuffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glpp/extra/Camera.hpp>
#include <glpp/extra/Grid.hpp>
#include <glpp/extra/Quad.hpp>
using namespace glpp::extra;

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace glm;

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
out vec2 FragTex;
void main() {
    gl_Position = vec4(aPos, 1.0);
    FragTex = aTex;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec2 FragTex;
out vec4 FragColor;
uniform sampler2D gTexture;
void main() {
    FragColor = texture(gTexture, FragTex);
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
    // Don't call glViewport here because it should be the viewport
    // for rendering, not for blit
    // glViewport(0, 0, width, height);
    FrameBuffer::getDefault().resize(uvec2(width, height));
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
        throw runtime_error("Could not initVertexArrayBufferialize GLEW");
    }

    glpp::extra::initDebug();

    Camera camera({width, height}, Camera::Perspective, 83.0f, {0, 0.2, 3});
    scam = &camera;

    Shader shader(vertexShaderSource, fragmentShaderSource);
    Texture texture = Texture::fromPath("../../../examples/04_framebuffer/uv.png");

    const float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Bottom Left
        0.5f, -0.5f, 0.0f,  // Bottom Right
        0.0f, 0.5f, 0.0f    // Top Center
    };

    const float texCoords[] = {
        -0.5f, -0.5f, // Bottom Left
        0.5f, -0.5f,  // Bottom Right
        0.0f, 0.5f,   // Top Center
    };

    const unsigned int indices[] = {
        0, 1, 2, // First Triangle
    };

    Buffer::Attribute a0(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    Buffer::Attribute a1(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

    BufferArray array(vector<vector<Buffer::Attribute>> {{a0}, {a1}});
    array.bind();
    array.bufferData(0, sizeof(vertices), vertices);
    array.bufferData(1, sizeof(texCoords), texCoords);
    array.bufferElements(sizeof(indices), indices);
    array.unbind();

    Quad quad({0, 0}, {1, 1});

    // Needed because FrameBuffer is storing size for future blit to default
    FrameBuffer::getDefault().resize(uvec2(width, height));

    FrameBuffer fbo(uvec2(width, height));

    RenderBuffer::Ptr rbo = make_shared<RenderBuffer>(uvec2(width, height), GL_DEPTH24_STENCIL8);
    fbo.attach(rbo, GL_DEPTH_STENCIL_ATTACHMENT);

    Texture::Ptr tex2 = make_shared<Texture>(uvec2(width, height),
                                             Texture::RGBA,
                                             Texture::RGBA,
                                             GL_FLOAT,
                                             0,
                                             Texture::Linear,
                                             Texture::Linear,
                                             Texture::Clamp,
                                             false);
    fbo.attach(tex2, GL_COLOR_ATTACHMENT0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cerr << "FBO is not complete!" << endl;
        return 1;
    }
    FrameBuffer::getDefault().bind();

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

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

        fbo.bind();
        fbo.setViewport();
        fbo.clear();

        shader.bind();
        texture.bind();
        array.drawElements(Buffer::Triangles, 3, GL_UNSIGNED_INT, 0);

        FrameBuffer::getDefault().bind();
        FrameBuffer::getDefault().setViewport();
        FrameBuffer::getDefault().clear();

        // You should notice some aliasing since fbo is not being resized when
        // the window is
        FrameBuffer::getDefault().blit(fbo);

        shader.bind();
        tex2->bind();
        quad.draw();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}