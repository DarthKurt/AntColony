#include "food.h"

Food::Food(Point position, float size, int capacity) : Entity(position, 0x387d27, size), capacity(capacity) {}

void Food::bite()
{
    if (capacity > 0)
    {
        capacity -= 1;
    }
}

void Food::render(GLFWwindow *window) const
{
    drawCircleInPosition(mainColor, entitySize, window);
}

Point Food::getPosition() const { return position; }
int Food::getCapacity() const { return capacity; }
float Food::getSize() const { return entitySize; }