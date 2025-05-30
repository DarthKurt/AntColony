#include "glRenderer.hpp"
#include <cmath>
#include <vector>
#include <iostream>

namespace AntColony::Render
{
    // Vertex shader source used for all drawing
    const char *vertexShaderSrc = R"(#version 330 core
layout(location = 0) in vec2 position;
uniform vec2 uCenter;
uniform float uScale;
void main() {
    gl_Position = vec4((position * uScale) + uCenter, 0.0, 1.0);
}
)";

    // Fragment shader source used for all drawing
    const char *fragmentShaderSrc = R"(#version 330 core
out vec4 fragColor;
uniform vec3 uColor;
void main() {
    fragColor = vec4(uColor, 1.0);
}
)";

    GLRenderer::GLRenderer() : isInited(false) {}

    GLRenderer::~GLRenderer()
    {
        isInited = false;
        glDeleteBuffers(1, &circleVBO);
        glDeleteVertexArrays(1, &circleVAO);
        glDeleteProgram(shaderProgram);
    }

    void GLRenderer::init()
    {
        shaderProgram = createShaderProgram();
        initCircleGeometry();
        isInited = true;
    }

    GLuint GLRenderer::compileShader(GLenum type, const char *src) const
    {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Shader compilation error:\n"
                      << infoLog << std::endl;
        }
        return shader;
    }

    GLuint GLRenderer::createShaderProgram() const
    {
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return program;
    }

    // Precomputes the circle geometry using a triangle fan
    void GLRenderer::initCircleGeometry()
    {
        std::vector<float> circleVertices;
        // First vertex is the center
        circleVertices.push_back(0.0f);
        circleVertices.push_back(0.0f);
        for (int i = 0; i <= numCircleSegments; i++)
        {
            float angle = 2.0f * 3.14159f * i / numCircleSegments;
            circleVertices.push_back(cos(angle));
            circleVertices.push_back(sin(angle));
        }

        glGenVertexArrays(1, &circleVAO);
        glGenBuffers(1, &circleVBO);
        glBindVertexArray(circleVAO);
        glBindBuffer(GL_ARRAY_BUFFER, circleVBO);
        glBufferData(GL_ARRAY_BUFFER, circleVertices.size() * sizeof(float), circleVertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // Draw a circle at the given position with the specified radius and color
    void GLRenderer::drawCircleInPosition(const Core::Point &position, const float radius, const Core::Color &color) const
    {
        if (!isInited)
            return;

        glUseProgram(shaderProgram);
        GLint centerLoc = glGetUniformLocation(shaderProgram, "uCenter");
        GLint scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        glUniform2f(centerLoc, position.x, position.y);
        glUniform1f(scaleLoc, radius);
        glUniform3f(colorLoc, color.r, color.g, color.b);

        glBindVertexArray(circleVAO);
        // 1 center + numCircleSegments + 1 vertices
        glDrawArrays(GL_TRIANGLE_FAN, 0, numCircleSegments + 2);
        glBindVertexArray(0);
    }

    // Draw a rectangular frame using on\-the\-fly VBO and VAO creation
    void GLRenderer::drawFrame(const Core::Point &position, const float width, const float height, const Core::Color &color) const
    {
        if (!isInited)
            return;

        // Calculate corners of the rectangle
        float halfW = width / 2.0f;
        float halfH = height / 2.0f;
        float vertices[8] = {
            position.x - halfW, position.y - halfH,
            position.x + halfW, position.y - halfH,
            position.x + halfW, position.y + halfH,
            position.x - halfW, position.y + halfH};

        // On\-the\-fly VAO/VBO
        GLuint vaoTmp, vboTmp;
        glGenVertexArrays(1, &vaoTmp);
        glGenBuffers(1, &vboTmp);
        glBindVertexArray(vaoTmp);
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glUseProgram(shaderProgram);
        GLint centerLoc = glGetUniformLocation(shaderProgram, "uCenter");
        GLint scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        // For frame drawing, no scaling or translation is applied in the shader.
        glUniform2f(centerLoc, 0.0f, 0.0f);
        glUniform1f(scaleLoc, 1.0f);
        glUniform3f(colorLoc, color.r, color.g, color.b);

        // Draw border using line loop
        glBindVertexArray(vaoTmp);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);

        glDeleteBuffers(1, &vboTmp);
        glDeleteVertexArrays(1, &vaoTmp);
    }

    // Basic text drawing implemented as a placeholder (draws a simple horizontal line)
    void GLRenderer::drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const
    {
        if (!isInited)
            return;

        // Compute width as fraction of text length (placeholder)
        float charWidth = 0.02f;
        float textWidth = text.size() * charWidth;
        float halfWidth = textWidth / 2.0f;

        float vertices[4] = {
            position.x - halfWidth, position.y,
            position.x + halfWidth, position.y};

        // On\-the\-fly VAO/VBO for a line
        GLuint vaoTmp, vboTmp;
        glGenVertexArrays(1, &vaoTmp);
        glGenBuffers(1, &vboTmp);
        glBindVertexArray(vaoTmp);
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glUseProgram(shaderProgram);
        GLint centerLoc = glGetUniformLocation(shaderProgram, "uCenter");
        GLint scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        glUniform2f(centerLoc, 0.0f, 0.0f);
        glUniform1f(scaleLoc, 1.0f);
        glUniform3f(colorLoc, color.r, color.g, color.b);

        glBindVertexArray(vaoTmp);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        glDeleteBuffers(1, &vboTmp);
        glDeleteVertexArrays(1, &vaoTmp);
    }
}