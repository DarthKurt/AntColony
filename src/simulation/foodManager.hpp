#pragma once

#include "food.hpp"
#include "../core/viewPort.hpp"
#include <vector>

namespace AntColony::Simulation
{
    class FoodManager
    {
    public:
        FoodManager(Core::Point colonyCenter, float colonyRadius, float foodRadius, Core::ViewPort viewPort);

        void update();
        void render(const Render::Renderer &renderer) const;

        std::vector<Food> &getFoodParticles();

    private:
        Core::Point colonyCenter;
        float colonyRadius;
        float foodRadius;
        Core::ViewPort viewPort;

        void spawnFood();
        void removeDepletedFood();

        std::vector<Food> foodParticles;
    };
}
