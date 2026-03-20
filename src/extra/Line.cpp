#include "glpp/extra/Line.hpp"

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

#define ARRAY                                                                 \
    make_shared<BufferArray>(vector<vector<Buffer::Attribute>> {              \
        {Buffer::Attribute {0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0}}, \
        {Buffer::Attribute {1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0}}, \
    })

namespace glpp::extra {
    using std::make_shared;
    using std::vector;

    void Line::updateBuffer() {
        vector<glm::vec3> vertices = points;
        vector<glm::vec4> colors;

        // Ignore last point if there are an odd number of points
        if (mode == Lines && vertices.size() % 2 == 1) {
            vertices.pop_back();
        }

        for (int i = 0; i < vertices.size(); i++) {
            colors.push_back(color);
        }

        n = vertices.size();

        array->bind();
        array->bufferData(0, n * sizeof(glm::vec3), vertices.data());
        array->bufferData(1, n * sizeof(glm::vec4), colors.data());
        array->unbind();
    }

    Line::Line(const glm::vec4 & color, Mode mode) : Line({}, color, mode) {}

    Line::Line(const std::vector<glm::vec3> & points,
               const glm::vec4 & color,
               Mode mode)
        : n(0), mode(mode), color(color), points(points), array(ARRAY) {
        updateBuffer();
    }

    Line::Line(std::vector<glm::vec3> && points, const glm::vec4 & color, Mode mode)
        : n(0), mode(mode), color(color), points(std::move(points)), array(ARRAY) {
        updateBuffer();
    }

    Line::Line(const glm::vec3 & p1,
               const glm::vec3 & p2,
               const glm::vec4 & color,
               Mode mode)
        : Line({p1, p2}, color, mode) {}

    Line::~Line() {}

    Line::Line(Line && other)
        : array(std::move(other.array)),
          n(other.n),
          mode(other.mode),
          color(other.color),
          points(std::move(other.points)) {}

    Line & Line::operator=(Line && other) {
        array = std::move(other.array);
        n = other.n;
        mode = other.mode;
        color = other.color;
        points = std::move(other.points);
        return *this;
    }

    size_t Line::size() const {
        return points.size();
    }

    const glm::vec3 & Line::getPoint(size_t i) const {
        return points.at(i);
    }

    void Line::setPoint(size_t i, const glm::vec3 & point) {
        points.at(i) = point;
    }

    const std::vector<glm::vec3> & Line::getPoints() const {
        return points;
    }

    void Line::setPoints(const std::vector<glm::vec3> & points) {
        this->points = points;
        updateBuffer();
    }

    void Line::setPoints(std::vector<glm::vec3> && points) {
        this->points = std::move(points);
        updateBuffer();
    }

    Line::Mode Line::getMode() const {
        return mode;
    }

    void Line::setMode(Mode mode) {
        this->mode = mode;
    }

    const glm::vec4 & Line::getColor() const {
        return color;
    }

    void Line::setColor(const glm::vec4 & color) {
        this->color = color;
        updateBuffer();
    }

    void Line::draw() const {
        // drawArrays calls bind
        array->drawArrays((Buffer::Mode)mode, 0, n);
    }

    void Line::draw(const glm::mat4 & transform) const {
        shader().bind();
        shader().uniform("mvp").setMat4(transform);
        draw();
    }

    Shader & Line::shader() {
        static Shader shader(vertexShaderSource, fragmentShaderSource);
        return shader;
    }
}
