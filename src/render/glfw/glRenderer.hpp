#pragma once

#include <string>
#include <glad/glad.h>
#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include "../renderer.hpp"

namespace AntColony::Render
{
    class GLRenderer : public Renderer
    {
    public:
        explicit GLRenderer();
        ~GLRenderer() override;

        void init();

        // Renderer interface implementations
        void drawCircleInPosition(const Core::Point &position, const float radius, const Core::Color &color) const override;
        void drawFrame(const Core::Point &position, const float width, const float height, const Core::Color &color) const override;
        void drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const override;

    private:
        GLuint shaderProgram;
        GLuint circleVAO, circleVBO;
        static const int numCircleSegments = 50;

        // Helper methods for shader creation and geometry initialization
        GLuint compileShader(GLenum type, const char* src) const;
        GLuint createShaderProgram() const;
        void initCircleGeometry();

        bool isInited;
    };
}