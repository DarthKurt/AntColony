#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"

#include "../core/point.hpp"

namespace AntColony::Simulation
{
    class Counter : public Entity, public BaseEntity
    {
    public:
        Counter(Core::Point position, float size);

        void increment(int count);
        void increment();

        void render(Render::Renderer &renderer) override;

        Core::Point getPosition() const override;
        float getSize() const override;

    private:
        int counter;
    };
}