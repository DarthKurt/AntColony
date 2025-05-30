#include "simulation.hpp"
#include "../render/frameContext.hpp"

namespace AntColony::Simulation
{
    // Constants for ant behavior and simulation parameters
    constexpr auto COLONY_SIZE = 0.5f;
    constexpr auto ANT_SIZE = 0.05f;
    constexpr auto FOOD_SIZE = 0.05f;
    constexpr auto PHEROMONE_SIZE = 0.01f;

    const float LEFT_BOUNDARY = -0.95f;
    const float RIGHT_BOUNDARY = 0.95f;

    Simulation::Simulation()
        : Simulation(
              Core::ViewPort(LEFT_BOUNDARY, LEFT_BOUNDARY, RIGHT_BOUNDARY, RIGHT_BOUNDARY),
              Core::Point(0.0f, 0.0f),
              COLONY_SIZE,
              FOOD_SIZE,
              ANT_SIZE,
              PHEROMONE_SIZE) {}

    Simulation::Simulation(Core::ViewPort viewPort, Core::Point colonyCenter, float colonySize, float foodSize, float antSize, float pheromoneSize)
        : colony(colonyCenter, colonySize),
          foodManager(colonyCenter,
                      colonySize,
                      foodSize,
                      viewPort),
          antManager(viewPort),
          pheromoneManager(pheromoneSize)
    {
        antManager.spawnAnts(colony, antSize);
    }

    void Simulation::update(const Render::FrameContext &ctx)
    {
        const auto &food = foodManager.getFoodParticles();
        const auto &pheromones = pheromoneManager.getPheromones();
        const auto poisitions = antManager.update(colony, food, pheromones);
        pheromoneManager.update(poisitions);
        foodManager.update();
    }

    void Simulation::render(const Render::FrameContext &ctx) const
    {
        auto &renderer = ctx.getRenderer();

        colony.render(renderer);
        antManager.render(renderer);
        foodManager.render(renderer);
        pheromoneManager.render(renderer);
    }
}
