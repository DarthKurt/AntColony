#include "simulation.hpp"
#include "../render/frameContext.hpp"
#include "../render/glfw/glfwFrameContext.hpp"

// Constants for ant behavior and simulation parameters
namespace
{
    constexpr auto COLONY_SIZE = 0.5f;
    constexpr auto ANT_SIZE = 0.05f;
}

Simulation::Simulation()
    : colony(Point(0.0f, 0.0f), COLONY_SIZE),
      foodManager(Point(0.0f, 0.0f), COLONY_SIZE),
      antManager()
{
    antManager.spawnAnts(colony, ANT_SIZE);
}

void Simulation::update(AntColony::Render::FrameContext &ctx)
{
    std::vector<Food> &food = foodManager.getFoodParticles();
    antManager.update(colony, food);
    foodManager.update();
}

void Simulation::render(AntColony::Render::FrameContext &ctx) const
{
    // TODO: get rid of GLFWFrameContext
    auto &glfwFrameContext = static_cast<AntColony::Render::GLFW::GLFWFrameContext&>(ctx);
    auto *window = glfwFrameContext.getWindow();

    colony.render(window);
    antManager.render(window);
    foodManager.render(window);
}
