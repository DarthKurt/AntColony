#include "simulation/simulation.hpp"
#include "render/render.hpp"
#include "render/renderEngines.hpp"
#include "utils/randomGenerator.hpp"

using AntColony::Render::initRenderContext;
using AntColony::Utils::RandomGenerator;
using AntColony::Simulation::Simulation;

int main()
{
    auto renderCtx = initRenderContext(AntColony::Render::OPENGL);

    if (!renderCtx->getInited())
        return -1;

    // Initialize the random generator with time-based seed
    RandomGenerator::getInstance().seed();

    Simulation simulation;

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
