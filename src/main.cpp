#include "simulation/simulation.hpp"
#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    float aspectRatio = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(600, 600, "Ant Colony Simulation", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    srand(static_cast<unsigned>(time(nullptr)));

    Simulation simulation;

    const float targetFrameTimeMs = 1000.0f / 30.0f; // 30 FPS

    while (!glfwWindowShouldClose(window)) {
        auto startTime = std::chrono::high_resolution_clock::now();

        simulation.update();

        glClear(GL_COLOR_BUFFER_BIT);

        const Colony &colony = simulation.getColony();
        colony.render(window);

        const AntManager &antManager = simulation.getAntManager();
        antManager.render(window);

        const FoodManager&foodManager = simulation.getFoodManager();
        foodManager.render(window);

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto endTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        if (frameDuration.count() < targetFrameTimeMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(targetFrameTimeMs - frameDuration.count())));
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}