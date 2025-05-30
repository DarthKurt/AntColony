#include "glfwRenderContext.hpp"
#include "glfwFrameContext.hpp"
#include "glfwRenderer.hpp"
#include "constants.hpp"

#include <iostream>
#include <sstream>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace AntColony::Render::GLFW
{
    GLFWRenderContext::GLFWRenderContext(std::shared_ptr<Core::Logger> logger)
        : logger(logger), isInited(false), window(nullptr), renderer(nullptr) {}

    void GLFWRenderContext::init()
    {
        if (!glfwInit())
        {
            logger->error("Failed to initialize GLFW");
            window = nullptr;
        }

        logger->debug("GLFW initialized");

        const auto *title = "Ant Colony Simulation";
        const auto windowWidth = 1280;
        const auto windowHeight = 720;

        GLFWwindow *w = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
        if (!w)
        {
            logger->error("Failed to create GLFW window");
            glfwTerminate();
        }

#ifndef NO_DEBUG
        std::ostringstream oss;
        oss << "GLFW Window (Title: '" << title << "', " << windowWidth << "x" << windowHeight << ") initialized";
        logger->debug(oss.str());
#endif

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            logger->error("Failed to initialize FreeType");
            glfwTerminate();
            window = nullptr;
        }
        logger->debug("FreeType initialized successfully");
        FT_Done_FreeType(ft);

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
