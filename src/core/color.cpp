#include "color.hpp"

namespace AntColony::Core
{
    Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}
    Color::Color(Color &anotherColor) : Color(anotherColor.r, anotherColor.g, anotherColor.b) {}
    Color::Color(unsigned int hexColor)
    {
        r = ((hexColor >> 16) & 0xFF) / 255.0f; // Extract red
        g = ((hexColor >> 8) & 0xFF) / 255.0f;  // Extract green
        b = (hexColor & 0xFF) / 255.0f;         // Extract blue
    }
}