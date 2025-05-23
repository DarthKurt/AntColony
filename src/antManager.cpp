#include "viewPort.h"
#include "antManager.h"

#include <algorithm>
#include <random>
#include <vector>
#include <cmath>
#include <iostream>

AntManager::AntManager() {}

// Spawning ants evenly
void AntManager::spawnAnts(const Colony &colony)
{
    // Already inited
    if (ants.size() > 0)
        return;

    // Defined size of each ant
    const float antSize = 0.05f;
    const Point colonyPostion = colony.getPosition();
    const float colonySize = colony.getSize();

    // Possible spawn positions
    const std::vector<Point> &positions = generateHexGrid(colonyPostion, colonySize, antSize);

    // Compute number of ants to fit in a straight line along the radius
    const int numAnts = static_cast<int>(std::floor(colonySize / antSize));

    // Ensure we have enough positions
    if (positions.size() < numAnts)
    {
        std::cerr << "Error: Not enough positions to spawn all ants!" << std::endl;
        return;
    }

    // Copy positions for shuffling
    std::vector<Point> shuffledPositions = positions;
    std::random_device rd;

    // Random number generator
    std::mt19937 rng(rd());

    // Shuffle
    std::shuffle(shuffledPositions.begin(), shuffledPositions.end(), rng);

    // Select unique positions for ants
    for (int i = 0; i < numAnts; i++)
    {
        ants.emplace_back(shuffledPositions[i], antSize);
    }
}

// Get list of ants
const std::vector<Ant> &AntManager::getAnts() const
{
    return ants;
}

// Function to arrange hexagonal cells inside a circle
std::vector<Point> AntManager::generateHexGrid(Point center, float radius, float cellSize)
{
    std::vector<Point> positions;

    // Constant for sqrt(3) used in hexagonal grid calculations
    const float sqrt3 = std::sqrt(3.0f);

    // Calculate hexagon side length from cellSize (inradius)
    // Inradius = (sqrt(3) / 2) * side_length, so side_length = (2 * inradius) / sqrt(3)
    const float size = (2.0f * cellSize) / sqrt3;

    // Compute range for axial coordinate q based on circle radius and hexagon size
    // Adjust range to ensure hexagons fit within the circle without excess
    int N_q = static_cast<int>(std::ceil((radius - size) / (size * sqrt3)));
    // Compute range for axial coordinate r
    int N_r = static_cast<int>(std::ceil((radius - size) / (size * 1.5f)));

    // Iterate over axial coordinates (q, r) to generate hexagon centers
    for (int q = -N_q; q <= N_q; ++q)
    {
        for (int r = -N_r; r <= N_r; ++r)
        {
            // Calculate Cartesian x-coordinate for hexagon center
            // Distance between adjacent centers (horizontally) is sqrt(3) * size = 2 * cellSize
            double x = size * sqrt3 * (q + r / 2.0);
            // Calculate Cartesian y-coordinate for hexagon center
            // Distance between rows (vertically) is (3/2) * size = sqrt(3) * cellSize
            double y = size * 1.5 * r;
            // Compute distance from circle center to hexagon center
            double dx = x - center.x;
            double dy = y - center.y;
            double d = std::sqrt(dx * dx + dy * dy);
            // Check if the entire hexagon fits within the circle
            // Distance to vertex (size) plus center distance must be <= radius
            if (d + size <= radius)
            {
                // Add valid hexagon center to positions
                positions.push_back(Point(x, y));
            }
        }
    }

    return positions;
}

bool AntManager::checkAntCollisions(const Point &newPosition, float antSize, size_t currentIndex)
{
    bool hasCollision = false;

    for (size_t i = 0; i < ants.size(); i++)
    {
        if (i == currentIndex)
            continue;

        const Ant &otherAnt = ants[i];

        if (checkCollision(newPosition, otherAnt.getPosition(), antSize, otherAnt.getSize()))
        {
            // Try again if collision detected
            hasCollision = true;
            break;
        }
    }

    return hasCollision;
}

Food *AntManager::checkFoodCollisions(const Point &newPosition, float antSize, std::vector<Food> &food)
{
    for (auto &piece : food)
    {
        if (piece.getCapacity() <= 0)
            continue;

        if (checkCollision(newPosition, piece.getPosition(), antSize, piece.getSize()))
        {
            // Return pointer to collided food
            return &piece;
        }
    }

    // No collision found
    return nullptr;
}

bool AntManager::checkCollision(
    const Point &lCenter,
    const Point &rCenter,
    const float &lSize,
    const float &rSize) const
{
    const float distance = lCenter.distanceTo(rCenter);
    const bool res = distance < (lSize + rSize);

    return res;
}

Point AntManager::calcVelocityTowards(const Point &oldPostion, const Point &newPosition, const float strength)
{
    float dx = newPosition.x - oldPostion.x;
    float dy = newPosition.y - oldPostion.y;

    const float distance = oldPostion.distanceTo(newPosition);

    if (distance > 0.0f)
    {
        dx /= distance;
        dy /= distance;
    }

    return Point(dx * strength, dy * strength);
}

