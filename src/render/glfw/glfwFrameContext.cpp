#include "../renderer.hpp"
#include "glRenderer.hpp"
#include "glfwFrameContext.hpp"

#include <thread>
#include <chrono>

namespace AntColony::Render::GLFW
{
    GLFWFrameContext::GLFWFrameContext(GLFWwindow *window, float frameRate, const GLRenderer &renderer)
        : window(window), renderer(renderer), frameRate(frameRate), startTime(std::chrono::steady_clock::now())
    {
    }

    void GLFWFrameContext::onBeforeRender()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLFWFrameContext::onAfterRender()
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    const Renderer &GLFWFrameContext::getRenderer() const { return renderer; }

    GLFWFrameContext::~GLFWFrameContext()
    {
        controlFrameRate();
    }

    void GLFWFrameContext::controlFrameRate()
    {
        auto endTime = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if (frameDuration.count() < frameRate)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(frameRate - frameDuration.count())));
        }
    }
}
