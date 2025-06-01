#include "render.hpp"

#include "glfw/glRenderContext.hpp"

#include <memory>

namespace AntColony::Render
{
    std::unique_ptr<RenderContext> initRenderContext(RENDER_ENGINE engine, std::shared_ptr<Core::Logger> logger)
    {
        auto ctx = GLFW::GLRenderContext::getInstance(logger);
        ctx->init();
        return ctx;
    }
}
