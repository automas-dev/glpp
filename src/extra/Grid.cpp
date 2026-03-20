#include "glpp/extra/Grid.hpp"

#include <vector>

static const char * vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aCol;
uniform mat4 mvp;
out vec4 color;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    color = aCol;
})";

static const char * fragmentShaderSource = R"(
#version 330 core
in vec4 color;
out vec4 FragColor;
void main() {
    FragColor = color;
})";

namespace glpp::extra {
    using std::make_shared;
    using std::vector;

    void Grid::updateBuffer() {
        vector<glm::vec3> vertices;
        vector<glm::vec4> colors;

        for (int i = -size; i <= size; i++) {
            if (i == 0)
                continue;

            // X axis
            vertices.emplace_back(size, 0, i);
            vertices.emplace_back(-size, 0, i);
            colors.push_back(color);
            colors.push_back(color);

            // Z axis
            vertices.emplace_back(i, 0, size);
            vertices.emplace_back(i, 0, -size);
            colors.push_back(color);
            colors.push_back(color);
        }

        // X axis center
        vertices.emplace_back(-size, 0, 0);
        vertices.emplace_back(0, 0, 0);
        colors.push_back(color);
        colors.push_back(color);

        vertices.emplace_back(0, 0, 0);
        vertices.emplace_back(size, 0, 0);

        if (colorAxis) {
            colors.push_back({1, 0, 0, color.a});
            colors.push_back({1, 0, 0, color.a});
        }
        else {
            colors.push_back(color);
            colors.push_back(color);
        }

        // Y axis center
        vertices.emplace_back(0, -size, 0);
        vertices.emplace_back(0, 0, 0);
        colors.push_back(color);
        colors.push_back(color);

        vertices.emplace_back(0, 0, 0);
        vertices.emplace_back(0, size, 0);

        if (colorAxis) {
            colors.push_back({0, 1, 0, color.a});
            colors.push_back({0, 1, 0, color.a});
        }
        else {
            colors.push_back(color);
            colors.push_back(color);
        }

        // Z axis center
        vertices.emplace_back(0, 0, -size);
        vertices.emplace_back(0, 0, 0);
        colors.push_back(color);
        colors.push_back(color);

        vertices.emplace_back(0, 0, 0);
        vertices.emplace_back(0, 0, size);

        if (colorAxis) {
            colors.push_back({0, 0, 1, color.a});
            colors.push_back({0, 0, 1, color.a});
        }
        else {
            colors.push_back(color);
            colors.push_back(color);
        }

        n = vertices.size();

        array->bind();
        array->bufferData(0, n * sizeof(glm::vec3), vertices.data());
        array->bufferData(1, n * sizeof(glm::vec4), colors.data());
        array->unbind();
    }

    Grid::Grid(int size, const glm::vec4 & color, bool colorAxis)
        : n(0),
          size(size),
          color(color),
          colorAxis(colorAxis),
          array(make_shared<BufferArray>(vector<vector<Buffer::Attribute>> {
              {Buffer::Attribute {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0}},
              {Buffer::Attribute {1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0}},
          })) {
        updateBuffer();
    }

    Grid::~Grid() {}

    Grid::Grid(Grid && other)
        : Grid(other.size, other.color, other.colorAxis) {}

    Grid & Grid::operator=(Grid && other) {
        size = other.size;
        color = other.color;
        colorAxis = other.colorAxis;
        updateBuffer();
        return *this;
    }

    int Grid::getSize() const {
        return size;
    }

    void Grid::setSize(int size) {
        this->size = size;
        updateBuffer();
    }

    const glm::vec4 & Grid::getColor() const {
        return color;
    }

    void Grid::setColor(const glm::vec4 & color) {
        this->color = color;
        updateBuffer();
    }

    void Grid::draw() const {
        // drawArrays calls bind
        array->drawArrays(Buffer::Lines, 0, n);
    }

    void Grid::draw(const glm::mat4 & transform) const {
        shader().bind();
        shader().uniform("mvp").setMat4(transform);
        draw();
    }

    Shader & Grid::shader() {
        static Shader shader(vertexShaderSource, fragmentShaderSource);
        return shader;
    }
}
