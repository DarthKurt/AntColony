#include "simulation.hpp"

namespace AntColony::Simulation
{
    // Constants for ant behavior and simulation parameters
    constexpr auto COLONY_SIZE = 0.5f;
    constexpr auto ANT_SIZE = 0.05f;
    constexpr auto FOOD_SIZE = 0.05f;
    constexpr auto PHEROMONE_SIZE = 0.01f;

    constexpr float LEFT_BOUNDARY = -0.95f;
    constexpr float RIGHT_BOUNDARY = 0.95f;

    // Private ctor
    Simulation::Simulation(std::shared_ptr<Core::Logger> logger,
                           Core::ViewPort viewPort,
                           Core::Point colonyCenter,
                           float colonySize,
                           float foodSize,
                           float antSize,
                           float pheromoneSize)
        : logger(logger),
          colony(colonyCenter, colonySize),
          foodManager(colonyCenter,
                      colonySize,
                      foodSize,
                      viewPort),
          antManager(viewPort),
          pheromoneManager(pheromoneSize),
          foodCounter(Core::Point(viewPort.minX + 2 * 0.05f, viewPort.maxY - 2 * 0.05f), 0.1f)

    {
        antManager.spawnAnts(colony, antSize);
    }

    Simulation::Simulation(std::shared_ptr<Core::Logger> logger)
        : Simulation(
              logger,
              Core::ViewPort(LEFT_BOUNDARY, LEFT_BOUNDARY, RIGHT_BOUNDARY, RIGHT_BOUNDARY),
              Core::Point(0.0f, 0.0f),
              COLONY_SIZE,
              FOOD_SIZE,
              ANT_SIZE,
              PHEROMONE_SIZE) {}

    Simulation::Simulation() : Simulation(std::make_shared<Utils::ConsoleLogger>()) {}

    void Simulation::update(const Render::FrameContext &ctx)
    {
        const auto &food = foodManager.getFoodParticles();
        const auto &pheromones = pheromoneManager.getPheromones();

        const auto poisitions = antManager.update(colony, foodCounter, food, pheromones);

        pheromoneManager.update(poisitions);
        foodManager.update();
    }

    void Simulation::render(const Render::FrameContext &ctx) const
    {
        const auto &renderer = *ctx.getRenderer().get();

        colony.render(renderer);
        antManager.render(renderer);
        foodManager.render(renderer);
        pheromoneManager.render(renderer);
        foodCounter.render(renderer);
    }
}
