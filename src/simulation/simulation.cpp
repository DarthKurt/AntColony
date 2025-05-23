#include "simulation.hpp"

Simulation::Simulation()
    : colony(Point(0.0f, 0.0f), 0.5f),
      foodManager(Point(0.0f, 0.0f), 0.5f),
      antManager()
{
    antManager.spawnAnts(colony);
}

void Simulation::update()
{
    std::vector<Food> &food = foodManager.getFoodParticles();
    antManager.update(colony, food);
    foodManager.update();
}

const Colony &Simulation::getColony() const { return colony; }
const FoodManager &Simulation::getFoodManager() const { return foodManager; }
const AntManager &Simulation::getAntManager() const { return antManager; }