#include "glpp/FrameBuffer.hpp"

#include <stdexcept>

namespace glpp {
    RenderBuffer::RenderBuffer(const glm::uvec2 & size, GLenum internal, GLsizei samples)
        : internal(internal), size(size), samples(samples) {
        glGenRenderbuffers(1, &buffer);
        resize(size);
    }

    RenderBuffer::RenderBuffer(RenderBuffer && other)
        : buffer(other.buffer), internal(other.internal), size(other.size) {
        other.buffer = 0;
    }

    RenderBuffer & RenderBuffer::operator=(RenderBuffer && other) {
        buffer = other.buffer;
        other.buffer = 0;
        internal = other.internal;
        size = other.size;
        return *this;
    }

    RenderBuffer::~RenderBuffer() {
        if (buffer)
            glDeleteRenderbuffers(1, &buffer);
    }

    GLuint RenderBuffer::getBufferId() const {
        return buffer;
    }

    GLsizei RenderBuffer::getSamples() const {
        return samples;
    }

    const glm::uvec2 & RenderBuffer::getSize() const {
        return size;
    }

    void RenderBuffer::resize(const glm::uvec2 & size) {
        this->size = size;
        bind();
        if (samples > 0)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                             samples,
                                             internal,
                                             size.x,
                                             size.y);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, internal, size.x, size.y);
    }

    void RenderBuffer::bind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
    }

    void RenderBuffer::unbind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

namespace glpp {
    FrameBuffer::Attachment::Attachment(const Texture::Ptr & texture, GLenum attachment)
        : texture(texture), type(TEXTURE), attachment(attachment) {}

    FrameBuffer::Attachment::Attachment(const RenderBuffer::Ptr & buffer,
                                        GLenum attachment)
        : buffer(buffer), type(RENDER_BUFFER), attachment(attachment) {}

    void FrameBuffer::Attachment::resize(const glm::uvec2 & size) {
        if (type == TEXTURE)
            texture->resize(size);
        else
            buffer->resize(size);
    }
}

namespace glpp {
    FrameBuffer::FrameBuffer(const glm::uvec2 & size) : size(size) {
        glGenFramebuffers(1, &buffer);
        bind();
    }

    FrameBuffer::FrameBuffer(FrameBuffer && other)
        : buffer(other.buffer),
          attachments(std::move(other.attachments)),
          size(other.size) {
        other.buffer = 0;
    }

    FrameBuffer & FrameBuffer::operator=(FrameBuffer && other) {
        buffer = other.buffer;
        other.buffer = 0;
        attachments = std::move(other.attachments);
        size = other.size;
        return *this;
    }

    FrameBuffer::~FrameBuffer() {
        if (buffer)
            glDeleteFramebuffers(1, &buffer);
    }

    bool FrameBuffer::isComplete() const {
        bind();
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    GLuint FrameBuffer::getBufferId() const {
        return buffer;
    }

    void FrameBuffer::attach(const Texture::Ptr & texture, GLenum attachment) {
        if (texture->getSize() != size)
            texture->resize(size);

        attachments.emplace_back(texture, attachment);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               attachment,
                               texture->getTarget(),
                               texture->getTextureId(),
                               0);
    }

    void FrameBuffer::attach(const RenderBuffer::Ptr & buffer, GLenum attachment) {
        if (buffer->getSize() != size)
            buffer->resize(size);

        attachments.emplace_back(buffer, attachment);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  attachment,
                                  GL_RENDERBUFFER,
                                  buffer->getBufferId());

        vector<GLenum> attrs;
        for (auto & att : attachments) {
            if (att.attachment == GL_DEPTH_STENCIL_ATTACHMENT)
                continue;
            attrs.push_back(att.attachment);
        }
        glDrawBuffers(attrs.size(), attrs.data());
    }

    const vector<FrameBuffer::Attachment> & FrameBuffer::getAttachments() const {
        return attachments;
    }

    const glm::uvec2 & FrameBuffer::getSize() const {
        return size;
    }

    void FrameBuffer::resize(const glm::uvec2 & size) {
        this->size = size;
        for (auto & att : attachments) {
            att.resize(size);
        }
    }

    void FrameBuffer::blit(const FrameBuffer & source,
                           GLbitfield mask,
                           GLenum filter) const {
        blit(source,                             //
             0, 0, source.size.x, source.size.y, //
             0, 0, size.x, size.y,               //
             mask, filter);
    }

    void FrameBuffer::blit(const FrameBuffer & source,
                           GLint sx0,
                           GLint sy0,
                           GLint sx1,
                           GLint sy1,
                           GLint dx0,
                           GLint dy0,
                           GLint dx1,
                           GLint dy1,
                           GLbitfield mask,
                           GLenum filter) const {
        source.bind(GL_READ_FRAMEBUFFER);
        bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(sx0, sy0, sx1, sy1, //
                          dx0, dy0, dx1, dy1, //
                          mask, filter);
    }

    void FrameBuffer::bind(GLenum target) const {
        glBindFramebuffer(target, buffer);
    }

    void FrameBuffer::setViewport() const {
        glViewport(0, 0, size.x, size.y);
    }

    void FrameBuffer::unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::clear(GLbitfield mask) {
        glClear(mask);
    }

    FrameBuffer & FrameBuffer::getDefault() {
        static FrameBuffer buffer(0);
        return buffer;
    }
}