Point AntManager::calcRepulsion(const Ant &ant, size_t currentIndex)
{
    const float collisionCoef = 1.5f;
    const float minSpacing = ant.getSize() * collisionCoef;
    const Point currentPosition = ant.getPosition();
    const Point currentVelocity = ant.getVelocity();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.025f, 0.025f);

    Point velocityComponent(0.0f, 0.0f);
    Point randomComponent(dist(gen), dist(gen));

    for (size_t i = 0; i < ants.size(); i++)
    {
        if (i == currentIndex)
            continue;

        const Ant &otherAnt = ants[i];
        const Point otherPosition = otherAnt.getPosition();

        float dx = otherPosition.x - currentPosition.x;
        float dy = otherPosition.y - currentPosition.y;
        float distance = currentPosition.distanceTo(otherPosition);

        // **Apply repulsion ONLY if ant is getting closer**
        float futureDistance = (currentPosition + currentVelocity).distanceTo(otherPosition);
        if (distance < minSpacing && futureDistance < distance)
        {
            float strength = std::pow((minSpacing - distance) / minSpacing, 2);
            velocityComponent.x -= strength * dx / distance;
            velocityComponent.y -= strength * dy / distance;
        }
    }

    // Blend repulsion based on current velocity
    float velocityMagnitude = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y);
    float velocityInfluence = std::min(1.0f, velocityMagnitude / 0.1f); // Scale impact

    Point totalRepulsion = (velocityComponent * velocityInfluence) + (randomComponent * (1.0f - velocityInfluence));

    // **Normalize repulsion strength**
    float magnitude = std::sqrt(totalRepulsion.x * totalRepulsion.x + totalRepulsion.y * totalRepulsion.y);
    if (magnitude > 0.2f)
    {
        totalRepulsion.x *= 0.1f / magnitude;
        totalRepulsion.y *= 0.1f / magnitude;
    }

    // Fine-tune the scaling
    return totalRepulsion * 0.25f;
}

// Update ant movement
void AntManager::updateAnt(const Colony &colony, std::vector<Food> &food, size_t currentIndex)
{
    const Point colonyPostion = colony.getPosition();
    const float colonySize = colony.getSize();

    Ant &ant = ants[currentIndex];
    const Point currentVelocity = ant.getVelocity();
    const Point currentPosition = ant.getPosition();
    const float antSize = ant.getSize();

    // If carrying food, move toward colony
    if (ant.isBusy())
    {
        const Point newVelocity = calcVelocityTowards(currentPosition, colonyPostion, 0.005f);
        ant.setVelocity(newVelocity);
    }

    if (ant.isMoving())
    {
        // Try moving with current velocity
        const Point newPosition = currentPosition + currentVelocity;
        bool validPosition = !checkAntCollisions(newPosition, antSize, currentIndex) && checkViewportBoundaries(newPosition);

        if (validPosition)
        {
            // check food collision
            Food *collidedFood = checkFoodCollisions(newPosition, antSize, food);
            if (collidedFood && !ant.isBusy())
            {
                // stop
                ant.setVelocity(currentVelocity * 0.0f);

                // Collect food
                ant.biteFood(*collidedFood);
            }

            // check colony collision
            if (checkCollision(newPosition, colonyPostion, antSize, colonySize))
            {
                ant.dropFood();
            }

            ant.setPosition(newPosition);
            return;
        }
    }

    Point newPosition;

    // Try 10 times and stop trying
    for (int attempt = 0; attempt < 10; attempt++)
    {
        bool validPosition = true;

        // Move randomly avoiding ants
        const Point repulsion = calcRepulsion(ant, currentIndex);

        newPosition = currentPosition + repulsion;

        validPosition = !checkAntCollisions(newPosition, antSize, currentIndex) && checkViewportBoundaries(newPosition);

        // Move
        if (validPosition)
        {
            // check food collision
            Food *collidedFood = checkFoodCollisions(newPosition, antSize, food);
            if (collidedFood && !ant.isBusy())
            {
                // stop
                ant.setVelocity(currentVelocity * 0.0f);

                // Collect food
                ant.biteFood(*collidedFood);
            }
            else
            {
                // stop
                ant.setVelocity(repulsion);
            }

            // check colony collision
            if (checkCollision(newPosition, colonyPostion, antSize, colonySize))
            {
                ant.dropFood();
            }

            ant.setPosition(newPosition);
            break;
        }
    }
}

void AntManager::update(const Colony &colony, std::vector<Food> &food)
{
    for (size_t i = 0; i < ants.size(); i++)
    {
        // Pass index instead of filtering in advance
        updateAnt(colony, food, i);
    }
}

void AntManager::render(GLFWwindow *window)
{
    for (Ant &ant : ants)
        ant.render(window);
}
