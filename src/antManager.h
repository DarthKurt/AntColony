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

    void render(GLFWwindow *window);

    // Get the list of ants
    const std::vector<Ant> &getAnts() const;

private:
    // Stores all ants in the simulation
    std::vector<Ant> ants;
    std::vector<Point> generateHexGrid(Point, float, float);
    bool checkAntCollisions(const Point &, float, size_t);
    Food *checkFoodCollisions(const Point &, float, std::vector<Food> &);
    Point calcVelocityTowards(const Point &, const Point &, float);
    Point calcRepulsion(const Ant &, size_t);
    void updateAnt(const Colony &, std::vector<Food> &, size_t);

    bool checkCollision(
        const Point &lCenter,
        const Point &rCenter,
        const float &lSize,
        const float &rSize) const;
};

#endif
