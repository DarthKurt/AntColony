#include "pheromoneSignal.hpp"

namespace AntColony::Simulation
{
    PheromoneSignal::PheromoneSignal(Core::Point position, int excitement)
        : position(position), excitement(excitement)
    {
    }
}