#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"

#include "../core/point.hpp"

#include <functional>

namespace AntColony::Simulation
{
    class Food : public Entity, public BaseEntity
    {
    public:
        Food(Core::Point position, float size, int capacity, std::function<void(Food*)> onEmpty);

        void take();
        void render(const Render::Renderer &renderer) const override;

        int getCapacity() const;
        Core::Point getPosition() const override;
        float getSize() const override;

    private:
        int capacity;

        /**
         * @brief Is called when food spot is empty
         */
        std::function<void(Food*)> onEmpty;
    };
}