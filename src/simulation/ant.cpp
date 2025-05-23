#include "ant.h"
#include "../utils/viewPort.h"

// Constructor: Initializes ant position
Ant::Ant(Point position, float size)
    : Entity(position, 0xfc6203, size), velocity(Point(0.0f, 0.0f)), carryFood(false) {}
Ant::Ant(Point position, Point velocity, float size)
    : Entity(position, 0xfc6203, size), velocity(velocity), carryFood(false) {}

void Ant::biteFood(Food &food)
{
    // Reduce food capacity
    food.bite();
    carryFood = true;
}

void Ant::dropFood()
{
    carryFood = false;
}

void Ant::render(GLFWwindow *window) const
{
    if (carryFood)
    {
        drawCircleInPosition(mainColor, entitySize, window);
        drawCircleInPosition(0xadf542, entitySize / 2, window);
    }
    else
    {
        drawCircleInPosition(mainColor, entitySize, window);
    }
}

bool Ant::isBusy() const { return carryFood; }
bool Ant::isMoving() const { return velocity.x != 0.0f || velocity.y != 0.0f; }

Point Ant::getVelocity() const { return velocity; }

void Ant::setVelocity(Point newVelocity) { velocity = newVelocity; }
void Ant::setPosition(Point newPosition) { position = newPosition; }

Point Ant::getPosition() const { return position; }
float Ant::getSize() const { return entitySize; }