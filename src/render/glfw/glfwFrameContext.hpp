#pragma once

#include "../frameContext.hpp"

#include <thread>
#include <chrono>
#include <GLFW/glfw3.h>

namespace AntColony::Render::GLFW
{
    class GLFWFrameContext : public FrameContext
    {
    public:
        GLFWFrameContext(GLFWwindow *window, float frameRate);
        ~GLFWFrameContext() override;

        // FrameContext
        void onBeforeRender() override;
        void onAfterRender() override;

        GLFWwindow *getWindow();

    private:
        GLFWwindow *window;
        float frameRate;
        std::chrono::steady_clock::time_point startTime;
        void controlFrameRate();
    };
}
