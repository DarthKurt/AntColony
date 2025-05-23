#include "foodManager.hpp"
#include "../utils/consoleLogger.hpp"
#include "../utils/randomGenerator.hpp"

#include <algorithm>
#include <cmath>

namespace AntColony::Simulation
{
    FoodManager::FoodManager(Core::Point colonyCenter, float colonyRadius, float foodRadius, Core::ViewPort viewPort)
        : BaseEntityManager(std::make_shared<Utils::ConsoleLogger>()), colonyCenter(colonyCenter), colonyRadius(colonyRadius), foodRadius(foodRadius), viewPort(viewPort) {}

    void FoodManager::spawnFood()
    {

#ifndef NO_DEBUG
        // Time the update operations
        const std::chrono::steady_clock::time_point tp = std::chrono::high_resolution_clock::now();
        const auto *startTime = &tp;
#else
        const auto *startTime = nullptr;
#endif

        debug(startTime, "Spawn new food");

        auto &random = AntColony::Utils::RandomGenerator::getInstance();
        const auto maxCapacity = random.getInt(1, 3);

        // Pre-calculate viewport distances from colony
        const float initialFoodSize = foodRadius * maxCapacity;
        const float distToRight = viewPort.maxX - colonyCenter.x - initialFoodSize;
        const float distToLeft = colonyCenter.x - viewPort.minX - initialFoodSize;
        const float distToTop = viewPort.maxY - colonyCenter.y - initialFoodSize;
        const float distToBottom = colonyCenter.y - viewPort.minY - initialFoodSize;

        // Use a full 0-2π angle for uniform angular distribution
        float angle = random.getFloat(0, 2 * M_PI);

        debug(startTime, "Selected angle: " + std::to_string(angle));

        // Calculate direction vector
        float dx = std::cos(angle);
        float dy = std::sin(angle);

        // Find maximum distance in this direction
        float maxDist;
        if (dx > 0)
        {
            maxDist = (dy > 0)
                        ? std::min(distToRight / dx, distToTop / dy)
                        : std::min(distToRight / dx, distToBottom / -dy);
        }
        else
        {
            maxDist = (dy > 0)
                        ? std::min(distToLeft / -dx, distToTop / dy)
                        : std::min(distToLeft / -dx, distToBottom / -dy);
        }

        // For uniform spatial distribution, we need to sample based on area
        float minRSquared = (colonyRadius + initialFoodSize) * (colonyRadius + initialFoodSize);
        float maxRSquared = maxDist * maxDist;
        
        // Sample from r² distribution for uniform area coverage
        float rSquared = random.getFloat(minRSquared, maxRSquared);
        float distance = std::sqrt(rSquared);
        
        debug(startTime, "Selected distance: " + std::to_string(distance));

        // Calculate position
        float x = colonyCenter.x + distance * dx;
        float y = colonyCenter.y + distance * dy;

        auto food = std::make_unique<Food>(
            Core::Point(x, y),
            foodRadius,
            maxCapacity,
            [this](Food *f)
            { this->foodParticles.erase(f); });

        // Store in map with pointer as key
        foodParticles[food.get()] = std::move(food);
    }

    void FoodManager::update()
    {
        auto &random = AntColony::Utils::RandomGenerator::getInstance();

        // Random chance per cycle to spawn new food (0.4% chance)
        if (random.getInt(0, 249) == 0)
            spawnFood();
    }

    void FoodManager::render(const Render::Renderer &renderer) const
    {
        for (const auto &[ptr, food] : foodParticles)
        {
            ptr->render(renderer);
        }
    }

    std::vector<Food *> FoodManager::getFoodParticles() const
    {
        std::vector<Food *> result;
        result.reserve(foodParticles.size());

        for (const auto &[ptr, food] : foodParticles)
        {
            result.emplace_back(ptr);
        }

        return result;
    }

}
