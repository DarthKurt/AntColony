#include "simulation.hpp"
#include "../render/frameContext.hpp"

namespace AntColony::Simulation
{
    // Constants for ant behavior and simulation parameters
    constexpr auto COLONY_SIZE = 0.5f;
    constexpr auto ANT_SIZE = 0.05f;
    constexpr auto FOOD_SIZE = 0.05f;

    const float LEFT_BOUNDARY = -0.95f;
    const float RIGHT_BOUNDARY = 0.95f;

    Simulation::Simulation()
        : Simulation(
              Core::ViewPort(LEFT_BOUNDARY, LEFT_BOUNDARY, RIGHT_BOUNDARY, RIGHT_BOUNDARY),
              Core::Point(0.0f, 0.0f),
              COLONY_SIZE,
              FOOD_SIZE,
              ANT_SIZE) {}

    Simulation::Simulation(Core::ViewPort viewPort, Core::Point colonyCenter, float colonySize, float foodSize, float antSize)
        : colony(colonyCenter, colonySize), foodManager(colonyCenter, colonySize, foodSize, viewPort), antManager(viewPort)
    {
        antManager.spawnAnts(colony, antSize);
    }

    void Simulation::update(Render::FrameContext &ctx)
    {
        std::vector<Food> &food = foodManager.getFoodParticles();
        antManager.update(colony, food);
        foodManager.update();
    }

    void Simulation::render(const Render::FrameContext &ctx) const
    {
        auto &render = ctx.getRenderer();

        colony.render(render);
        antManager.render(render);
        foodManager.render(render);
    }
}
