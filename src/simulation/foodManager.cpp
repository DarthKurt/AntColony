#include "foodManager.hpp"
#include "../utils/viewPort.hpp"
#include "../utils/randomGenerator.hpp"

#include <algorithm>

FoodManager::FoodManager(Point colonyCenter, float colonyRadius) : colonyRadius(colonyRadius) {}

void FoodManager::spawnFood()
{
    const float foodSize = 0.05f;
    const int maxCapacity = 1;
    Point foodPosition;

    float x, y;
    x = getCoordInBoundaries(colonyRadius, foodSize);
    y = getCoordInBoundaries(colonyRadius, foodSize);

    foodPosition = Point(x, y);
    foodParticles.emplace_back(foodPosition, foodSize, maxCapacity);
}

float FoodManager::getCoordInBoundaries(float innerSpot, float itemSize)
{
    auto& random = AntColony::Utils::RandomGenerator::getInstance();
    
    // Decide randomly whether to spawn food on the left or right side
    if (random.getBool())
    {
        return LEFT_BOUNDARY + itemSize + random.getFloat(0.0f, 1.0f) * (innerSpot - itemSize);
    }
    else
    {
        return innerSpot + itemSize + random.getFloat(0.0f, 1.0f) * (RIGHT_BOUNDARY - itemSize - innerSpot);
    }
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
    auto& random = AntColony::Utils::RandomGenerator::getInstance();
    
    // Random chance per cycle to spawn new food (0.4% chance)
    if (random.getInt(0, 249) == 0)
        spawnFood();

    removeDepletedFood();
}

void FoodManager::render(GLFWwindow *window) const
{
    for (const Food &food : foodParticles)
        food.render(window);
}

std::vector<Food> &FoodManager::getFoodParticles()
{
    return foodParticles;
}
