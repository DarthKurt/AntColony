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

        // Generate position using simplified logic
        float x, y;

        // First determine which quadrant to place food in
        auto quadrant = random.getInt(0, 3);
        auto angleMin = quadrant * M_PI_2;
        auto angleMax = angleMin + M_PI_2;

        debug(startTime, "Spawn in quadrant: " + std::to_string(quadrant) +
                             ". Min angle: " + std::to_string(angleMin) +
                             ". Max angle: " + std::to_string(angleMax));

        float angle = random.getFloat(angleMin, angleMax);

        debug(startTime, "Selected angle: " + std::to_string(angle));

        // Use angle to determine max distance in that direction
        float dx = std::cos(angle);
        float dy = std::sin(angle);

        // Fast approximation to find distance to edge
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

        // Generate actual distance
        float distance = random.getFloat(colonyRadius + initialFoodSize, maxDist);
        debug(startTime, "Selected distance: " + std::to_string(distance));

        // Calculate position
        x = colonyCenter.x + distance * dx;
        y = colonyCenter.y + distance * dy;

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
