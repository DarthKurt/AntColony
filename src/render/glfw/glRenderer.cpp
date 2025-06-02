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

        int winWidth, winHeight;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight);

        // Convert from normalized [-1,1] to pixel coordinates
        float pixel_x = (position.x + 1.0f) * 0.5f * winWidth;
        float pixel_y = (1.0f - position.y) * 0.5f * winHeight;

        // Scale radius consistently with NDC space
        // In NDC, the range is 2.0 (-1 to 1), so we multiply by the dimension/2
        float pixel_radius = radius * (std::min(winWidth, winHeight) / 2.0f);

        glUseProgram(figureShaderProgram);
        auto ortho = glm::ortho(0.0f, (float)winWidth, (float)winHeight, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(figureShaderProgram, "uOrtho"), 1, GL_FALSE, &ortho[0][0]);
        glUniform3f(glGetUniformLocation(figureShaderProgram, "uColor"), color.r, color.g, color.b);

        // Position and scale the circle
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(pixel_x, pixel_y, 0.0f));
        model = glm::scale(model, glm::vec3(pixel_radius, pixel_radius, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(figureShaderProgram, "uModel"), 1, GL_FALSE, &model[0][0]);

        glBindVertexArray(circleVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_CIRCLE_SEGMENTS + 2);
        glBindVertexArray(0);
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