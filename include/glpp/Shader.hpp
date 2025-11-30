#pragma once

#include <GL/glew.h>
// gl.h after glew.h, clang-format don't sort
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

// https://www.khronos.org/opengl/wiki/Shader_Compilation

namespace glpp {
    using std::shared_ptr;
    using std::string;
    using std::string_view;

    class ShaderCompileException : public std::runtime_error {
    public:
        ShaderCompileException(GLuint shader);
    };

    class ShaderLinkException : public std::runtime_error {
    public:
        ShaderLinkException(GLuint program);
    };

    /**
     * Open a source file at path and read it's content.
     *
     * @param path the path to the source file
     *
     * @return a string that contains the file's contents
     */
    string shaderSource(const string & path);

    class Uniform {
        GLuint location;

    public:
        Uniform(GLuint location = 0);

        GLuint getLocation() const;

        /**
         * Check if the uniform exists in a given shader.
         *
         * @return true
         * @return false
         */
        bool exists() const;

        /**
         * Set the value to integer 0 or 1.
         *
         * @param value the value to set
         */
        void setBool(bool value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setInt(int value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setUInt(unsigned int value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setFloat(float value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setVec2(const glm::vec2 & value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setVec3(const glm::vec3 & value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setVec4(const glm::vec4 & value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setMat2(const glm::mat2 & value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setMat3(const glm::mat3 & value) const;

        /**
         * Set the value.
         *
         * @param value the value to set
         */
        void setMat4(const glm::mat4 & value) const;
    };

    /**
     * Manages a single OpenGL shader.
     */
    class Shader {
    public:
        using Ptr = shared_ptr<Shader>;
        using ConstPtr = const shared_ptr<Shader>;

    private:
        GLuint program;

    public:
        /**
         * Create a new Shader with program from vertex and fragment source.
         *
         * @param vertexSource the vertex shader source
         * @param fragmentSource the fragment shader source
         *
         * @pre vertexSource and fragmentSource must be null terminated.
         */
        Shader(const string_view & vertexSource,
               const string_view & fragmentSource);

        Shader(Shader && other);

        Shader & operator=(Shader && other);

        Shader(const Shader &) = delete;
        Shader & operator=(const Shader &) = delete;

        /// Free OpenGL resources
        ~Shader();

        /**
         * Get the OpenGL shader program id.
         *
         * @return the shader program id
         */
        GLuint getProgram() const;

        /**
         * Bind the shader.
         */
        void bind() const;

        /**
         * Unbind the shader, effectively binding 0.
         */
        void unbind() const;

        /**
         * Get the uniform for name in this shader.
         *
         * @param name the uniform name
         *
         * @return the uniform
         */
        Uniform uniform(const char * name) const;

        /**
         * Load the default shader from the internal source.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * @deprecated A default shader will be provided in a different form, if
         * at all, in future releases. This shader will always outlive the
         * OpenGL context.
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * @return the default shader
         */
        static Shader & defaultShader();

        /**
         * Load a shader using the default vertex shader and a fragment shader.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * Out to fragment shader:
         * - FragPos: vec3
         * - FragNorm: vec3
         * - FragTex: vec2
         *
         * @param source the fragment shader source
         *
         * @pre source must be null terminated.
         *
         * @return the shader
         */
        static Shader fromFragmentSource(const string_view & source);

        /**
         * Load a shader using a vertex and fragment shader path.
         *
         * @param vertexPath the path to the vertex shader source
         * @param fragmentPath the path to the fragment shader source
         *
         * @return the shader
         */
        static Shader fromPaths(const string & vertexPath,
                                const string & fragmentPath);

        /**
         * Load a shader using the default vertex shader and a fragment shader.
         *
         * This shader takes a Vertex from VBO.hpp,
         *
         * Uniforms:
         * - mvp: mat4
         * - model: mat4
         *
         * Out to fragment shader:
         * - FragPos: vec3
         * - FragNorm: vec3
         * - FragTex: vec2
         *
         * @param path the path to the fragment shader source
         *
         * @return the shader
         */
        static Shader fromFragmentPath(const string & path);
    };
}
