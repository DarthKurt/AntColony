#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"
#include "food.hpp"
#include "pheromoneSignal.hpp"

#include "../core/point.hpp"

#include <memory>

namespace AntColony::Simulation
{
    class Ant : public Entity, public BaseEntity
    {
    public:
        Ant(Core::Point position, float size, int pheromoneThreshold);
        Ant(Core::Point position, Core::Point velocity, float size, int pheromoneThreshold);

        void render(Render::Renderer &renderer) override;

        void biteFood(std::shared_ptr<Food> food);
        void dropFood();
        bool trySpawnPheromone();

        PheromoneSignal consumePheromoneCharge();

        void setPosition(Core::Point newPosition);
        void setVelocity(Core::Point newVelocity);
        Core::Point getVelocity() const;

        bool isBusy() const;
        bool isMoving() const;

        Core::Point getPosition() const override;
        float getSize() const override;

    private:
        // Stores previous movement direction
        Core::Point velocity;

        // Is carrying food
        bool carryFood;

        // Ant feels excited
        int pheromoneExcitement;

        // Readiness to spawn pheromone
        int pheromoneCharge;

        // When to spawn
        const int pheromoneChargeThreshold;

        // Store point where to spawn
        Core::Point nextPheromonPosition;
    };

}