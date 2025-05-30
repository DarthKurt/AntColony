#pragma once
// MUST Be first to not conflict with GLFW
#include <glad/glad.h>

#include "../../core/logger.hpp"
#include "../renderContext.hpp"
#include "../renderer.hpp"
#include "glRenderer.hpp"

#include <memory>
#include <GLFW/glfw3.h>

namespace AntColony::Render::GLFW
{
    class GLFWRenderContext : public RenderContext
    {
    public:
        GLFWRenderContext(std::shared_ptr<Core::Logger> logger);
        ~GLFWRenderContext() override;

        // RenderContext
        void init() override;
        bool getInited() const override;
        bool shouldClose() const override;
        std::unique_ptr<FrameContext> getFrameContext() const override;

    private:
        std::shared_ptr<Core::Logger> logger;
        bool isInited;
        GLFWwindow *window;
        GLRenderer renderer;

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    };
}
