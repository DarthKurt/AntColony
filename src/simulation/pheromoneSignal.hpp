#pragma once

#include "../core/point.hpp"

namespace AntColony::Simulation
{
    struct PheromoneSignal
    {
        PheromoneSignal(Core::Point position, int excitement);
        
        const Core::Point position;
        const int excitement;
    };
}