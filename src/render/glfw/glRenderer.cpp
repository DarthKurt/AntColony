#include "glRenderer.hpp"
#include "glShaders.hpp"

#include <cmath>
#include <vector>
#include <iostream>

#include <dejavu_fonts.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace AntColony::Render::GLFW
{
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
        GLuint vertexShader = compileShader(GL_VERTEX_SHADER, VERTEXT_SHADER_SOURCE);
        GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
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

    // // Basic text drawing implemented as a placeholder (draws a simple horizontal line)
    // void GLRenderer::drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const
    // {
    //     if (!isInited)
    //         return;

    //     // Compute width as fraction of text length (placeholder)
    //     float charWidth = 0.02f;
    //     float textWidth = text.size() * charWidth;
    //     float halfWidth = textWidth / 2.0f;

    //     float vertices[4] = {
    //         position.x - halfWidth, position.y,
    //         position.x + halfWidth, position.y};

    //     // On\-the\-fly VAO/VBO for a line
    //     GLuint vaoTmp, vboTmp;
    //     glGenVertexArrays(1, &vaoTmp);
    //     glGenBuffers(1, &vboTmp);
    //     glBindVertexArray(vaoTmp);
    //     glBindBuffer(GL_ARRAY_BUFFER, vboTmp);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    //     glEnableVertexAttribArray(0);

    //     glUseProgram(shaderProgram);
    //     GLint centerLoc = glGetUniformLocation(shaderProgram, "uCenter");
    //     GLint scaleLoc = glGetUniformLocation(shaderProgram, "uScale");
    //     GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    //     glUniform2f(centerLoc, 0.0f, 0.0f);
    //     glUniform1f(scaleLoc, 1.0f);
    //     glUniform3f(colorLoc, color.r, color.g, color.b);

    //     glBindVertexArray(vaoTmp);
    //     glDrawArrays(GL_LINES, 0, 2);
    //     glBindVertexArray(0);

    //     glDeleteBuffers(1, &vboTmp);
    //     glDeleteVertexArrays(1, &vaoTmp);
    // }

    Text::Font *GLRenderer::loadFont(float fontSize) const
    {
        auto *font = new Text::Font();
        font->size = fontSize;
        font->spacing = 0.0f;
        font->sdf = false;

        // Initialize the font from the embedded DejaVu Sans Mono font data
        stbtt_InitFont(&font->font, _usr_share_fonts_truetype_dejavu_DejaVuSansMono_ttf, 0);

        // Calculate scale for the desired pixel height
        font->scale = stbtt_ScaleForPixelHeight(&font->font, fontSize);

        // Get font metrics
        stbtt_GetFontVMetrics(&font->font, &font->ascent, &font->descent, &font->lineGap);
        font->lineHeight = (font->ascent - font->descent + font->lineGap) * font->scale;

        // Create texture atlas for glyphs
        font->w = 512;
        font->h = 512;
        font->bitmap = new unsigned char[font->w * font->h](); // Zero-initialized

        // Create OpenGL texture for the atlas
        glGenTextures(1, &font->tex_id);
        glBindTexture(GL_TEXTURE_2D, font->tex_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font->w, font->h, 0, GL_RED, GL_UNSIGNED_BYTE, font->bitmap);

        // Cache common ASCII characters
        for (int c = 32; c < 128; c++)
        {
            Text::GlyphInfo info;
            stbtt_GetCodepointHMetrics(&font->font, c, &info.advance, &info.lsb);
            stbtt_GetCodepointBitmapBox(&font->font, c, font->scale, font->scale,
                                        &info.x0, &info.y0, &info.x1, &info.y1);
            font->glyphCache[c] = info;
        }

        return font;
    }

    void GLRenderer::drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const
    {
        if (!isInited)
            return;

        auto *font = loadFont(16.0f);
        if (font)
        {
            drawTextCore(font, text.c_str(), position.x, position.y, color.r, color.g, color.b);
            freeFont(font);
        }
    }

    void GLRenderer::freeFont(Text::Font *font) const
    {
        if (font)
        {
            // Delete the OpenGL texture
            glDeleteTextures(1, &font->tex_id);

            // Free the bitmap memory
            delete[] font->bitmap;

            // Free the font struct itself
            delete font;
        }
    }

    void GLRenderer::drawTextCore(Text::Font *font, const char *text, float x, float y, float r, float g, float b) const
    {
        // Bind the font texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, font->tex_id);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Calculate font scale and metrics.
        float scale = stbtt_ScaleForPixelHeight(&font->font, font->size);
        int ascent, descent, line_gap;
        stbtt_GetFontVMetrics(&font->font, &ascent, &descent, &line_gap);
        float baseline = ascent * scale;

        int bitmap_w = font->w, bitmap_h = font->h;
        unsigned char *bitmap = font->bitmap;
        int pos_x = 0, pos_y = 0;

        // Prepare a dynamic array for vertex data (x, y, u, v)
        std::vector<float> vertices;

        // Process each character in the text.
        for (const char *c = text; *c; c++)
        {
            int advance, lsb, x0, y0, x1, y1;
            stbtt_GetCodepointHMetrics(&font->font, *c, &advance, &lsb);
            stbtt_GetCodepointBitmapBox(&font->font, *c, scale, scale, &x0, &y0, &x1, &y1);

            int w = x1 - x0, h = y1 - y0;
            if (pos_x + w > bitmap_w)
            {
                pos_x = 0;
                pos_y += static_cast<int>(font->size * 1.2f);
            }

            unsigned char *glyph_bitmap = stbtt_GetCodepointBitmap(&font->font, scale, scale, *c, &w, &h, 0, 0);
            for (int j = 0; j < h; j++)
                for (int i = 0; i < w; i++)
                    bitmap[(pos_y + j) * bitmap_w + pos_x + i] = glyph_bitmap[j * w + i];
            stbtt_FreeBitmap(glyph_bitmap, NULL);
            glTexSubImage2D(GL_TEXTURE_2D, 0, pos_x, pos_y, w, h, GL_RED, GL_UNSIGNED_BYTE, bitmap);

            float u0 = static_cast<float>(pos_x) / bitmap_w;
            float v0 = static_cast<float>(pos_y) / bitmap_h;
            float u1 = static_cast<float>(pos_x + w) / bitmap_w;
            float v1 = static_cast<float>(pos_y + h) / bitmap_h;

            // Calculate glyph quad positions
            float posX = x + x0 * scale;
            float posY = y - y0 * scale;

            // First triangle
            vertices.push_back(posX); // x
            vertices.push_back(posY); // y
            vertices.push_back(u0);   // u
            vertices.push_back(v0);   // v

            vertices.push_back(posX + w * scale); // x
            vertices.push_back(posY);             // y
            vertices.push_back(u1);               // u
            vertices.push_back(v0);               // v

            vertices.push_back(posX + w * scale); // x
            vertices.push_back(posY - h * scale); // y
            vertices.push_back(u1);               // u
            vertices.push_back(v1);               // v

            // Second triangle
            vertices.push_back(posX); // x
            vertices.push_back(posY); // y
            vertices.push_back(u0);   // u
            vertices.push_back(v0);   // v

            vertices.push_back(posX + w * scale); // x
            vertices.push_back(posY - h * scale); // y
            vertices.push_back(u1);               // u
            vertices.push_back(v1);               // v

            vertices.push_back(posX);             // x
            vertices.push_back(posY - h * scale); // y
            vertices.push_back(u0);               // u
            vertices.push_back(v1);               // v

            x += advance * scale;
            pos_x += w;
        }

        // Create temporary VAO and VBO to render the text.
        GLuint vao, vbo;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

        // Assume location 0: position (vec2), location 1: texture coordinates (vec2)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Use the shader program and set text color uniform.
        glUseProgram(shaderProgram);
        GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3f(colorLoc, r, g, b);

        // Draw all glyph quads in a single call.
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 4));

        // Cleanup temporary VAO/VBO.
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);

        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}