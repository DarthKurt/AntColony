#include "glfwRenderContext.hpp"
#include "glfwFrameContext.hpp"
#include "glfwRenderer.hpp"
#include "constants.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

namespace AntColony::Render::GLFW
{
    GLFWRenderContext::GLFWRenderContext() : isInited(false), window(nullptr), renderer(nullptr) {}

    void GLFWRenderContext::init()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            window = nullptr;
        }

        GLFWwindow *w = glfwCreateWindow(600, 600, "Ant Colony Simulation", NULL, NULL);
        if (!w)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(w);
        glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
        window = w;
        renderer = GLFWRenderer(window);
        isInited = true;
    }

    bool GLFWRenderContext::getInited() const { return isInited; }
    bool GLFWRenderContext::shouldClose() const { return glfwWindowShouldClose(window); }

    void GLFWRenderContext::framebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        float aspectRatio = (float)width / (float)height;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
    }

    std::unique_ptr<FrameContext> GLFWRenderContext::getFrameContext() const
    {
        return std::make_unique<GLFWFrameContext>(window, FRAME_RATE, renderer);
    }

    GLFWRenderContext::~GLFWRenderContext()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
