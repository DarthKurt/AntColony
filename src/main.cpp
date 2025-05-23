#include "simulation/simulation.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <iostream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    float aspectRatio = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

GLFWwindow *initUI()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    GLFWwindow *window = glfwCreateWindow(600, 600, "Ant Colony Simulation", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    return window;
}

void render(GLFWwindow *window, const Simulation &simulation)
{
    glClear(GL_COLOR_BUFFER_BIT);

    const auto &colony = simulation.getColony();
    colony.render(window);

    const auto &antManager = simulation.getAntManager();
    antManager.render(window);

    const auto &foodManager = simulation.getFoodManager();
    foodManager.render(window);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void controlFrameRate(const std::chrono::steady_clock::time_point &startTime, const float targetFrameTimeMs)
{
    auto endTime = std::chrono::steady_clock::now();
    auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    if (frameDuration.count() < targetFrameTimeMs)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(targetFrameTimeMs - frameDuration.count())));
    }
}

int main()
{
    auto window = initUI();

    if (!window)
        return -1;

    // Seed randomizer
    srand(static_cast<unsigned>(time(nullptr)));

    Simulation simulation;

    // Set framerate
    const float targetFrameTimeMs = 1000.0f / 30.0f;

    while (!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::steady_clock::now();

        simulation.update();

        render(window, simulation);
        controlFrameRate(startTime, targetFrameTimeMs);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}