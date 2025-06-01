#include "render/glfw/glTextRenderer.hpp"
#include "render/glfw/glShaderProvider.hpp"
#include "core/logger.hpp"
#include "core/point.hpp"
#include "core/color.hpp"
#include "utils/consoleLogger.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>

constexpr auto FRAME_RATE = 1000.0f / 5.0f;

/**
 * @brief Main function to test text rendering with GLRenderer.
 * @return int Program exit code (0 for success, 1 for failure).
 */
int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    // Set OpenGL version to 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Text Rendering Test", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    // Set up viewport
    glViewport(0, 0, 1280, 720);

    // Create logger and shader provider
    auto logger = std::make_shared<AntColony::Utils::ConsoleLogger>();
    auto shaderProvider = std::make_shared<AntColony::Render::GLFW::GLShaderProvider>(logger);

    // Create renderer
    auto renderer = std::make_shared<AntColony::Render::GLFW::GLTextRenderer>(shaderProvider, logger);
    renderer->init();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        const auto startTime = std::chrono::steady_clock::now();

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render text at position (100, 100) with white color
        renderer->drawText(AntColony::Core::Point{100.0f, 100.0f}, "Hello, World!", AntColony::Core::Color(0x7d4827));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();

        auto endTime = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if (frameDuration.count() < FRAME_RATE)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_RATE - frameDuration.count())));
        }
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}