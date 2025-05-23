#pragma once

#include "../core/point.hpp"

#include <GLFW/glfw3.h>

namespace AntColony::Simulation
{
    class Entity
    {
    public:
        /**
         * @brief Method for rendering the entity
         */
        virtual void render(GLFWwindow *window) const = 0;

        virtual Core::Point getPosition() const = 0;
        virtual float getSize() const = 0;
    };
}
