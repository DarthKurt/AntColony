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
        const bool shouldProfile = true;

        // Time the update operations
        const std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
        debug(shouldProfile ? &startTime : nullptr, "Spawn new food");

        auto &random = AntColony::Utils::RandomGenerator::getInstance();
        const auto maxCapacity = random.getInt(1, 3);

        // Pre-calculate viewport distances from colony
        const float distToRight = viewPort.maxX - colonyCenter.x - foodRadius;
        const float distToLeft = colonyCenter.x - viewPort.minX - foodRadius;
        const float distToTop = viewPort.maxY - colonyCenter.y - foodRadius;
        const float distToBottom = colonyCenter.y - viewPort.minY - foodRadius;

        // Generate position using simplified logic
        float x, y;

        // First determine which quadrant to place food in
        auto quadrant = random.getInt(0, 3);
        auto angleMin = quadrant * M_PI_2;
        auto angleMax = angleMin + M_PI_2;

        debug(shouldProfile ? &startTime : nullptr, "Spawn in quadrant: " + std::to_string(quadrant) +
                                                        ". Min angle: " + std::to_string(angleMin) +
                                                        ". Max angle: " + std::to_string(angleMax));

        float angle = random.getFloat(angleMin, angleMax);

        debug(shouldProfile ? &startTime : nullptr, "Selected angle: " + std::to_string(angle));

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
        float distance = random.getFloat(colonyRadius + foodRadius, maxDist);
        debug(shouldProfile ? &startTime : nullptr, "Selected distance: " + std::to_string(distance));

        // Calculate position
        x = colonyCenter.x + distance * dx;
        y = colonyCenter.y + distance * dy;

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
