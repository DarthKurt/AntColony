#include "text/_fonts.hpp"
#include "text/font.hpp"

#include "glShaders.hpp"
#include "glTextRenderer.hpp"
#include "glShaderProvider.hpp"

#include "../../core/logger.hpp"
#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include <cmath>
#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace AntColony::Render::GLFW
{
    // ASCII
    constexpr const auto TAB_CHAR = 9;
    constexpr const auto LINE_FEED_CHAR = 10;
    constexpr const auto RETURN_CHAR = 13;
    constexpr const auto SPACE_CHAR = 32;

    constexpr const auto TAB_SIZE = 4.0f;
    constexpr const auto MAX_ACII_CODE = 128;

    // TEXTURE
    constexpr const auto TEXTURE_SIZE = 512;

    bool GLTextRenderer::isSpecialChar(const char *c)
    {
        return *c == TAB_CHAR || *c == LINE_FEED_CHAR || *c == RETURN_CHAR || *c == SPACE_CHAR;
    }

    GLTextRenderer::GLTextRenderer(std::shared_ptr<GLShaderProvider> shaderProvider, std::shared_ptr<AntColony::Core::Logger> logger)
        : shaderProvider(shaderProvider), logger(logger), isInited(false) {}

    GLTextRenderer::~GLTextRenderer()
    {
        isInited = false;

        // Clean up all font textures
        for (auto &[size, font] : fontCache)
        {
            if (font && font->tex_id != 0)
            {
                glDeleteTextures(1, &font->tex_id);
                font->tex_id = 0;
            }
        }
        fontCache.clear();

        glDeleteProgram(textShaderProgram);
    }

    void GLTextRenderer::init()
    {
        textShaderProgram = shaderProvider->createShaderProgram(TEXT_VERTEX_SHADER_SOURCE, TEXT_FRAGMENT_SHADER_SOURCE);
        if (textShaderProgram == 0)
        {
            logger->error("Failed to create text shader program");
            return;
        }

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
        logger->debug("GLTextRenderer initialized successfully");
    }

    void GLTextRenderer::drawText(
        const Core::Point &position,
        const std::string &text,
        const Core::Color &color,
        const float fontSize)
    {
        if (!isInited)
        {
            logger->error("Renderer not initialized, skipping drawText");
            return;
        }

        // Get or load font for the requested size
        auto it = fontCache.find(fontSize);
        if (it == fontCache.end())
        {
            auto font = loadFont(fontSize);
            if (!font)
            {
                logger->error("Failed to load font size: " + std::to_string(fontSize));
                return;
            }
            it = fontCache.emplace(fontSize, font).first;
            logger->debug("Loaded new font size: " + std::to_string(fontSize));
        }

        int winWidth, winHeight;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        drawTextCore(it->second, text.c_str(), position.x, position.y, color.r, color.g, color.b, static_cast<float>(winWidth), static_cast<float>(winHeight));
    }

    std::shared_ptr<Text::Font> GLTextRenderer::loadFont(float fontSize) const
    {
        auto font = std::make_shared<Text::Font>();
        font->size = fontSize;
        font->spacing = 0.0f;
        font->sdf = false;
        font->glyphCache.reserve(MAX_ACII_CODE);

        logger->debug("Initializing font for size: " + std::to_string(fontSize));
        if (!stbtt_InitFont(&font->font, MAIN_FONT, 0))
        {
            logger->error("Failed to initialize font: Invalid font data or file");
            return nullptr;
        }

        font->scale = stbtt_ScaleForPixelHeight(&font->font, fontSize);
        stbtt_GetFontVMetrics(&font->font, &font->ascent, &font->descent, &font->lineGap);
        font->lineHeight = (font->ascent - font->descent + font->lineGap) * font->scale;
        logger->debug("Font metrics: ascent=" + std::to_string(font->ascent) +
                      ", descent=" + std::to_string(font->descent) +
                      ", lineGap=" + std::to_string(font->lineGap) +
                      ", lineHeight=" + std::to_string(font->lineHeight));

        font->w = TEXTURE_SIZE;
        font->h = TEXTURE_SIZE;
        font->bitmap = new unsigned char[font->w * font->h]();
        if (!font->bitmap)
        {
            logger->error("Failed to allocate bitmap for font texture");
            return nullptr;
        }

        int pos_x = 0, pos_y = 0, max_height = 0;
        int glyphCount = 0;

        for (int c : {SPACE_CHAR, TAB_CHAR, LINE_FEED_CHAR, RETURN_CHAR})
        {
            Text::GlyphInfo info{};
            stbtt_GetCodepointHMetrics(&font->font, c, &info.advance, &info.lsb);
            stbtt_GetCodepointBitmapBox(&font->font, c, font->scale, font->scale, &info.x0, &info.y0, &info.x1, &info.y1);
            info.tex_x = 0;
            info.tex_y = 0;
            info.x0 = 0;
            info.y0 = 0;
            info.x1 = 0;
            info.y1 = 0;

            if (c == SPACE_CHAR)
            {
                if (info.advance <= 0)
                {
                    logger->warning("Invalid advance for space, using default: " + std::to_string(info.advance));
                    info.advance = static_cast<int>(fontSize / 4.0f);
                }
            }
            else if (c == TAB_CHAR)
            {
                info.advance = font->glyphCache[SPACE_CHAR].advance * TAB_SIZE;
            }
            else
            {
                info.advance = 0;
            }

            font->glyphCache[c] = info;
            glyphCount++;
            std::string charName;
            switch (c)
            {
            case SPACE_CHAR:
                charName = " ";
                break;
            case TAB_CHAR:
                charName = "\\t";
                break;
            case LINE_FEED_CHAR:
                charName = "\\n";
                break;
            case RETURN_CHAR:
                charName = "\\r";
                break;
            }
            logger->debug("Special char '" + charName + "' advance=" + std::to_string(info.advance));
        }

        for (auto c = SPACE_CHAR + 1; c < MAX_ACII_CODE; c++)
        {
            if (c == TAB_CHAR || c == LINE_FEED_CHAR || c == RETURN_CHAR)
                continue;
            Text::GlyphInfo info{};
            stbtt_GetCodepointHMetrics(&font->font, c, &info.advance, &info.lsb);
            stbtt_GetCodepointBitmapBox(&font->font, c, font->scale, font->scale, &info.x0, &info.y0, &info.x1, &info.y1);
            int w = info.x1 - info.x0, h = info.y1 - info.y0;

            if (info.advance <= 0)
            {
                logger->warning("Invalid advance for char '" + std::string(1, static_cast<char>(c)) +
                                "', using default: " + std::to_string(info.advance));
                info.advance = static_cast<int>(fontSize / 2.0f);
            }
            if (w <= 0 || h <= 0)
            {
                logger->warning("Invalid glyph dimensions for char '" + std::string(1, static_cast<char>(c)) +
                                "', using default size");
                w = static_cast<int>(fontSize / 2.0f);
                h = static_cast<int>(fontSize);
                info.x1 = info.x0 + w;
                info.y1 = info.y0 + h;
            }
            if (pos_x + w > font->w)
            {
                pos_x = 0;
                pos_y += max_height + 1;
                max_height = 0;
            }
            auto *glyph_bitmap = stbtt_GetCodepointBitmap(&font->font, font->scale, font->scale, c, &w, &h, 0, 0);
            if (!glyph_bitmap)
            {
                logger->error("Failed to generate glyph bitmap for character: " + std::to_string(c));
                continue;
            }
            for (int j = 0; j < h; j++)
                for (int i = 0; i < w; i++)
                    font->bitmap[(pos_y + j) * font->w + pos_x + i] = glyph_bitmap[j * w + i];
            stbtt_FreeBitmap(glyph_bitmap, nullptr);
            info.tex_x = pos_x;
            info.tex_y = pos_y;
            font->glyphCache[c] = info;
            pos_x += w + 1;
            max_height = std::max(max_height, h);
            glyphCount++;
            logger->debug("Glyph '" + std::string(1, static_cast<char>(c)) + "' advance=" + std::to_string(info.advance) +
                          ", size=(" + std::to_string(w) + "," + std::to_string(h) + ")");
        }

        int nonZeroPixels = 0;
        for (int i = 0; i < font->w * font->h; i++)
        {
            if (font->bitmap[i] > 0)
                nonZeroPixels++;
        }
        logger->debug("Font texture non-zero pixels: " + std::to_string(nonZeroPixels));
        if (nonZeroPixels == 0)
        {
            logger->error("Font texture is empty");
            delete[] font->bitmap;
            return nullptr;
        }

        logger->debug("Generating texture for font size: " + std::to_string(fontSize));
        glGenTextures(1, &font->tex_id);
        if (font->tex_id == 0)
        {
            logger->error("Failed to generate texture ID for font");
            delete[] font->bitmap;
            return nullptr;
        }
        glBindTexture(GL_TEXTURE_2D, font->tex_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->w, font->h, 0, GL_RED, GL_UNSIGNED_BYTE, font->bitmap);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            logger->error("OpenGL error during texture upload: " + std::to_string(err));
            glDeleteTextures(1, &font->tex_id);
            delete[] font->bitmap;
            return nullptr;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        logger->debug("Font loaded successfully with size: " + std::to_string(fontSize));
        return font;
    }

    void GLTextRenderer::drawTextCore(const std::shared_ptr<Text::Font> font, const char *text, float x, float y, float r, float g, float b, float winWidth, float winHeight) const
    {
        if (!font || font->tex_id == 0)
        {
            logger->error("Invalid font in drawTextCore");
            return;
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->tex_id);
        GLint boundTex;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTex);
        logger->debug("Bound texture ID: " + std::to_string(boundTex));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::vector<float> vertices;
        auto cursor_x = x;
        auto cursor_y = y; // Baseline of text

        int renderedGlyphs = 0;
        for (const char *c = text; *c; c++)
        {
            if (font->glyphCache.find(*c) == font->glyphCache.end())
            {
                logger->error("Glyph not found for character: " + std::string(1, *c));
                continue;
            }

            const auto &glyph = font->glyphCache.at(*c);
            int w = glyph.x1 - glyph.x0, h = glyph.y1 - glyph.y0;

            if (isSpecialChar(c))
            {
                switch (*c)
                {
                case SPACE_CHAR:
                    cursor_x += static_cast<float>(glyph.advance) * font->scale;
                    logger->debug("Space: advance=" + std::to_string(static_cast<float>(glyph.advance) * font->scale));
                    break;
                case TAB_CHAR:
                    cursor_x += static_cast<float>(glyph.advance) * font->scale;
                    logger->debug("Tab: advance=" + std::to_string(static_cast<float>(glyph.advance) * font->scale));
                    break;
                case LINE_FEED_CHAR:
                    cursor_x = x;
                    cursor_y += font->lineHeight;
                    logger->debug("Line feed: cursor_y=" + std::to_string(cursor_y));
                    break;
                case RETURN_CHAR:
                    cursor_x = x;
                    logger->debug("Carriage return: cursor_x=" + std::to_string(cursor_x));
                    break;
                default:
                    logger->debug("Skipping unprintable: ASCII " + std::to_string(static_cast<int>(*c)));
                    break;
                }
                renderedGlyphs++;
                continue;
            }

            if (w <= 0 || h <= 0)
            {
                logger->error("Invalid glyph dimensions for character: " + std::string(1, *c));
                cursor_x += static_cast<float>(glyph.advance) * font->scale;
                continue;
            }
            float u0 = static_cast<float>(glyph.tex_x) / font->w;
            float v0 = static_cast<float>(glyph.tex_y + h) / font->h; // Bottom of glyph
            float u1 = static_cast<float>(glyph.tex_x + w) / font->w;
            float v1 = static_cast<float>(glyph.tex_y) / font->h; // Top of glyph

            // Position top of glyph above baseline
            float posX = cursor_x;
            float posY = cursor_y - font->ascent * font->scale + static_cast<float>(glyph.y0) * font->scale; // Top of glyph
            float posX2 = posX + static_cast<float>(w);
            float posY2 = posY + static_cast<float>(h);

            // Flip y-coordinates to match viewport (y=0 at top)
            float flippedPosY = winHeight - posY;
            float flippedPosY2 = winHeight - posY2;

            logger->debug("Glyph '" + std::string(1, *c) + "' quad: (" +
                          std::to_string(posX) + ", " + std::to_string(flippedPosY2) + ") to (" +
                          std::to_string(posX2) + ", " + std::to_string(flippedPosY) + "), tex: (" +
                          std::to_string(u0) + "," + std::to_string(v0) + ") to (" +
                          std::to_string(u1) + "," + std::to_string(v1) + ")");

            // Define quad vertices: bottom-left, bottom-right, top-right, bottom-left, top-right, top-left
            vertices.insert(vertices.end(), {posX, flippedPosY2, u0, v0});  // Bottom-left
            vertices.insert(vertices.end(), {posX2, flippedPosY2, u1, v0}); // Bottom-right
            vertices.insert(vertices.end(), {posX2, flippedPosY, u1, v1});  // Top-right
            vertices.insert(vertices.end(), {posX, flippedPosY2, u0, v0});  // Bottom-left
            vertices.insert(vertices.end(), {posX2, flippedPosY, u1, v1});  // Top-right
            vertices.insert(vertices.end(), {posX, flippedPosY, u0, v1});   // Top-left

            cursor_x += static_cast<float>(glyph.advance) * font->scale;
            renderedGlyphs++;
        }
        logger->debug("Prepared " + std::to_string(renderedGlyphs) + " glyphs for rendering");

        if (vertices.empty())
        {
            logger->error("No valid glyphs to render");
            glDisable(GL_BLEND);
            glBindTexture(GL_TEXTURE_2D, 0);
            return;
        }

        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(textShaderProgram);
        glUniform1i(glGetUniformLocation(textShaderProgram, "uFontTex"), 0);
        glUniform3f(glGetUniformLocation(textShaderProgram, "uColor"), r, g, b);
        auto ortho = glm::ortho(0.0f, winWidth, 0.0f, winHeight);
        glUniformMatrix4fv(glGetUniformLocation(textShaderProgram, "uOrtho"), 1, GL_FALSE, &ortho[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 4));

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            logger->error("OpenGL error during text rendering: " + std::to_string(err));
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);

        logger->debug("Text rendered: " + std::string(text));
    }
}