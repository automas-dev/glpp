#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <cstddef>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace glpp {
    using std::shared_ptr;
    using std::vector;

    /**
     * A single array buffer.
     */
    class Buffer {
    public:
        using Ptr = shared_ptr<Buffer>;
        using ConstPtr = const shared_ptr<Buffer>;

        /**
         * Describes a vertex array attribute for a Buffer.
         */
        struct Attribute {
            GLuint index;
            GLint size;
            GLenum type;
            bool normalized;
            GLsizei stride;
            const void * pointer;
            GLuint divisor = 0;

            Attribute(GLuint index,
                      GLint size,
                      GLenum type,
                      bool normalized,
                      GLsizei stride,
                      const void * pointer = nullptr,
                      GLuint divisor = 0);

            Attribute(const Attribute & other) = default;

            Attribute(Attribute && other) = default;

            Attribute & operator=(const Attribute & other) = default;

            Attribute & operator=(Attribute && other) = default;

            void enable() const;

            void disable() const;

            bool isInstanced() const;
        };

        enum Target {
            Array = GL_ARRAY_BUFFER,
            Index = GL_ELEMENT_ARRAY_BUFFER,
            Uniform = GL_UNIFORM_BUFFER,
            ShaderStorage = GL_SHADER_STORAGE_BUFFER,
            TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
            PixelPack = GL_PIXEL_PACK_BUFFER,
            PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
        };

        /**
         * OpenGL buffer usage.
         */
        enum Usage {
            /// Data will be buffered once and used multiple times
            Static = GL_STATIC_DRAW,
            /// Data will be buffered and used a few times
            Stream = GL_STREAM_DRAW,
            /// Data will be buffered and used once
            Dynamic = GL_DYNAMIC_DRAW,
        };

        /**
         * OpenGL Draw mode.
         */
        enum Mode {
            Lines = GL_LINES,
            LineStrip = GL_LINE_STRIP,
            LineLoop = GL_LINE_LOOP,

            Triangles = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            TriangleFan = GL_TRIANGLE_FAN,
        };

    private:
        Target target;
        GLuint buffer;
        vector<Attribute> attrib;

    public:
        /**
         * Create a new VBO with empty attributes.
         *
         * @param target the target buffer
         *
         * After creating the vertex array attribute vaa will be enabled and the
         * new vbo will be bound.
         */
        Buffer(Target target = Array);

        /**
         * Create a new VBO with attributes.
         *
         * @param attrib the VBO attributes
         * @param target the target buffer
         *
         * After creating the vertex array attribute vaa will be enabled and the
         * new vbo will be bound.
         */
        Buffer(const vector<Attribute> & attrib, Target target = Array);

        Buffer(Buffer && other);

        Buffer & operator=(Buffer && other);

        Buffer(const Buffer &) = delete;
        Buffer & operator=(const Buffer &) = delete;

        virtual ~Buffer();

        Target getTarget() const;

        GLuint getBufferId() const;

        bool isInstanced() const;

        void attach() const;

        void bind() const;

        void unbind() const;

        void bufferData(GLsizeiptr size, const void * data, Usage usage = Static);

        void bufferSubData(GLintptr offset, GLsizeiptr size, const void * data);
    };

    class BufferArray {
    public:
        using Ptr = shared_ptr<BufferArray>;
        using ConstPtr = const shared_ptr<BufferArray>;

    private:
        GLuint array;
        vector<Buffer::Ptr> buffers;
        Buffer::Ptr elementBuffer;

    public:
        using Usage = Buffer::Usage;
        using Mode = Buffer::Mode;

        BufferArray();

        BufferArray(const vector<vector<Buffer::Attribute>> & attributes);

        BufferArray(vector<Buffer::Ptr> && buffers);

        BufferArray(BufferArray && other);

        BufferArray & operator=(BufferArray && other);

        BufferArray(const BufferArray &) = delete;
        BufferArray & operator=(const BufferArray &) = delete;

        virtual ~BufferArray();

        GLuint getArrayId() const;

        /**
         * The number of buffers not including the optional index buffer.
         */
        std::size_t size() const;

        const vector<Buffer::Ptr> & getBuffers() const;

        vector<Buffer::Ptr> & getBuffers();

        bool isInstanced() const;

        void bind() const;

        static void unbind();

        inline void bufferData(size_t index,
                               GLsizeiptr size,
                               const void * data,
                               Usage usage = Usage::Static) {
            buffers[index]->bufferData(size, data, usage);
        }

        inline void bufferSubData(size_t index,
                                  GLintptr offset,
                                  GLsizeiptr size,
                                  const void * data) {
            buffers[index]->bufferSubData(offset, size, data);
        }

        void bufferElements(GLsizeiptr size,
                            const void * data,
                            Usage usage = Usage::Static);

        void drawArrays(Mode mode, GLint first, GLsizei count) const;


        void drawArraysInstanced(Mode mode,
                                 GLint first,
                                 GLsizei count,
                                 GLsizei primcount) const;

        void drawElements(Mode mode,
                          GLsizei count,
                          GLenum type,
                          const void * indices) const;


        void drawElementsInstanced(Mode mode,
                                   GLsizei count,
                                   GLenum type,
                                   const void * indices,
                                   GLsizei primcount) const;
    };
}
