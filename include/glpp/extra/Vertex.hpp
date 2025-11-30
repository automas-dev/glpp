#pragma once

#include <memory>
#include <vector>

#include "glpp/Buffer.hpp"

namespace glpp::extra {
    using std::shared_ptr;
    using std::vector;

    /**
     * A single point in the format accepted by VBO, Mesh and Model.
     */
    struct Vertex {
        /// The position
        glm::vec3 pos;
        /// The normal
        glm::vec3 norm;
        /// The UV texture coordinate
        glm::vec2 uv;

        /**
         * Create a new vertex with all fields set to 0.
         */
        Vertex();

        /**
         * Create a new Vertex and set it fields to pos, norm and uv.
         *
         * @param pos the position
         * @param norm the normal
         * @param uv the texture coordinate
         */
        Vertex(const glm::vec3 & pos, const glm::vec3 & norm, const glm::vec2 & uv);

        /// Default copy constructor
        Vertex(const Vertex & other) = default;

        /// Default move constructor
        Vertex(Vertex && other) = default;

        /// Default copy assign operator
        Vertex & operator=(const Vertex & other) = default;

        /// Default move assign operator
        Vertex & operator=(Vertex && other) = default;

        /**
         * Add the fields element wise of two Vertex objects.
         *
         * @param other the Vertex to add to
         *
         * @return a new Vertex with the sum of both objects.
         */
        Vertex operator+(const Vertex & other) const;

        /**
         * Add offset to just the position field.
         *
         * @param offset the offset to add to pos
         *
         * @return a new Vertex with the sum of pos and offset
         */
        Vertex operator+(const glm::vec3 & offset) const;

        /**
         * Subtract the fields element wise of two Vertex objects.
         *
         * @param other the Vertex to subtract from this
         *
         * @return a new Vertex with the difference of both objects.
         */
        Vertex operator-(const Vertex & other) const;

        /**
         * Subtract offset from just the position field.
         *
         * @param offset the offset to add to pos
         *
         * @return a new Vertex with the difference of pos and offset
         */
        Vertex operator-(const glm::vec3 & offset) const;

        /**
         * Add the fields element wise of other to the fields of this.
         *
         * @param other the Vertex to add to this
         *
         * @return a reference to this
         */
        Vertex & operator+=(const Vertex & other);

        /**
         * Subtract the fields element wise of other from the fields of this.
         *
         * @param other the Vertex to subtract from this
         *
         * @return a reference to this
         */
        Vertex & operator-=(const Vertex & other);
    };

    /**
     * Derived from BufferArray for use with the Vertex type.
     */
    class VertexBufferArray : public BufferArray {
    public:
        using Ptr = shared_ptr<VertexBufferArray>;
        using ConstPtr = const shared_ptr<VertexBufferArray>;

    private:
        using BufferArray::bufferData;
        using BufferArray::bufferSubData;

    public:
        /**
         * Create an empty VertexBufferArray.
         */
        VertexBufferArray();

        VertexBufferArray(VertexBufferArray && other);

        VertexBufferArray & operator=(VertexBufferArray && other);

        VertexBufferArray(const VertexBufferArray &) = delete;
        VertexBufferArray & operator=(const VertexBufferArray &) = delete;

        ~VertexBufferArray();

        /**
         * Send a vector of Vertex to the buffer.
         *
         * @param data the buffer data
         * @param usage the buffer usage hint
         *
         * @see BufferArray::bufferData
         */
        inline void bufferData(const vector<Vertex> & data,
                               Usage usage = Usage::Static) {
            bufferData(data.size() * sizeof(Vertex), data.data(), usage);
        }

        /**
         * Send Vertex to the buffer from pointer and size.
         *
         * @param size the number of elements in data
         * @param data the buffer data
         * @param usage the buffer usage hint
         *
         * @see BufferArray::bufferData
         */
        void bufferData(GLsizeiptr size,
                        const Vertex * data,
                        Usage usage = Usage::Static);

        /**
         * Replace a subset of the buffer with new data.
         *
         * @param offset the start index
         * @param size the number of elements to replace
         * @param data the buffer data
         */
        void bufferSubData(GLintptr offset, GLsizeiptr size, const Vertex * data);
    };
}
