#pragma once

#include "_fwd.hpp"
#include "../../core/_fwd.hpp"

#include <memory>
#include "_gl.hpp"

namespace AntColony::Render::GLFW
{
    /**
     * @brief Manages creation and compilation of OpenGL shader programs.
     */
    class GLShaderProvider
    {
    public:
        /**
         * @brief Constructor with logger dependency.
         * @param logger Shared pointer to the logger instance.
         */
        explicit GLShaderProvider(const std::shared_ptr<Core::Logger> logger);

        /**
         * @brief Default destructor.
         */
        ~GLShaderProvider() = default;

        /**
         * @brief Creates and links a shader program from vertex and fragment shader sources.
         * @param vertexSrc Source code for the vertex shader.
         * @param fragmentSrc Source code for the fragment shader.
         * @return GLuint ID of the linked shader program, or 0 if creation fails.
         */
        GLuint createShaderProgram(const char *vertexSrc, const char *fragmentSrc) const;

    private:
        /// @brief Shared pointer to the logger for error reporting.
        const std::shared_ptr<Core::Logger> logger;

        /**
         * @brief Compiles a single shader of the specified type.
         * @param type Shader type (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
         * @param src Shader source code.
         * @return GLuint ID of the compiled shader, or 0 if compilation fails.
         */
        GLuint compileShader(GLenum type, const char *src) const;
    };
}