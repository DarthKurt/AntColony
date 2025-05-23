#pragma once

#include "food.hpp"
#include <vector>

namespace AntColony::Simulation
{
    class FoodManager
    {
    public:
        FoodManager(Core::Point colonyCenter, float colonyRadius);

        void update();
        void render(GLFWwindow *window) const;

        std::vector<Food> &getFoodParticles();

    private:
        float colonyRadius;
        void spawnFood();
        void removeDepletedFood();
        static float getCoordInBoundaries(float innerSpot, float itemSize);

        std::vector<Food> foodParticles;
    };
}
