#include "glfwRenderContext.hpp"
#include "glfwFrameContext.hpp"
#include "glRenderer.hpp"
#include "constants.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <stb_truetype.h>

namespace AntColony::Render::GLFW
{
    GLFWRenderContext::GLFWRenderContext(std::shared_ptr<Core::Logger> logger)
        : logger(logger), isInited(false), window(nullptr), renderer() {}

    void GLFWRenderContext::init()
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

#ifndef NO_DEBUG
        std::ostringstream oss;
        oss << "GLFW Window (Title: '" << title << "', " << windowWidth << "x" << windowHeight << ") initialized";
        logger->debug(oss.str());
#endif

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
        renderer.init();
        isInited = true;
    }

    bool GLFWRenderContext::getInited() const { return isInited; }
    bool GLFWRenderContext::shouldClose() const { return glfwWindowShouldClose(window); }

    void GLFWRenderContext::framebufferSizeCallback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
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

    // bool initFont(const char *fontPath)
    // {
    //     std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    //     if (!file)
    //         return false;

    //     std::streamsize size = file.tellg();
    //     file.seekg(0, std::ios::beg);
    //     std::vector<unsigned char> buffer(size);
    //     if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    //         return false;

    //     stbtt_fontinfo fontInfo;
    //     if (!stbtt_InitFont(&fontInfo, buffer.data(), 0))
    //         return false;

    //     return true;
    // }
}
