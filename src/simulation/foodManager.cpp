#include "foodManager.h"
#include "../utils/viewPort.h"

#include <algorithm>

FoodManager::FoodManager(Point center, float size) : size(size) {}

void FoodManager::spawnFood()
{
    const float foodSize = 0.05f;
    const int maxCapacity = 5;
    Point foodPosition;

    float x, y;
    x = getCoordInBoundaries(size, foodSize);
    y = getCoordInBoundaries(size, foodSize);

    foodPosition = Point(x, y);
    foodParticles.emplace_back(foodPosition, foodSize, maxCapacity);
}

float FoodManager::getCoordInBoundaries(float innerSpot, float itemSize)
{
    // Decide randomly whether to spawn food on the left or right side
    if (rand() % 2 == 0)
    {
        return LEFT_BOUNDARY + itemSize + ((static_cast<float>(rand()) / RAND_MAX) * (innerSpot - itemSize));
    }
    else
    {
        return innerSpot + itemSize + ((static_cast<float>(rand()) / RAND_MAX) * (RIGHT_BOUNDARY - itemSize - innerSpot));
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
    // Random chance per cycle to spawn new food
    if (rand() % 250 == 0)
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
