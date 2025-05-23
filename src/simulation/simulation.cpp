#include "simulation.hpp"

// Constants for ant behavior and simulation parameters
namespace {
    constexpr auto COLONY_SIZE = 0.05f;
    constexpr auto ANT_SIZE = 0.05f;
}

Simulation::Simulation()
    : colony(Point(0.0f, 0.0f), COLONY_SIZE),
      foodManager(Point(0.0f, 0.0f), COLONY_SIZE),
      antManager()
{
    antManager.spawnAnts(colony, ANT_SIZE);
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