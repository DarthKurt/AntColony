#pragma once
#include "_fwd.hpp"
#include "text/_fwd.hpp"
#include "../../core/_fwd.hpp"

#include <memory>
#include <string>
#include "_gl.hpp"
#include <unordered_map>
#include <vector>

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
         * @brief Initializes shaders for rendering.
         */
        void init();

        /**
         * @brief Draws text at the specified position with given color.
         * @param position Top-left position for text rendering.
         * @param text String to render.
         * @param color RGB color of the text.
         * @param fontSize Size of the font.
         */
        void drawText(const Core::Point &position, const std::string &text, const Core::Color &color, const float fontSize);

    private:
        /// @brief Shared pointer to the shader provider.
        std::shared_ptr<GLShaderProvider> shaderProvider;
        /// @brief Shared pointer to the logger for error reporting.
        std::shared_ptr<AntColony::Core::Logger> logger;
        /// @brief Shader program for rendering shapes (circles, frames).
        GLuint textShaderProgram;
        /// @brief Shader program for rendering shapes (circles, frames).
        std::unordered_map<float, std::shared_ptr<Text::Font>> fontCache;
        /// @brief Tracks initialization state.
        bool isInited;

        /**
         * @brief Loads a font with the specified size.
         * @param fontSize Size of the font in pixels.
         * @return Unique pointer to the loaded font structure, or nullptr on failure.
         */
        std::shared_ptr<Text::Font> loadFont(float fontSize) const;

        /**
         * @brief Factory method that initializes font structure with basic parameters.
         * @param fontSize Size of the font in pixels.
         * @param numberOfGlyphs Hint of how many glyphs will be stored in the font.
         * @return Pointer to the loaded font structure.
         */
        static std::shared_ptr<Text::Font> createFont(float fontSize, int numberOfGlyphs);

        /**
         * @brief Initializes font metrics.
         * @param font Font to initialize.
         * @param fontData Binary data of the font.
         * @param fontSize Size of the font in pixels.
         * @return True if successful, false otherwise.
         */
        static bool tryInitializeFont(std::shared_ptr<Text::Font> font, const unsigned char *fontData, float fontSize);

        /**
         * @brief Allocates memory for the font bitmap.
         * @param font Font to allocate bitmap for.
         * @return True if successful, false otherwise.
         */
        static bool tryAllocateFontBitmap(std::shared_ptr<Text::Font> font);

        /**
         * @brief Generates glyphs for special characters (space, tab, newline, etc.).
         * @param font Font to generate glyphs for.
         * @param fontSize Size of the font in pixels.
         */
        void generateSpecialCharacterGlyphs(std::shared_ptr<Text::Font> font, float fontSize) const;

        /**
         * @brief Generates glyphs for regular characters.
         * @param font Font to generate glyphs for.
         * @param pos_x X position in the bitmap, updated as glyphs are added.
         * @param pos_y Y position in the bitmap, updated as glyphs are added.
         * @param max_height Maximum height of glyphs in the current row, updated as glyphs are added.
         */
        void generateRegularCharacterGlyphs(std::shared_ptr<Text::Font> font, int &pos_x, int &pos_y, int &max_height) const;

        /**
         * @brief Copies a glyph bitmap to the font bitmap.
         * @param font Font to copy to.
         * @param glyph_bitmap Source bitmap to copy from.
         * @param pos_x X position in the destination bitmap.
         * @param pos_y Y position in the destination bitmap.
         * @param w Width of the glyph.
         * @param h Height of the glyph.
         */
        static void copyGlyphToBitmap(std::shared_ptr<Text::Font> font, const unsigned char *glyph_bitmap, int pos_x, int pos_y, int w, int h);

        /**
         * @brief Validates that the font texture contains actual data.
         * @param font Font to validate.
         * @return True if valid, false otherwise.
         */
        bool validateFontTexture(std::shared_ptr<Text::Font> font) const;

        /**
         * @brief Creates an OpenGL texture from the font bitmap.
         * @param font Font to create texture for.
         * @return True if successful, false otherwise.
         */
        bool createFontTexture(std::shared_ptr<Text::Font> font) const;

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
        void drawTextCore(const std::shared_ptr<Text::Font> font, const char *text, float x, float y, float r, float g, float b, float winWidth, float winHeight) const;

        /**
         * @brief Handles cursor positioning for special characters.
         * @param font Font in use.
         * @param c Character to handle.
         * @param cursor_x Current X cursor position.
         * @param cursor_y Current Y cursor position.
         * @param x Original X position.
         * @param fontScale Font scale factor.
         * @param lineHeight Line height for newline.
         */
        void handleSpecialCharacter(
            const std::shared_ptr<Text::Font> font,
            char c,
            float &cursor_x,
            float &cursor_y,
            float x,
            float fontScale,
            float lineHeight) const;

        /**
         * @brief Adds vertices for a regular character glyph.
         * @param font Font in use.
         * @param glyph Glyph information.
         * @param cursor_x Current X cursor position.
         * @param cursor_y Current Y cursor position.
         * @param winHeight Window height for coordinate flipping.
         * @param vertices Vector to add vertices to.
         * @return True if successful, false otherwise.
         */
        bool tryAddGlyphVertices(
            const std::shared_ptr<Text::Font> font,
            const Text::GlyphInfo &glyph,
            const float cursor_x,
            const float cursor_y,
            float winHeight,
            std::vector<float> &vertices) const;

        /**
         * @brief Sets up OpenGL state for text rendering.
         * @param boundTex Font texture id to bind.
         */
        static GLint setupTexture(const unsigned int &textId);

        /**
         * @brief Renders vertices using OpenGL.
         * @param vertices Vertex data to render.
         * @param r Red component of text color.
         * @param g Green component of text color.
         * @param b Blue component of text color.
         * @param winWidth Window width for orthographic projection.
         * @param winHeight Window height for orthographic projection.
         */
        void renderVertices(const std::vector<float> &vertices, float r, float g, float b, float winWidth, float winHeight) const;

        static bool isSpecialChar(const char *c);
    };
}