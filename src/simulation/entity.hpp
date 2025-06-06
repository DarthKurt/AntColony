#pragma once

#include "../core/point.hpp"
#include "../render/renderer.hpp"

namespace AntColony::Simulation
{
    class Entity
    {
    public:
        /**
         * @brief Method for rendering the entity
         */
        virtual void render(Render::Renderer &renderer) = 0;

        virtual Core::Point getPosition() const = 0;
        virtual float getSize() const = 0;
    };
}
