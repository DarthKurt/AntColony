#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "colony.hpp"
#include "foodManager.hpp"
#include "antManager.hpp"
#include "../render/frameContext.hpp"

class Simulation
{
public:
    Simulation();

    void update(AntColony::Render::FrameContext &ctx);
    void render(AntColony::Render::FrameContext &ctx) const;

private:
    Colony colony;
    FoodManager foodManager;
    AntManager antManager;
};

#endif