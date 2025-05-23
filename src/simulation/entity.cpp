#include "entity.h"
#include "../utils/viewPort.h"

#include <algorithm>
#include <iostream>

Entity::Entity(Point position, unsigned int hexColor, float size) : position(position), mainColor(hexColor), entitySize(size) {}

// Draws a circle at the entity's position with a given size and color
void Entity::drawCircleInPosition(unsigned int hexColor, float size, GLFWwindow *window) const
{
    // Get viewport dimensions in pixels
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Assume orthographic projection maps height to [-1, 1] (2 units total)
    // Adjust size from pixel-based fraction to coordinate units
    float minDimension = std::min(width, height);
    float pixelSize = size * minDimension;         // Size as fraction of viewport height in pixels
    float scaledSize = (pixelSize / minDimension); // Convert to coordinate units

    // Set color
    setColor(hexColor);

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