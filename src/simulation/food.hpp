#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"

#include "../core/point.hpp"

#include <GLFW/glfw3.h>

namespace AntColony::Simulation
{
    class Food : public Entity, public BaseEntity
    {
    public:
        Food(Core::Point position, float size, int capacity);

        void bite();
        void render(GLFWwindow *window) const override;

        int getCapacity() const;
        Core::Point getPosition() const override;
        float getSize() const override;

    private:
        int capacity;
    };
}