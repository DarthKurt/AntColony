#ifndef ANTMANAGER_H
#define ANTMANAGER_H

#include <vector>
#include "ant.h"
#include "colony.h"
#include "food.h"

class AntManager
{
public:
    // Constructor
    AntManager();

    // Spawn ants at the colony
    void spawnAnts(const Colony &colony);

    void update(const Colony &, std::vector<Food> &);

    void render(GLFWwindow *window) const;

    // Get the list of ants
    const std::vector<Ant> &getAnts() const;

private:
    // Stores all ants in the simulation
    std::vector<Ant> ants;
    std::vector<Point> generateHexGrid(Point, float, float);
    bool checkAntCollisions(const Point &, float, size_t);
    Food *checkFoodCollisions(const Point &, float, std::vector<Food> &);
    Point calcVelocityTowards(const Point &, const Point &, float);

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
    Point calcRepulsion(const Ant &, size_t);

    void updateAnt(const Colony &, std::vector<Food> &, size_t);

    bool checkCollision(
        const Point &lCenter,
        const Point &rCenter,
        const float &lSize,
        const float &rSize) const;
};

#endif
