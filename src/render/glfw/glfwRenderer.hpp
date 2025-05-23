#pragma once

#include "../renderer.hpp"

#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include <GLFW/glfw3.h>

namespace AntColony::Render::GLFW
{
    class GLFWRenderer : public Renderer
    {
    public:
        explicit GLFWRenderer(GLFWwindow *window);
        ~GLFWRenderer() override;

        void drawCircleInPosition(Core::Point position, float radius, Core::Color color) const override;

    private:
        GLFWwindow *window;
    };
}