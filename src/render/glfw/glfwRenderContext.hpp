#pragma once

#include "../renderContext.hpp"
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
        std::unique_ptr<FrameContext> getFrameContext() override;

    private:
        GLFWwindow *window;

        static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
    };
}
