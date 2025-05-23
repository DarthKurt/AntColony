#pragma once
#include "../core/point.hpp"
#include "../core/color.hpp"

namespace AntColony::Utils
{
    const float LEFT_BOUNDARY = -0.95f;
    const float RIGHT_BOUNDARY = 0.95f;

    bool checkViewportBoundaries(const Core::Point &newPosition);
}