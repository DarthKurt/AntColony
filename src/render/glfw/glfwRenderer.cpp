#include "glfwRenderer.hpp"

#include "../../core/point.hpp"
#include "../../core/color.hpp"

#include <algorithm>
#include <iostream>
#include <GLFW/glfw3.h>

namespace AntColony::Render::GLFW
{
    GLFWRenderer::GLFWRenderer(GLFWwindow *window) : window(window) {}

    GLFWRenderer::~GLFWRenderer() {}

    void GLFWRenderer::drawCircleInPosition(Core::Point position, float radius, Core::Color color) const
    {
        // Get viewport dimensions in pixels
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Assume orthographic projection maps height to [-1, 1] (2 units total)
        // Adjust size from pixel-based fraction to coordinate units
        float minDimension = std::min(width, height);
        // Size as fraction of viewport height in pixels
        float pixelSize = radius * minDimension;
        // Convert to coordinate units
        float scaledSize = (pixelSize / minDimension);

        // Set color
        glColor3f(color.r, color.g, color.b);

        // Draw circle
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(position.x, position.y); // Center
        for (int i = 0; i <= 50; i++)
        {
            float angle = i * 2.0f * M_PI / 50;
            glVertex2f(position.x + scaledSize * cos(angle),
                       position.y + scaledSize * sin(angle));
        }
        glEnd();
    }

}
