#include "colony.h"
#include "../utils/viewPort.h"

Colony::Colony(Point position, float size)
    : Entity(position, 0x7d4827, size)
{
}

Colony::Colony(Point position, float size, unsigned int hexColor)
    : Entity(position, hexColor, size)
{
}

void Colony::render(GLFWwindow *window) const
{
    drawCircleInPosition(mainColor, entitySize, window);
}

Point Colony::getPosition() const { return position; }
float Colony::getSize() const { return entitySize; }
