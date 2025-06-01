#pragma once

#include "_fonts.hpp"

#include "glyphInfo.hpp"
#include <unordered_map>

namespace AntColony::Render::GLFW::Text
{
    /**
     * @brief Structure to hold font data for text rendering.
     */
    struct Font
    {
        /// @brief Font size in pixels.
        float size;
        /// @brief Spacing between characters.
        float spacing;
        /// @brief Whether the font uses signed distance fields (SDF).
        bool sdf;
        /// @brief Scaling factor for font rendering.
        float scale;
        /// @brief Line height for the font.
        float lineHeight;
        /// @brief Font ascent (distance from baseline to top).
        int ascent;
        /// @brief Font descent (distance from baseline to bottom).
        int descent;
        /// @brief Line gap for the font.
        int lineGap;
        /// @brief OpenGL texture ID for the font atlas.
        unsigned int tex_id;
        /// @brief Width of the font atlas texture.
        int w;
        /// @brief Height of the font atlas texture.
        int h;
        /// @brief Bitmap data for the font atlas.
        unsigned char *bitmap;
        /// @brief STB TrueType font info.
        stbtt_fontinfo font;
        /// @brief Cache of glyph information, indexed by character code.
        std::unordered_map<int, GlyphInfo> glyphCache;

        /**
         * @brief Constructor initializes default values.
         */
        Font() : size(0.0f), spacing(0.0f), sdf(false), scale(0.0f), lineHeight(0.0f),
                 ascent(0), descent(0), lineGap(0), tex_id(0), w(0), h(0), bitmap(nullptr) {}

        /**
         * @brief Destructor frees bitmap if allocated.
         */
        ~Font()
        {
            if (bitmap)
            {
                delete[] bitmap;
                bitmap = nullptr;
            }
        }

        // Disable copy to prevent double-free of bitmap
        Font(const Font &) = delete;
        Font &operator=(const Font &) = delete;

        // Allow move for std::unique_ptr compatibility
        Font(Font &&) = default;
        Font &operator=(Font &&) = default;
    };
}