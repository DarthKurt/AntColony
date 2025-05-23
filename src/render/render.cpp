#include "render.hpp"
#include "glfw/glfwRenderContext.hpp"

#include <memory>

namespace AntColony::Render
{
    std::unique_ptr<RenderContext> initRenderContext(RENDER_ENGINE engine)
    {
        auto ctx = std::make_unique<GLFW::GLFWRenderContext>();
        ctx->init();
        return ctx;
    }
}
