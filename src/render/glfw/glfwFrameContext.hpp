#pragma once

#include "_fwd.hpp"

#include "../frameContext.hpp"

#include <memory>
#include "_gl.hpp"
#include <thread>
#include <chrono>

namespace AntColony::Render::GLFW
{
    class GLFWFrameContext : public FrameContext
    {
    public:
        GLFWFrameContext(GLFWwindow *window, float frameRate, const std::shared_ptr<GLRenderer> renderer);
        ~GLFWFrameContext() override;

        // FrameContext
        void onBeforeRender() override;
        void onAfterRender() override;
        const std::shared_ptr<Renderer> getRenderer() const override;

    private:
        GLFWwindow *window;
        const std::shared_ptr<GLRenderer> renderer;
        float frameRate;
        std::chrono::steady_clock::time_point startTime;
        void controlFrameRate();
    };
}
