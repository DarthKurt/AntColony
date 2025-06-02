#pragma once
#include "_fwd.hpp"
#include "text/_fwd.hpp"
#include "../../core/_fwd.hpp"

#include "../renderer.hpp"

#include <memory>
#include <string>
#include "_gl.hpp"

namespace AntColony::Render::GLFW
{
    /**
     * @brief OpenGL-based renderer for drawing shapes and text.
     */
    class GLRenderer : public Renderer
    {
    public:
        /**
         * @brief Constructor initializes renderer with a shader provider and logger.
         * @param shaderProvider Shared pointer to the shader provider instance.
         * @param logger Shared pointer to the logger instance.
         */
        explicit GLRenderer(const std::shared_ptr<GLShaderProvider> shaderProvider, const std::shared_ptr<AntColony::Core::Logger> logger);

        /**
         * @brief Destructor cleans up OpenGL resources.
         */
        ~GLRenderer() override;

        /**
         * @brief Initializes shaders and geometry for rendering.
         */
        void init();

        /**
         * @brief Draws a circle at the specified position with given radius and color.
         * @param position Center point of the circle.
         * @param radius Radius of the circle.
         * @param color RGB color of the circle.
         */
        void drawCircleInPosition(const Core::Point &position, const float radius, const Core::Color &color) override;

        /**
         * @brief Draws a rectangular frame at the specified position with given dimensions and color.
         * @param position Center point of the frame.
         * @param width Width of the frame.
         * @param height Height of the frame.
         * @param color RGB color of the frame.
         */
        void drawFrame(const Core::Point &position, const float width, const float height, const Core::Color &color) override;

        /**
         * @brief Draws text at the specified position with given color.
         * @param position Top-left position for text rendering.
         * @param text String to render.
         * @param color RGB color of the text.
         * @param fontSize Size of the font.
         */
        void drawText(const Core::Point &position, const std::string &text, const Core::Color &color, const float fontSize) override;

    private:
        /// @brief Shared pointer to the shader provider.
        const std::shared_ptr<GLShaderProvider> shaderProvider;
        /// @brief Shared pointer to the logger for error reporting.
        const std::shared_ptr<AntColony::Core::Logger> logger;
        /// @brief Shared pointer to the text renderer.
        const std::shared_ptr<GLTextRenderer> textRenderer;
        /// @brief Shader program for rendering shapes (circles, frames).
        GLuint figureShaderProgram;
        /// @brief Shader program for rendering text.
        GLuint textShaderProgram;
        /// @brief Vertex Array Object for circle geometry.
        GLuint circleVAO;
        /// @brief Vertex Buffer Object for circle geometry.
        GLuint circleVBO;
        /// @brief Tracks initialization state.
        bool isInited;

        /**
         * @brief Precomputes circle geometry using a triangle fan.
         */
        void initCircleGeometry();
    };
}