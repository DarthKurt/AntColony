#pragma once

#include "entity.hpp"
#include "baseEntity.hpp"
#include "../core/point.hpp"

#include <GLFW/glfw3.h>
#include <vector>

namespace AntColony::Simulation
{
    class Colony : public Entity, public BaseEntity
    {
    public:
        Colony(Core::Point position, float size);
        Colony(Core::Point position, float size, unsigned int hexColor);

        void render(GLFWwindow *window) const override;

        Core::Point getPosition() const override;
        float getSize() const override;
    };
}