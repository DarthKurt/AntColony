#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include "food.h"

#include <GLFW/glfw3.h>
#include <vector>

class Ant : public Entity
{
public:
    Ant(Point position, float size);
    Ant(Point position, Point velocity, float size);

    void render(GLFWwindow *window);

    void biteFood(Food &food);
    void dropFood();

    void setPosition(Point position);
    void setVelocity(Point velocity);
    Point getVelocity() const;

    bool isBusy() const;
    bool isMoving() const;

private:
    /**
     * @brief Computes a repulsion force to react on collisions with nearby entities.
     *
     * This function analyzes all ants in the given vector and determines if the
     * current ant is too close to others. If the distance between ants falls below
     * the collision threshold, a repulsion force is calculated to push the ant away.
     * The force is scaled based on proximity, ensuring a smooth and realistic movement.
     *
     * @param ants Vector containing all ants in the simulation.
     * @param currentIndex Index of the Ant in the provided vector.
     * @return Point representing the computed repulsion force to adjust the ant's movement.
     */
    // Point getRepulsion(const std::vector<Ant> &ants, size_t currentIndex);

    // Stores previous movement direction
    Point velocity;
    bool carryFood;
};

#endif
