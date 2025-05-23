#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "colony.h"
#include "foodManager.h"
#include "antManager.hpp"
#include "../render/frameContext.hpp"

class Simulation
{
public:
    Simulation();

    void update(AntColony::Render::FrameContext &ctx);
    void render(AntColony::Render::FrameContext &ctx);

private:
    Colony colony;
    FoodManager foodManager;
    AntManager antManager;
};

#endif