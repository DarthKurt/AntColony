#pragma once

#include "colony.hpp"
#include "foodManager.hpp"
#include "antManager.hpp"
#include "pheromoneManager.hpp"

#include "../render/frameContext.hpp"

namespace AntColony::Simulation
{
    class Simulation
    {
    public:
        Simulation();

        void update(const Render::FrameContext &ctx);
        void render(const Render::FrameContext &ctx) const;

    private:
        Simulation(Core::ViewPort viewPort, Core::Point colonyCenter, float colonySize, float foodSize, float antSize, float pheromoneSize);

        Colony colony;
        FoodManager foodManager;
        AntManager antManager;
        PheromoneManager pheromoneManager;
    };
}