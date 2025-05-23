#pragma once

namespace AntColony::Core
{
    struct Color
    {
        float r, g, b;

        Color(float r, float g, float b);

        explicit Color(Color &anotherColor);
        explicit Color(unsigned int hexColor);
    };
}