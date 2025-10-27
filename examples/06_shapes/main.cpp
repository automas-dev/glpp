#include <glpp/Buffer.hpp>
#include <glpp/Shader.hpp>
#include <glpp/extra/debug.hpp>
using namespace glpp;

#include <glpp/extra/Camera.hpp>
#include <glpp/extra/Grid.hpp>
#include <glpp/extra/Line.hpp>
#include <glpp/extra/Marker.hpp>
using namespace glpp::extra;

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
using namespace std;

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
    GLFWwindow * window = glfwCreateWindow(width, height, "Shapes", NULL, NULL);
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

    Line line1(
        std::vector<glm::vec3> {
            {1,  2, 1 },
            {1,  2, -1},
            {-1, 2, -1},
            {-1, 2, 1 },
    },
        glm::vec4(1.0, 0.0, 0.0, 1.0), Line::Lines);

    Line line2(
        std::vector<glm::vec3> {
            {1,  1, 1 },
            {1,  1, -1},
            {-1, 1, -1},
            {-1, 1, 1 },
    },
        glm::vec4(0.0, 1.0, 0.0, 1.0), Line::Strip);

    Line line3(
        std::vector<glm::vec3> {
            {1,  0, 1 },
            {1,  0, -1},
            {-1, 0, -1},
            {-1, 0, 1 },
    },
        glm::vec4(0.0, 0.0, 1.0, 1.0), Line::LineLoop);

    AxisMark axm;
    axm.move({3, 0, 1});
    axm.setColor({1, 1, 0, 1});

    DiamondMark dm;
    dm.move({-2, 0, 1});
    dm.setColor({0, 1, 1, 1});

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

        auto vp = camera.projMatrix() * camera.viewMatrix();

        line1.draw(vp);
        line2.draw(vp);
        line3.draw(vp);
        axm.draw(vp);
        dm.draw(vp);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}