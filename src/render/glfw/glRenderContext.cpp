#include "glRenderer.hpp"
#include "glRenderContext.hpp"
#include "glShaderProvider.hpp"
#include "glfwFrameContext.hpp"
#include "constants.hpp"

#include "../../core/logger.hpp"

#include <string>

#include <vector>
#include <iostream>
#include <sstream>

namespace AntColony::Render::GLFW
{

    std::unique_ptr<RenderContext> GLRenderContext::getInstance(const std::shared_ptr<Core::Logger> logger)
    {
        return std::make_unique<GLRenderContext>(GLRenderContext(logger));
    }

    GLRenderContext::GLRenderContext(const std::shared_ptr<Core::Logger> logger)
        : logger(logger),
          isInited(false),
          window(nullptr),
          renderer(std::make_shared<GLRenderer>(std::make_shared<GLShaderProvider>(logger), logger)) {}

    void GLRenderContext::init()
    {
        if (!glfwInit())
        {
            logger->error("Failed to initialize GLFW");
            window = nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        logger->debug("GLFW initialized");

        const auto *title = "Ant Colony Simulation";
        const auto windowWidth = 1280;
        const auto windowHeight = 720;

        auto *w = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
        if (!w)
        {
            logger->error("Failed to create GLFW window");
            glfwTerminate();
        }

        logger->debug("GLFW Window (Title: '" +
                      std::string(title) + "', " +
                      std::to_string(windowWidth) + "x" +
                      std::to_string(windowHeight) +
                      ") initialized");

        // Make the window's context current
        glfwMakeContextCurrent(w);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            logger->error("Failed to initialize GLAD");
            glfwTerminate();
            window = nullptr;
            return;
        }

        // Set viewport
        glViewport(0, 0, windowWidth, windowHeight);

        // Set callback for window resize
        glfwSetFramebufferSizeCallback(w, framebufferSizeCallback);
        window = w;
        renderer->init();
        isInited = true;
    }

    bool GLRenderContext::getInited() const { return isInited; }
    bool GLRenderContext::shouldClose() const { return glfwWindowShouldClose(window); }

    void GLRenderContext::framebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    std::unique_ptr<FrameContext> GLRenderContext::getFrameContext() const
    {
        return std::make_unique<GLFWFrameContext>(window, FRAME_RATE, renderer);
    }

    GLRenderContext::~GLRenderContext()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
