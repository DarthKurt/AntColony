#include "simulation/simulation.hpp"
#include "render/render.hpp"
#include "render/renderEngines.hpp"

int main()
{
    auto renderCtx = AntColony::Render::initRenderContext(AntColony::Render::OPENGL);

    if (!renderCtx->getInited())
        return -1;

    // Seed randomizer
    srand(static_cast<unsigned>(time(nullptr)));

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
