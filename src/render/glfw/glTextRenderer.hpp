#pragma once
#include "_fwd.hpp"
#include "text/_fwd.hpp"
#include "../../core/_fwd.hpp"

#include <memory>
#include <string>
#include "_gl.hpp"

namespace AntColony::Render::GLFW
{
    /**
     * @brief OpenGL-based renderer for drawing shapes and text.
     */
    class GLTextRenderer
    {
    public:
        /**
         * @brief Constructor initializes renderer with a shader provider and logger.
         * @param shaderProvider Shared pointer to the shader provider instance.
         * @param logger Shared pointer to the logger instance.
         */
        explicit GLTextRenderer(std::shared_ptr<GLShaderProvider> shaderProvider, std::shared_ptr<AntColony::Core::Logger> logger);

        /**
         * @brief Destructor cleans up OpenGL resources.
         */
        ~GLTextRenderer();

        /**
         * @brief Initializes shaders and geometry for rendering.
         */
        void init();

        /**
         * @brief Draws text at the specified position with given color.
         * @param position Top-left position for text rendering.
         * @param text String to render.
         * @param color RGB color of the text.
         */
        void drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const;

    private:
        /// @brief Shared pointer to the shader provider.
        std::shared_ptr<GLShaderProvider> shaderProvider;
        /// @brief Shared pointer to the logger for error reporting.
        std::shared_ptr<AntColony::Core::Logger> logger;
        /// @brief Shader program for rendering shapes (circles, frames).
        GLuint textShaderProgram;
        /// @brief Tracks initialization state.
        bool isInited;

        /**
         * @brief Loads a font with the specified size.
         * @param fontSize Size of the font in pixels.
         * @return Unique pointer to the loaded font structure, or nullptr on failure.
         */
        std::unique_ptr<Text::Font> loadFont(float fontSize) const;

        /**
         * @brief Core text rendering function.
         * @param font Font to use for rendering.
         * @param text Text to render.
         * @param x X-coordinate for text position.
         * @param y Y-coordinate for text position.
         * @param r Red component of text color.
         * @param g Green component of text color.
         * @param b Blue component of text color.
         * @param winWidth Window width for orthographic projection.
         * @param winHeight Window height for orthographic projection.
         */
        void drawTextCore(const Text::Font &font, const char *text, float x, float y, float r, float g, float b, float winWidth, float winHeight) const;

        static bool isSpecialChar(const char * c);
    };
}