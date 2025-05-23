#pragma once

#include "../core/point.hpp"

#include <GLFW/glfw3.h>

namespace AntColony::Simulation
{
    class BaseEntity
    {
    public:
        virtual ~BaseEntity() {}

    protected:
        BaseEntity(Core::Point position, unsigned int hexColor, float size);

        Core::Point position;
        unsigned int mainColor;
        float entitySize;

        /**
         * @brief Draws a circle at the entity's position with a given size and color
         */
        virtual void drawCircleInPosition(unsigned int hexColor, float size, GLFWwindow *window) const;
    };
}
