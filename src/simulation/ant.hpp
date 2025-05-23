#pragma once

#include "entity.hpp"
#include "food.hpp"
#include "../core/point.hpp"

namespace AntColony::Simulation
{
    class Ant : public Entity, public BaseEntity
    {
    public:
        Ant(Core::Point position, float size);
        Ant(Core::Point position, Core::Point velocity, float size);

        void render(const Render::Renderer &renderer) const override;

        void biteFood(Food *food);
        void dropFood();

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
        bool carryFood;
    };

}