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
    GLTextRenderer::GLTextRenderer(std::shared_ptr<GLShaderProvider> shaderProvider, std::shared_ptr<AntColony::Core::Logger> logger)
        : shaderProvider(shaderProvider), logger(logger), isInited(false) {}

    GLTextRenderer::~GLTextRenderer()
    {
        isInited = false;
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

    void GLTextRenderer::drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const
    {
        if (!isInited)
        {
            logger->error("Renderer not initialized, skipping drawText");
            return;
        }

        auto font = loadFont(32.0f);
        if (!font)
        {
            logger->error("Failed to load font, skipping text rendering");
            return;
        }
        int winWidth, winHeight;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &winWidth, &winHeight);
        drawTextCore(*font, text.c_str(), 640.0f, 360.0f, color.r, color.g, color.b, static_cast<float>(winWidth), static_cast<float>(winHeight));
    }

    std::unique_ptr<Text::Font> GLTextRenderer::loadFont(float fontSize) const
    {
        auto font = std::make_unique<Text::Font>();
        font->size = fontSize;
        font->spacing = 0.0f;
        font->sdf = false;

        if (!stbtt_InitFont(&font->font, MAIN_FONT, 0))
        {
            logger->error("Failed to initialize font: Invalid font data or file");
            return nullptr;
        }
        font->scale = stbtt_ScaleForPixelHeight(&font->font, fontSize);
        stbtt_GetFontVMetrics(&font->font, &font->ascent, &font->descent, &font->lineGap);
        font->lineHeight = (font->ascent - font->descent + font->lineGap) * font->scale;

        font->w = 512;
        font->h = 512;
        font->bitmap = new unsigned char[font->w * font->h]();
        if (!font->bitmap)
        {
            logger->error("Failed to allocate bitmap for font texture");
            return nullptr;
        }

        int pos_x = 0, pos_y = 0, max_height = 0;
        int glyphCount = 0;
        for (int c = 32; c < 128; c++)
        {
            Text::GlyphInfo info;
            stbtt_GetCodepointHMetrics(&font->font, c, &info.advance, &info.lsb);
            stbtt_GetCodepointBitmapBox(&font->font, c, font->scale, font->scale, &info.x0, &info.y0, &info.x1, &info.y1);
            int w = info.x1 - info.x0, h = info.y1 - info.y0;
            if (c == 32)
            {
                info.tex_x = 0;
                info.tex_y = 0;
                info.x0 = 0;
                info.y0 = 0;
                info.x1 = 0;
                info.y1 = 0;
                font->glyphCache[c] = info;
                glyphCount++;
                continue;
            }
            if (pos_x + w > font->w)
            {
                pos_x = 0;
                pos_y += max_height + 1;
                max_height = 0;
            }
            unsigned char *glyph_bitmap = stbtt_GetCodepointBitmap(&font->font, font->scale, font->scale, c, &w, &h, 0, 0);
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
            return nullptr;
        }

        logger->debug("Loaded " + std::to_string(glyphCount) + " glyphs for font");

        glGenTextures(1, &font->tex_id);
        if (font->tex_id == 0)
        {
            logger->error("Failed to generate texture ID for font");
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
            return nullptr;
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        logger->debug("Font loaded successfully with size: " + std::to_string(fontSize));
        return font;
    }

    void GLTextRenderer::drawTextCore(const Text::Font &font, const char *text, float x, float y, float r, float g, float b, float winWidth, float winHeight) const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font.tex_id);
        GLint boundTex;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTex);
        logger->debug("Bound texture ID: " + std::to_string(boundTex));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        std::vector<float> vertices;
        float cursor_x = x;
        int renderedGlyphs = 0;
        for (const char *c = text; *c; c++)
        {
            if (font.glyphCache.find(*c) == font.glyphCache.end())
            {
                logger->error("Glyph not found for character: " + std::string(1, *c));
                continue;
            }
            const auto &glyph = font.glyphCache.at(*c);
            int w = glyph.x1 - glyph.x0, h = glyph.y1 - glyph.y0;
            if (*c == 32)
            {
                cursor_x += glyph.advance;
                renderedGlyphs++;
                continue;
            }
            if (w <= 0 || h <= 0)
            {
                logger->error("Invalid glyph dimensions for character: " + std::string(1, *c));
                continue;
            }
            float u0 = static_cast<float>(glyph.tex_x) / font.w;
            float v0 = static_cast<float>(glyph.tex_y) / font.h;
            float u1 = static_cast<float>(glyph.tex_x + w) / font.w;
            float v1 = static_cast<float>(glyph.tex_y + h) / font.h;

            float posX = cursor_x;
            float posY = y - h / 2.0f; // Center vertically
            float posX2 = posX + w;
            float posY2 = posY + h;

            logger->debug("Glyph '" + std::string(1, *c) + "' quad: (" +
                          std::to_string(posX) + ", " + std::to_string(posY) + ") to (" +
                          std::to_string(posX2) + ", " + std::to_string(posY2) + ")");

            vertices.insert(vertices.end(), {posX, posY, u0, v0});
            vertices.insert(vertices.end(), {posX2, posY, u1, v0});
            vertices.insert(vertices.end(), {posX2, posY2, u1, v1});
            vertices.insert(vertices.end(), {posX, posY, u0, v0});
            vertices.insert(vertices.end(), {posX2, posY2, u1, v1});
            vertices.insert(vertices.end(), {posX, posY2, u0, v1});

            cursor_x += glyph.advance;
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
        auto ortho = glm::ortho(0.0f, winWidth, winHeight, 0.0f);
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