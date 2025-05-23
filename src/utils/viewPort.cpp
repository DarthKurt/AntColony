#include "viewPort.hpp"
#include <GLFW/glfw3.h>

void setColor(const unsigned int hexColor)
{
    const float r = ((hexColor >> 16) & 0xFF) / 255.0f; // Extract red
    const float g = ((hexColor >> 8) & 0xFF) / 255.0f;  // Extract green
    const float b = (hexColor & 0xFF) / 255.0f;         // Extract blue
    glColor3f(r, g, b);
}

bool checkViewportBoundaries(const Point &newPosition)
{
    return LEFT_BOUNDARY < newPosition.x && newPosition.x < RIGHT_BOUNDARY && LEFT_BOUNDARY < newPosition.y && newPosition.y < RIGHT_BOUNDARY;
}
