#include "viewPort.hpp"
#include "../core/color.hpp"
#include <GLFW/glfw3.h>

namespace AntColony::Utils
{
    bool checkViewportBoundaries(const Core::Point &newPosition)
    {
        return LEFT_BOUNDARY < newPosition.x && newPosition.x < RIGHT_BOUNDARY && LEFT_BOUNDARY < newPosition.y && newPosition.y < RIGHT_BOUNDARY;
    }
}