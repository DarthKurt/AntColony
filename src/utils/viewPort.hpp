#pragma once
#include "../core/point.hpp"

namespace AntColony::Utils
{
    const float LEFT_BOUNDARY = -0.95f;
    const float RIGHT_BOUNDARY = 0.95f;

    void setColor(const unsigned int hexColor);

    bool checkViewportBoundaries(const Core::Point &newPosition);
}