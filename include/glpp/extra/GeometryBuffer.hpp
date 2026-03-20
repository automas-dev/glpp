#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "glpp/FrameBuffer.hpp"
#include "glpp/Shader.hpp"
#include "glpp/Texture.hpp"
#include "glpp/extra/Vertex.hpp"

namespace glpp::extra {
    using glm::uvec2;
    using std::shared_ptr;

    class GeometryBuffer : public FrameBuffer {
    public:
        using Ptr = shared_ptr<GeometryBuffer>;
        using ConstPtr = const shared_ptr<GeometryBuffer>;

        Texture::Ptr diffuse;
        Texture::Ptr normal;
        Texture::Ptr position;
        Texture::Ptr specular;
        RenderBuffer::Ptr depth;

        // TODO: implement these as public, remember to move textures
        using FrameBuffer::FrameBuffer;
        using FrameBuffer::operator=;

        // GeometryBuffer should manage these
        using FrameBuffer::attach;

    public:
        GeometryBuffer(const uvec2 & size, GLsizei samples = 0);

        virtual ~GeometryBuffer();

        void bindTextures() const;

        static Shader & getShader();
    };
}
