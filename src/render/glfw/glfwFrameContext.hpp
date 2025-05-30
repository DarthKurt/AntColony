#pragma once

#include "../frameContext.hpp"
#include "../renderer.hpp"

#include "glfwRenderer.hpp"

#include <thread>
#include <chrono>
#include <GLFW/glfw3.h>

namespace AntColony::Render::GLFW
{
    class GLFWFrameContext : public FrameContext
    {
    public:
        GLFWFrameContext(GLFWwindow *window, float frameRate, const GLFWRenderer &renderer);
        ~GLFWFrameContext() override;

        // FrameContext
        void onBeforeRender() override;
        void onAfterRender() override;
        const Renderer &getRenderer() const override;

    private:
        GLFWwindow *window;
        const GLFWRenderer &renderer;
        float frameRate;
        std::chrono::steady_clock::time_point startTime;
        void controlFrameRate();
    };
}
