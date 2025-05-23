#ifndef FOOD_MANAGER_H
#define FOOD_MANAGER_H

#include "food.h"
#include <vector>

class FoodManager
{
public:
    FoodManager(Point colonyCenter, float colonyRadius);

    void update();
    void render(GLFWwindow *window);

    std::vector<Food>& getFoodParticles();

private:
    float size;
    void spawnFood();
    void removeDepletedFood();
    float getCoordInBoundaries(float innerSpot, float itemSize);
    
    std::vector<Food> foodParticles;
};

#endif
