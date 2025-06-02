#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"
#include "pheromoneSignal.hpp"

#include "../core/point.hpp"

namespace AntColony::Simulation
{
    class Pheromone : public Entity, public BaseEntity
    {
    public:
        Pheromone(PheromoneSignal signal, float size, int strength);

        void render(Render::Renderer &renderer) override;
        void evaporate();

        int getStrength() const;
        Core::Point getPosition() const override;
        float getSize() const override;

    private:
        int strength;
    };
}