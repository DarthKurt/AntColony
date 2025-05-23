#include "simulation.hpp"
#include "../render/frameContext.hpp"
#include "../render/glfw/glfwFrameContext.hpp"

namespace AntColony::Simulation
{
    // Constants for ant behavior and simulation parameters
    constexpr auto COLONY_SIZE = 0.5f;
    constexpr auto ANT_SIZE = 0.05f;

    Simulation::Simulation()
        : colony(Core::Point(0.0f, 0.0f), COLONY_SIZE),
          foodManager(Core::Point(0.0f, 0.0f), COLONY_SIZE),
          antManager()
    {
        antManager.spawnAnts(colony, ANT_SIZE);
    }

    void Simulation::update(Render::FrameContext &ctx)
    {
        std::vector<Food> &food = foodManager.getFoodParticles();
        antManager.update(colony, food);
        foodManager.update();
    }

    void Simulation::render(Render::FrameContext &ctx) const
    {
        // TODO: get rid of GLFWFrameContext
        auto &glfwFrameContext = static_cast<Render::GLFW::GLFWFrameContext &>(ctx);
        auto *window = glfwFrameContext.getWindow();

        colony.render(window);
        antManager.render(window);
        foodManager.render(window);
    }
}
