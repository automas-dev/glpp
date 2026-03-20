#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace glpp {
    using std::shared_ptr;
    using std::string;

    class TextureLoadException : public std::runtime_error {
    public:
        using std::runtime_error::runtime_error;
    };

    /**
     * Manages a single OpenGL texture.
     */
    class Texture {
    public:
        using Ptr = shared_ptr<Texture>;
        using ConstPtr = const shared_ptr<Texture>;

        enum Format {
            Gray = GL_RED,
            RGB = GL_RGB,
            RGBA = GL_RGBA,
        };

        /**
         * Mag filter only accepts Nearest or Linear.
         */
        enum Filter {
            // Min and Mag filter
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,

            // Min filter only
            NearestMmNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMmNearest = GL_LINEAR_MIPMAP_NEAREST,
            NearestMmLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMmLinear = GL_LINEAR_MIPMAP_LINEAR,
        };

        enum Wrap {
            Clamp = GL_CLAMP_TO_EDGE,
            Border = GL_CLAMP_TO_BORDER,
            MirrorRepeat = GL_MIRRORED_REPEAT,
            Repeat = GL_REPEAT,
            MirrorClamp = GL_MIRROR_CLAMP_TO_EDGE
        };

    private:
        GLuint textureId;
        glm::uvec2 size;
        Format internal;
        Format format;
        GLenum type;
        GLsizei samples;
        GLenum target;

        Filter magFilter, minFilter;
        Wrap wrap;
        bool mipmaps;

    public:
        /**
         * Create a texture from an image.
         *
         * Throw TextureLoadException if nrComponents is unsupported. Only 1, 3
         * and 4 are supported.
         *
         * @param data the pixel data
         * @param size the image dimensions in pixesl
         * @param nrComponents the number of components for each pixel
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param mipmaps should mipmaps be generated
         *
         * @throws TextureLoadException for unsupported nrComponents
         */
        Texture(const unsigned char * data,
                const glm::uvec2 & size,
                size_t nrComponents,
                Filter magFilter = Linear,
                Filter minFilter = LinearMmLinear,
                Wrap wrap = Repeat,
                bool mipmaps = true);

        /**
         * Create an empty texture of the specified size.
         *
         * Note: if samples is > 0, the filter, wrap and mipmaps arguments are
         * ignored.
         *
         * @param size the image size in pixels
         * @param internal the internal format like GL_RGBA16F, GL_RGBA,
         *                 GL_DEPTH_COMPONENT, etc.
         * @param format the format of pixel data
         * @param type the data type of pixel data
         * @param samples the number of samples to use, 0 to disable
         *                multisampling
         * @param magFilter the magnification filter (default GL_NEAREST)
         * @param minFilter the minification filter (default GL_NEAREST)
         * @param wrap the wrap mode when drawing
         * @param mipmaps should mipmaps be generated
         */
        Texture(const glm::uvec2 & size,
                Format internal = RGBA,
                Format format = RGBA,
                GLenum type = GL_FLOAT,
                GLsizei samples = 0,
                Filter magFilter = Linear,
                Filter minFilter = LinearMmLinear,
                Wrap wrap = Repeat,
                bool mipmaps = true);

        /// Free all opengl resources
        virtual ~Texture();

        Texture(Texture && other);

        Texture & operator=(Texture && other);

        Texture(const Texture &) = delete;
        Texture & operator=(const Texture &) = delete;

        /**
         * Load the texture from an image, setting the size to match
         * image.getSize().
         *
         * Throw TextureLoadException if nrComponents is unsupported. Only 1, 3
         * and 4 are supported.
         *
         * @param data the pixel data
         * @param size the image dimensions in pixesl
         * @param nrComponents the number of components for each pixel
         *
         * @throws TextureLoadException for unsupported nrComponents
         */
        void loadFrom(const unsigned char * data,
                      const glm::uvec2 & size,
                      size_t nrComponents);

        /**
         * Get the OpenGL texture id.
         *
         * @return the texture id
         */
        GLuint getTextureId() const;

        /**
         * Get the number of samples. Any value > 0 is multi-samples.
         *
         * @return the samples
         */
        GLsizei getSamples() const;

        /**
         * Get the OpenGL texture taget.
         *
         * @return the texture target
         */
        GLenum getTarget() const;

        /**
         * Get the texture size
         *
         * @return the texture size
         */
        const glm::uvec2 & getSize() const;

        /**
         * Resize the texture clearing all data.
         *
         * @param size the new texture size
         */
        void resize(const glm::uvec2 & size);

        /**
         * Bind the texture.
         */
        void bind(int index = 0) const;

        /**
         * Unbind the texture, effectively binding 0.
         */
        void unbind() const;

        /**
         * Load the texture from a file, setting the size from the image.
         *
         * Throw TextureLoadException if the image has an unsupported number of
         * components. Only 1, 3 and 4 are supported.
         *
         * @param path the path to the image file
         *
         * @return the Texture object
         *
         * @throws TextureLoadException if the image han an unsupported number
         * of components
         */
        static Texture fromPath(const string & path);
    };
}
