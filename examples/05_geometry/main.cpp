#include <glpp/Buffer.hpp>
#include <glpp/FrameBuffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glpp/extra/Camera.hpp>
#include <glpp/extra/GeometryBuffer.hpp>
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

static const char * screenVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
out vec2 FragTex;
void main() {
    gl_Position = vec4(aPos, 1.0);
    FragTex = aTex;
})";

static const char * screenFragmentShaderSource = R"(
#version 330 core
in vec2 FragTex;
out vec4 FragColor;
uniform sampler2D gTexture;
void main() {
    FragColor = texture(gTexture, FragTex);
})";

static const char * lightPassVertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
out vec2 FragTex;
void main() {
    gl_Position = vec4(aPos, 1.0);
    FragTex = aTex;
})";

static const char * lightPassFragmentShaderSource = R"(
#version 330 core
in vec2 FragTex;
out vec4 FragColor;
uniform sampler2D gDiffuse;
uniform sampler2D gNormal;
uniform sampler2D gPosition;
uniform sampler2D gSpecular;
uniform vec3 lightPos = vec3(1, 2, 3);
uniform vec3 lightColor = vec3(1.0);
void main() {
    vec3 FragDiff = texture(gDiffuse, FragTex).xyz;
    vec3 FragPos = texture(gPosition, FragTex).xyz;
    vec3 FragNorm = texture(gNormal, FragTex).xyz;

    float ambStr = 0.1;
    vec3 ambient = ambStr * lightColor;

    vec3 norm = normalize(FragNorm);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 objColor = FragDiff;
    vec4 res = vec4((ambient + diffuse) * objColor, 1.0);
    
    FragColor = res;
})";

static vector<Vertex> cube = {
    {glm::vec3(0.292893,  0.000000,  1.707107),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.625000, 0.000000)},
    {glm::vec3(-1.707107, 0.000000,  -0.292893),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.375000, 0.250000)},
    {glm::vec3(-0.707107, -1.414214, 0.707107),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.375000, 0.000000)},
    {glm::vec3(-0.707107, 1.414214,  0.707107),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.625000, 0.250000)},
    {glm::vec3(-0.292893, 0.000000,  -1.707107),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.375000, 0.500000)},
    {glm::vec3(-1.707107, 0.000000,  -0.292893),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.375000, 0.250000)},
    {glm::vec3(0.707107,  1.414214,  -0.707107),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.625000, 0.500000)},
    {glm::vec3(0.707107,  -1.414214, -0.707107),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.375000, 0.750000)},
    {glm::vec3(-0.292893, 0.000000,  -1.707107),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.375000, 0.500000)},
    {glm::vec3(1.707107,  0.000000,  0.292893),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.625000, 0.750000)},
    {glm::vec3(-0.707107, -1.414214, 0.707107),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.375000, 1.000000)},
    {glm::vec3(0.707107,  -1.414214, -0.707107),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.375000, 0.750000)},
    {glm::vec3(-0.292893, 0.000000,  -1.707107),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.375000, 0.500000)},
    {glm::vec3(-0.707107, -1.414214, 0.707107),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.125000, 0.750000)},
    {glm::vec3(-1.707107, 0.000000,  -0.292893),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.125000, 0.500000)},
    {glm::vec3(-0.707107, 1.414214,  0.707107),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.875000, 0.500000)},
    {glm::vec3(1.707107,  0.000000,  0.292893),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.625000, 0.750000)},
    {glm::vec3(0.707107,  1.414214,  -0.707107),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.625000, 0.500000)},
    {glm::vec3(0.292893,  0.000000,  1.707107),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.625000, 0.000000)},
    {glm::vec3(-0.707107, 1.414214,  0.707107),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.625000, 0.250000)},
    {glm::vec3(-1.707107, 0.000000,  -0.292893),
     glm::vec3(-0.707100, 0.000000,  0.707100),  glm::vec2(0.375000, 0.250000)},
    {glm::vec3(-0.707107, 1.414214,  0.707107),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.625000, 0.250000)},
    {glm::vec3(0.707107,  1.414214,  -0.707107),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.625000, 0.500000)},
    {glm::vec3(-0.292893, 0.000000,  -1.707107),
     glm::vec3(-0.500000, 0.707100,  -0.500000), glm::vec2(0.375000, 0.500000)},
    {glm::vec3(0.707107,  1.414214,  -0.707107),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.625000, 0.500000)},
    {glm::vec3(1.707107,  0.000000,  0.292893),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.625000, 0.750000)},
    {glm::vec3(0.707107,  -1.414214, -0.707107),
     glm::vec3(0.707100,  0.000000,  -0.707100), glm::vec2(0.375000, 0.750000)},
    {glm::vec3(1.707107,  0.000000,  0.292893),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.625000, 0.750000)},
    {glm::vec3(0.292893,  0.000000,  1.707107),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.625000, 1.000000)},
    {glm::vec3(-0.707107, -1.414214, 0.707107),
     glm::vec3(0.500000,  -0.707100, 0.500000),  glm::vec2(0.375000, 1.000000)},
    {glm::vec3(-0.292893, 0.000000,  -1.707107),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.375000, 0.500000)},
    {glm::vec3(0.707107,  -1.414214, -0.707107),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.375000, 0.750000)},
    {glm::vec3(-0.707107, -1.414214, 0.707107),
     glm::vec3(-0.500000, -0.707100, -0.500000), glm::vec2(0.125000, 0.750000)},
    {glm::vec3(-0.707107, 1.414214,  0.707107),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.875000, 0.500000)},
    {glm::vec3(0.292893,  0.000000,  1.707107),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.875000, 0.750000)},
    {glm::vec3(1.707107,  0.000000,  0.292893),
     glm::vec3(0.500000,  0.707100,  0.500000),  glm::vec2(0.625000, 0.750000)},
};

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

