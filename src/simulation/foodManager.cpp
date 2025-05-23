#include "foodManager.hpp"
#include "../utils/randomGenerator.hpp"

#include <algorithm>
#include <cmath>

namespace AntColony::Simulation
{
    FoodManager::FoodManager(Core::Point colonyCenter, float colonyRadius, float foodRadius, Core::ViewPort viewPort)
        : colonyCenter(colonyCenter), colonyRadius(colonyRadius), foodRadius(foodRadius), viewPort(viewPort) {}

    void FoodManager::spawnFood()
    {
        auto &random = AntColony::Utils::RandomGenerator::getInstance();

        const auto maxCapacity = random.getInt(1, 3);

        // Generate a random angle (0 to 2π)
        const auto angle = random.getFloat(0, 2 * M_PI);

        // Calculate direction vector
        const auto dx = std::cos(angle);
        const auto dy = std::sin(angle);

        // Calculate distance to viewport boundaries in this direction
        float distToEdge;
        if (std::abs(dx) < 0.0001f)
        {
            // Vertical direction
            distToEdge = (dy > 0) ? (viewPort.maxY - colonyCenter.y) : (colonyCenter.y - viewPort.minY);
        }
        else if (std::abs(dy) < 0.0001f)
        {
            // Horizontal direction
            distToEdge = (dx > 0) ? (viewPort.maxX - colonyCenter.x) : (colonyCenter.x - viewPort.minX);
        }
        else
        {
            // Diagonal direction - find which boundary we hit first
            auto tx1 = (viewPort.minX - colonyCenter.x) / dx;
            auto tx2 = (viewPort.maxX - colonyCenter.x) / dx;
            auto ty1 = (viewPort.minY - colonyCenter.y) / dy;
            auto ty2 = (viewPort.maxY - colonyCenter.y) / dy;

            // Consider only positive distances
            auto tx = (dx > 0) ? tx2 : tx1;
            auto ty = (dy > 0) ? ty2 : ty1;

            distToEdge = std::min(std::abs(tx), std::abs(ty));
        }

        // Choose random distance between colony radius and edge (with buffer for food size)
        auto distance = random.getFloat(colonyRadius + foodRadius, distToEdge - foodRadius);

        // Calculate final position
        auto x = colonyCenter.x + distance * dx;
        auto y = colonyCenter.y + distance * dy;

        foodParticles.emplace_back(Core::Point(x, y), foodRadius, maxCapacity);
    }

    void FoodManager::removeDepletedFood()
    {
        foodParticles.erase(
            std::remove_if(foodParticles.begin(), foodParticles.end(),
                           [](const Food &food)
                           { return food.getCapacity() <= 0; }),
            foodParticles.end());
    }

    void FoodManager::update()
    {
        auto &random = AntColony::Utils::RandomGenerator::getInstance();

        // Random chance per cycle to spawn new food (0.4% chance)
        if (random.getInt(0, 3) == 0)
            spawnFood();

        removeDepletedFood();
    }

    void FoodManager::render(const Render::Renderer &renderer) const
    {
        for (const Food &food : foodParticles)
            food.render(renderer);
    }

    std::vector<Food> &FoodManager::getFoodParticles()
    {
        return foodParticles;
    }

}
