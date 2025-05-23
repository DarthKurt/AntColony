#pragma once

#include "../renderContext.hpp"
#include "../renderer.hpp"
#include "glfwRenderer.hpp"

#include <GLFW/glfw3.h>
#include <memory>

namespace AntColony::Render::GLFW
{
    class GLFWRenderContext : public RenderContext
    {
    public:
        GLFWRenderContext();
        ~GLFWRenderContext() override;

        // RenderContext
        void init() override;
        bool getInited() const override;
        bool shouldClose() const override;
        std::unique_ptr<FrameContext> getFrameContext() const override;

    private:
        bool isInited;
        GLFWwindow *window;
        GLFWRenderer renderer;

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    };
}
