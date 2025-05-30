#include "simulation/simulation.hpp"
#include "render/render.hpp"
#include "render/renderEngines.hpp"
#include "utils/randomGenerator.hpp"
#include "utils/consoleLogger.hpp"

#include "memory"

int main()
{
    const auto logger = std::make_shared<AntColony::Utils::ConsoleLogger>();
    auto renderCtx = AntColony::Render::initRenderContext(AntColony::Render::OPENGL, logger);

    if (!renderCtx->getInited())
        return -1;

    // Initialize the random generator with time-based seed
    AntColony::Utils::RandomGenerator::getInstance().seed();

    AntColony::Simulation::Simulation simulation(logger);

    while (!renderCtx->shouldClose())
    {
        auto frameCtx = renderCtx->getFrameContext();

        frameCtx->onBeforeRender();

        simulation.update(*frameCtx);
        simulation.render(*frameCtx);

        frameCtx->onAfterRender();
    }

    return 0;
}
