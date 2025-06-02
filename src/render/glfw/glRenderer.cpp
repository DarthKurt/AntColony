#include "text/_fonts.hpp"
#include "text/font.hpp"

#include "glShaders.hpp"
#include "glRenderer.hpp"
#include "glShaderProvider.hpp"
#include "glTextRenderer.hpp"

#include "../../core/logger.hpp"
#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include <cmath>
#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace AntColony::Render::GLFW
{
    constexpr const int NUM_CIRCLE_SEGMENTS = 50;

    GLRenderer::GLRenderer(const std::shared_ptr<GLShaderProvider> shaderProvider, const std::shared_ptr<AntColony::Core::Logger> logger)
        : shaderProvider(shaderProvider),
          logger(logger),
          textRenderer(std::make_shared<GLTextRenderer>(shaderProvider, logger)),
          isInited(false) {}

    GLRenderer::~GLRenderer()
    {
        isInited = false;
        glDeleteBuffers(1, &circleVBO);
        glDeleteVertexArrays(1, &circleVAO);
        glDeleteProgram(figureShaderProgram);
        glDeleteProgram(textShaderProgram);
    }

    void GLRenderer::init()
    {
        figureShaderProgram = shaderProvider->createShaderProgram(VERTEXT_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
        if (figureShaderProgram == 0)
        {
            logger->error("Failed to create figure shader program");
            return;
        }

        textShaderProgram = shaderProvider->createShaderProgram(TEXT_VERTEX_SHADER_SOURCE, TEXT_FRAGMENT_SHADER_SOURCE);
        if (textShaderProgram == 0)
        {
            logger->error("Failed to create text shader program");
            glDeleteProgram(figureShaderProgram);
            return;
        }

        initCircleGeometry();
        textRenderer->init();
        isInited = true;

        // Log viewport
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        logger->debug("Viewport: " + std::to_string(viewport[0]) + ", " +
                      std::to_string(viewport[1]) + ", " +
                      std::to_string(viewport[2]) + ", " +
                      std::to_string(viewport[3]));

        // Set clear color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        logger->debug("GLRenderer initialized successfully");
    }

    void GLRenderer::initCircleGeometry()
    {
        std::vector<float> circleVertices;
        circleVertices.push_back(0.0f);
        circleVertices.push_back(0.0f);
        for (int i = 0; i <= NUM_CIRCLE_SEGMENTS; i++)
        {
            float angle = 2.0f * M_PI * i / NUM_CIRCLE_SEGMENTS;
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

    void GLRenderer::drawCircleInPosition(const Core::Point &position, const float radius, const Core::Color &color)
    {
        if (!isInited)
        {
            logger->error("Renderer not initialized, skipping drawCircleInPosition");
            return;
        }

        glUseProgram(figureShaderProgram);
        int winWidth, winHeight;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        auto ortho = glm::ortho(0.0f, (float)winWidth, (float)winHeight, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(figureShaderProgram, "uOrtho"), 1, GL_FALSE, &ortho[0][0]);
        glUniform3f(glGetUniformLocation(figureShaderProgram, "uColor"), color.r, color.g, color.b);

        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_CIRCLE_SEGMENTS + 2);
        glBindVertexArray(0);
    }

    void GLRenderer::drawFrame(const Core::Point &position, const float width, const float height, const Core::Color &color)
    {
        if (!isInited)
        {
            logger->error("Renderer not initialized, skipping drawFrame");
            return;
        }

        float halfW = width / 2.0f;
        float halfH = height / 2.0f;
        float vertices[8] = {
            position.x - halfW, position.y - halfH,
            position.x + halfW, position.y - halfH,
            position.x + halfW, position.y + halfH,
            position.x - halfW, position.y + halfH};

        GLuint vaoTmp, vboTmp;
        glGenVertexArrays(1, &vaoTmp);
        glGenBuffers(1, &vboTmp);
        glBindVertexArray(vaoTmp);
        glBindBuffer(GL_ARRAY_BUFFER, vboTmp);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glUseProgram(figureShaderProgram);
        int winWidth, winHeight;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        auto ortho = glm::ortho(0.0f, (float)winWidth, (float)winHeight, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(figureShaderProgram, "uOrtho"), 1, GL_FALSE, &ortho[0][0]);
        glUniform3f(glGetUniformLocation(figureShaderProgram, "uColor"), color.r, color.g, color.b);

        glBindVertexArray(vaoTmp);
        glDrawArrays(GL_LINE_LOOP, 0, 4);
        glBindVertexArray(0);

        glDeleteBuffers(1, &vboTmp);
        glDeleteVertexArrays(1, &vaoTmp);
    }

    void GLRenderer::drawText(const Core::Point &position, const std::string &text, const Core::Color &color, const float fontSize)
    {
        if (!isInited)
        {
            logger->error("Renderer not initialized, skipping drawText");
            return;
        }

        textRenderer->drawText(position, text, color, fontSize);
    }
}