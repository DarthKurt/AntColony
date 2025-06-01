#pragma once

#include "_fonts.hpp"
#include <unordered_map>

namespace AntColony::Render::GLFW::Text
{
    /**
     * @brief Structure to hold glyph information for text rendering.
     */
    struct GlyphInfo
    {
        /// @brief Glyph advance width.
        int advance;
        /// @brief Left side bearing of the glyph.
        int lsb;
        /// @brief X-coordinate of the glyph's bottom-left corner in the font atlas.
        int tex_x;
        /// @brief Y-coordinate of the glyph's bottom-left corner in the font atlas.
        int tex_y;
        /// @brief Left bound of the glyph bitmap.
        int x0;
        /// @brief Top bound of the glyph bitmap.
        int y0;
        /// @brief Right bound of the glyph bitmap.
        int x1;
        /// @brief Bottom bound of the glyph bitmap.
        int y1;
    };
}