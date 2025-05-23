#include "colony.h"
#include "foodManager.h"
#include "antManager.h"

#include <GLFW/glfw3.h>
#include <iostream>

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    float aspectRatio = (float)width / (float)height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

// Update all entities
void update(Colony &colony, FoodManager &foodManager, AntManager &antManager)
{
    std::vector<Food> &food = foodManager.getFoodParticles();
    antManager.update(colony, food);
    foodManager.update();
}

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ant Colony Simulation", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set OpenGL context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Seed the random number generator
    srand(time(nullptr));

    // Simulation constants
    const float targetFrameTime = 1.0f / 30.0f; // Limit to 15 FPS

    // Create a colony at the center of the screen with 50 ants
    Colony colony(Point(0.0f, 0.0f), 0.5f);
    FoodManager food(Point(0.0f, 0.0f), 0.5f);
    AntManager antManager;

    antManager.spawnAnts(colony);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float startTime = glfwGetTime();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update food manager
        update(colony, food, antManager);

        // Render the colony (and ants inside it)
        colony.render(window);

        antManager.render(window);

        // Render food
        food.render(window);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Control speed
        while (glfwGetTime() - startTime < targetFrameTime)
            ;
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
