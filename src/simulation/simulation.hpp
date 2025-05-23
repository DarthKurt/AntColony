#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "colony.h"
#include "foodManager.h"
#include "antManager.hpp"

class Simulation
{
public:
    Simulation();

    void update();

    const Colony &getColony() const;
    const FoodManager &getFoodManager() const;
    const AntManager &getAntManager() const;

private:
    Colony colony;
    FoodManager foodManager;
    AntManager antManager;
};

#endif