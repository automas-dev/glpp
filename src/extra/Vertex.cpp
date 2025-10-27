#include "glpp/extra/Vertex.hpp"

namespace glpp::extra {
    Vertex::Vertex() : pos(0), norm(0), uv(0) {}

    Vertex::Vertex(const glm::vec3 & pos, const glm::vec3 & norm, const glm::vec2 & uv)
        : pos(pos), norm(norm), uv(uv) {}

    Vertex Vertex::operator+(const Vertex & other) const {
        return Vertex(pos + other.pos, norm + other.norm, uv + other.uv);
    }

    Vertex Vertex::operator+(const glm::vec3 & offset) const {
        return Vertex(pos + offset, norm, uv);
    }

    Vertex Vertex::operator-(const Vertex & other) const {
        return Vertex(pos - other.pos, norm - other.norm, uv - other.uv);
    }

    Vertex Vertex::operator-(const glm::vec3 & offset) const {
        return Vertex(pos - offset, norm, uv);
    }

    Vertex & Vertex::operator+=(const Vertex & other) {
        pos += other.pos;
        norm += other.norm;
        uv += other.uv;
        return *this;
    }

    Vertex & Vertex::operator-=(const Vertex & other) {
        pos -= other.pos;
        norm -= other.norm;
        uv -= other.uv;
        return *this;
    }
}

namespace glpp::extra {
    static const vector<vector<Buffer::Attribute>> attrs {
        {
         {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0},
         {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float))},
         {2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float))},
         }
    };

    VertexBufferArray::VertexBufferArray() : BufferArray(attrs) {}

    VertexBufferArray::VertexBufferArray(VertexBufferArray && other)
        : BufferArray(std::move(other)) {}

    VertexBufferArray & VertexBufferArray::operator=(VertexBufferArray && other) {
        BufferArray::operator=(std::move(other));
        return *this;
    }

    VertexBufferArray::~VertexBufferArray() {}

    void VertexBufferArray::bufferData(GLsizeiptr size,
                                       const Vertex * data,
                                       Usage usage) {
        BufferArray::bufferData(0, size, data, usage);
    }

    void VertexBufferArray::bufferSubData(GLintptr offset,
                                          GLsizeiptr size,
                                          const Vertex * data) {
        BufferArray::bufferSubData(0, offset, size, data);
    }
}
