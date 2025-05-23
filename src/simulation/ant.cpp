#include "ant.hpp"
#include "../utils/viewPort.hpp"
#include "../core/point.hpp"

namespace AntColony::Simulation
{
    Ant::Ant(Core::Point position, float size)
        : BaseEntity(position, 0xfc6203, size), velocity(Core::Point(0.0f, 0.0f)), carryFood(false) {}
    Ant::Ant(Core::Point position, Core::Point velocity, float size)
        : BaseEntity(position, 0xfc6203, size), velocity(velocity), carryFood(false) {}

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

    void Ant::render(const Render::Renderer &renderer) const
    {
        if (carryFood)
        {
            renderer.drawCircleInPosition(position, entitySize, Core::Color(mainColor));
            renderer.drawCircleInPosition(position, entitySize / 2, Core::Color(0xadf542));
        }
        else
        {
            renderer.drawCircleInPosition(position, entitySize, Core::Color(mainColor));
        }
    }

    bool Ant::isBusy() const { return carryFood; }
    bool Ant::isMoving() const { return velocity.x != 0.0f || velocity.y != 0.0f; }

    Core::Point Ant::getVelocity() const { return velocity; }

    void Ant::setVelocity(Core::Point newVelocity) { velocity = newVelocity; }
    void Ant::setPosition(Core::Point newPosition) { position = newPosition; }

    Core::Point Ant::getPosition() const { return position; }
    float Ant::getSize() const { return entitySize; }
}