#include "viewPort.hpp"

namespace AntColony::Core
{
    ViewPort::ViewPort(float minX, float minY, float maxX, float maxY)
        : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

    bool ViewPort::checkViewportBoundaries(const Point &position) const
    {
        return minX < position.x && position.x < maxX && minY < position.y && position.y < maxY;
    }
}
