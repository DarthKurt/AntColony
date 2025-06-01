#include "glShaderProvider.hpp"

#include "../../core/logger.hpp"
#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include <iostream>


namespace AntColony::Render::GLFW
{
    constexpr const GLuint WRONG_SHADER = 0;
    constexpr const int LOG_SIZE = 512;

    GLShaderProvider::GLShaderProvider(const std::shared_ptr<Core::Logger> logger) : logger(logger) {}

    GLuint GLShaderProvider::compileShader(GLenum type, const char *src) const
    {
        GLuint shader = glCreateShader(type);

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[LOG_SIZE];
            glGetShaderInfoLog(shader, LOG_SIZE, nullptr, infoLog);
            logger->error("Shader compilation error:\n" + std::string(infoLog));
        }

        logger->debug("Shader compilation finished");

        return shader;
    }

    GLuint GLShaderProvider::createShaderProgram(const char *vertexSrc, const char *fragmentSrc) const
    {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);

        if (vertexShader == WRONG_SHADER)
        {
            return WRONG_SHADER;
        }

        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

        if (fragmentShader == WRONG_SHADER)
        {
            glDeleteShader(vertexShader);
            return WRONG_SHADER;
        }

        GLuint program = glCreateProgram();

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        int success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            char infoLog[LOG_SIZE];
            glGetProgramInfoLog(program, LOG_SIZE, nullptr, infoLog);
            logger->error("Shader program linking error:\n" + std::string(infoLog));
            glDeleteProgram(program);
            return WRONG_SHADER;
        }

        logger->debug("Shader program compilation finished");

        return program;
    }
}