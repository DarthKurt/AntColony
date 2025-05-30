#include "ant.hpp"

namespace AntColony::Simulation
{
    Ant::Ant(Core::Point position, float size, int pheromoneThreshold)
        : BaseEntity(position, 0xfc6203, size), velocity(Core::Point(0.0f, 0.0f)), carryFood(false), pheromoneChargeThreshold(pheromoneThreshold) {}
    Ant::Ant(Core::Point position, Core::Point velocity, float size, int pheromoneThreshold)
        : BaseEntity(position, 0xfc6203, size), velocity(velocity), carryFood(false), pheromoneChargeThreshold(pheromoneThreshold) {}

    void Ant::biteFood(std::shared_ptr<Food> food)
    {
        if (!carryFood)
        {
            // Reduce food capacity
            food->take();

            // Start carrying
            carryFood = true;

            // Stop old movement
            velocity *= 0;

            // Start spawning if there is more food
            pheromoneExcitement = food.get()->getCapacity();

            // Immediate spawn
            pheromoneCharge = pheromoneChargeThreshold;

            // Calculate and store collision point - at the edge of the ant body facing the food
            auto directionToFood = food->getPosition() - position;
            auto distance = position.distanceTo(food->getPosition());

            if (distance > 0)
            {
                // Normalize direction vector
                directionToFood.x /= distance;
                directionToFood.y /= distance;
            }

            // Place collision point at the edge of the ant's body
            nextPheromonPosition = position + directionToFood * entitySize;
        }
    }

    void Ant::dropFood()
    {
        if (carryFood)
        {
            carryFood = false;
            velocity *= 0;
            pheromoneCharge = 0;
            pheromoneExcitement = 0;
        }
    }

    PheromoneSignal Ant::consumePheromoneCharge()
    {
        auto spawnPosition = nextPheromonPosition;
        nextPheromonPosition = position;
        return PheromoneSignal(spawnPosition, pheromoneExcitement--);
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

    bool Ant::trySpawnPheromone()
    {
        if (pheromoneExcitement <= 0 || !carryFood)
            return false;

        if (pheromoneCharge < pheromoneChargeThreshold)
        {
            pheromoneCharge += 1;
            return false;
        }

        return true;
    }

    Core::Point Ant::getVelocity() const { return velocity; }

    void Ant::setVelocity(Core::Point newVelocity) { velocity = newVelocity; }
    void Ant::setPosition(Core::Point newPosition) { position = newPosition; }

    Core::Point Ant::getPosition() const { return position; }
    float Ant::getSize() const { return entitySize; }
}