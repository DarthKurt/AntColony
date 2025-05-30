#include "render.hpp"

#include "glfw/glfwRenderContext.hpp"

#include <memory>

namespace AntColony::Render
{
    std::unique_ptr<RenderContext> initRenderContext(RENDER_ENGINE engine, std::shared_ptr<Core::Logger> logger)
    {
        auto ctx = std::make_unique<GLFW::GLFWRenderContext>(logger);
        ctx->init();
        return ctx;
    }
}
