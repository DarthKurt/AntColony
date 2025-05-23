#pragma once
#include "point.hpp"

namespace AntColony::Core
{
    struct ViewPort
    {
        ViewPort(float minX, float minY, float maxX, float maxY);

        float minX;
        float minY;
        float maxX;
        float maxY;

        bool checkViewportBoundaries(const Core::Point &newPosition) const;
    };
}