struct TextureViewport {
    Texture::ConstPtr tex;
    Quad quad;

    TextureViewport(Texture::ConstPtr & tex, Quad && quad)
        : tex(tex), quad(std::move(quad)) {}

    void draw() const {
        glDisable(GL_DEPTH_TEST);
        tex->bind();
        quad.draw();
    }
};

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

    glViewport(0, 0, width, height);
    FrameBuffer::getDefault().resize(uvec2(width, height));

    Camera camera({width, height}, Camera::Perspective, 83.0f, {0, 0.2, 3});
    scam = &camera;

    Texture texture = Texture::fromPath("../../../examples/05_geometry/uv.png");

    Grid grid(10, {1, 1, 1, 1}, true);

    VertexBufferArray vba;
    vba.bufferData(cube);
    vba.unbind();

    GeometryBuffer gb(uvec2(width, height));
    if (!gb.isComplete())
        throw runtime_error("GBO is not complete");
    gb.unbind();

    Shader & gShader = GeometryBuffer::getShader();
    auto gvp = gShader.uniform("vp");
    auto gmodel = gShader.uniform("model");

    Shader screenShader(screenVertexShaderSource, screenFragmentShaderSource);
    Shader lightPassShader(lightPassVertexShaderSource,
                           lightPassFragmentShaderSource);
    auto lGdiff = lightPassShader.uniform("gDiffuse");
    auto lGnorm = lightPassShader.uniform("gNormal");
    auto lGpos = lightPassShader.uniform("gPosition");


    TextureViewport diffTV(gb.diffuse, Quad({0.5, 0.5}, {0.5, 0.5}));
    TextureViewport normTV(gb.normal, Quad({0.5, 0}, {0.5, 0.5}));
    TextureViewport posTV(gb.position, Quad({0.5, -0.5}, {0.5, 0.5}));

    Quad screenQuad;

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

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

        gb.bind();
        gb.setViewport();
        gb.clear();

        {
            glEnable(GL_DEPTH_TEST);
            gShader.bind();
            texture.bind();
            gvp.setMat4(camera.projMatrix() * camera.viewMatrix());
            gmodel.setMat4(glm::mat4(1));
            vba.drawArrays(Buffer::Triangles, 0, cube.size());
        }

        FrameBuffer::getDefault().bind();
        FrameBuffer::getDefault().setViewport();
        FrameBuffer::getDefault().clear();

        // You should notice some aliasing since fbo is not being resized when
        // the window is
        // FrameBuffer::getDefault().blit(gb);

        lightPassShader.bind();
        glDisable(GL_DEPTH_TEST);
        gb.bindTextures();
        lGdiff.setInt(0);
        lGnorm.setInt(1);
        lGpos.setInt(2);
        screenQuad.draw();

        {
            // glEnable(GL_DEPTH_TEST);
            // grid.draw(camera.projMatrix() * camera.viewMatrix());

            glDisable(GL_DEPTH_TEST);
            screenShader.bind();

            diffTV.draw();
            normTV.draw();
            posTV.draw();
        }

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}