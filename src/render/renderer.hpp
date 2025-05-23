#pragma once

#include "../core/point.hpp"
#include "../core/color.hpp"

namespace AntColony::Render
{
    class Renderer
    {
    public:
        virtual ~Renderer() {}

        /**
         * @brief Draws a circle at the entity's position with a given size and color
         */
        virtual void drawCircleInPosition(Core::Point position, float radius, Core::Color color) const = 0;
    };
}