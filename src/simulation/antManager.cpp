#include "../utils/consoleLogger.hpp"
#include "../utils/randomGenerator.hpp"

#include "antManager.hpp"

#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>

namespace AntColony::Simulation
{
    // Constants for ant behavior and simulation parameters
    constexpr auto COLLISION_COEF = 1.5f;
    constexpr auto COLONY_TARGET_ATTRACTION = 0.005f;
    constexpr auto RANDOM_MOVEMENT = 0.025f;
    constexpr auto MAX_REPULSION_MAGNITUDE = 0.2f;
    constexpr auto NORMALIZED_REPULSION_STRENGTH = 0.1f;
    constexpr auto REPULSION_SCALING = 0.25f;
    constexpr auto VELOCITY_SCALING_THRESHOLD = 0.1f;
    constexpr auto MAX_POSITION_ATTEMPTS = 10;

    AntManager::AntManager(Core::ViewPort viewPort)
        : AntManager(std::make_shared<Utils::ConsoleLogger>(), viewPort) {}

    AntManager::AntManager(std::shared_ptr<Core::Logger> logger, Core::ViewPort viewPort)
        : BaseEntityManager(logger), viewPort(viewPort) {}

    void AntManager::spawnAnts(const Colony &colony, const float antSize)
    {
        // Skip if ants are already initialized
        if (!ants.empty())
            return;

        logger->debug("Spawning ants in colony");

        const auto colonyPosition = colony.getPosition();
        const auto colonySize = colony.getSize();

        logger->debug("Colony size: " + std::to_string(colonySize));
        logger->debug("Ant size: " + std::to_string(antSize));

        // Generate possible spawn positions using a hexagonal grid
        const auto &positions = generateHexGrid(colonyPosition, colonySize, antSize);

        // Calculate number of ants based on colony size
        const auto numAnts = static_cast<int>(std::floor(colonySize / antSize));

        logger->debug("Placing " + std::to_string(numAnts) + " ant(s) in colony space");

        // Validate we have enough positions for all ants
        if (positions.size() < numAnts)
        {
            logger->error("Not enough positions to spawn all ants! Available: " +
                          std::to_string(positions.size()) + ", Required: " +
                          std::to_string(numAnts));
            return;
        }

        // Randomly distribute ants across available positions
        std::vector<Core::Point> shuffledPositions = positions;

        // Use the RandomGenerator to shuffle the positions
        auto &random = Utils::RandomGenerator::getInstance();

        // Fisher-Yates shuffle algorithm
        for (size_t i = shuffledPositions.size() - 1; i > 0; --i)
        {
            size_t j = random.getInt(0, i);
            std::swap(shuffledPositions[i], shuffledPositions[j]);
        }

        // Create ants at the selected positions
        ants.reserve(numAnts);
        for (auto i = 0; i < numAnts; i++)
        {
            ants.emplace_back(shuffledPositions[i], antSize);
        }
    }

    std::vector<Core::Point> AntManager::generateHexGrid(Core::Point center, float radius, float cellSize)
    {
        std::vector<Core::Point> positions;

        // Constant for sqrt(3) used in hexagonal grid calculations
        const auto sqrt3 = std::sqrt(3.0f);

        // Calculate hexagon side length from cellSize (inradius)
        // Inradius = (sqrt(3) / 2) * side_length, so side_length = (2 * inradius) / sqrt(3)
        const auto hexSideLength = (2.0f * cellSize) / sqrt3;

        // Compute range for axial coordinate q based on circle radius and hexagon size
        // Adjust range to ensure hexagons fit within the circle without excess
        auto gridWidth = static_cast<int>(std::ceil((radius - hexSideLength) / (hexSideLength * sqrt3)));
        // Compute range for axial coordinate r
        auto gridHeight = static_cast<int>(std::ceil((radius - hexSideLength) / (hexSideLength * 1.5f)));

        // Iterate over axial coordinates (q, r) to generate hexagon centers
        for (auto q = -gridWidth; q <= gridWidth; ++q)
        {
            for (auto r = -gridHeight; r <= gridHeight; ++r)
            {
                // Calculate Cartesian x-coordinate for hexagon center
                // Distance between adjacent centers (horizontally) is sqrt(3) * hexSideLength = 2 * cellSize
                auto x = hexSideLength * sqrt3 * (q + r / 2.0);
                // Calculate Cartesian y-coordinate for hexagon center
                // Distance between rows (vertically) is (3/2) * hexSideLength = sqrt(3) * cellSize
                auto y = hexSideLength * 1.5 * r;
                // Compute distance from circle center to hexagon center
                auto dx = x - center.x;
                auto dy = y - center.y;
                auto distance = std::sqrt(dx * dx + dy * dy);
                // Check if the entire hexagon fits within the circle
                // Distance to vertex (hexSideLength) plus center distance must be <= radius
                if (distance + hexSideLength <= radius)
                {
                    // Add valid hexagon center to positions
                    positions.push_back(Core::Point(x, y));
                }
            }
        }

        return positions;
    }

    bool AntManager::checkAntCollisions(const Core::Point &newPosition, float antSize, size_t currentIndex, const std::vector<Ant> &ants)
    {
        auto hasCollision = false;

        for (auto i = 0; i < ants.size(); i++)
        {
            if (i == currentIndex)
                continue;

            const auto &otherAnt = ants[i];

            if (checkCollision(newPosition, otherAnt.getPosition(), antSize, otherAnt.getSize()))
            {
                // Try again if collision detected
                hasCollision = true;
                break;
            }
        }

        return hasCollision;
    }

    Food *AntManager::checkFoodCollisions(const Core::Point &newPosition, float antSize, std::vector<Food> &food)
    {
        for (auto &piece : food)
        {
            if (piece.getCapacity() <= 0)
                continue;

            if (checkCollision(newPosition, piece.getPosition(), antSize, piece.getSize()))
            {
                // Return Core::Pointer to collided food
                return &piece;
            }
        }

        // No collision found
        return nullptr;
    }

    bool AntManager::checkCollision(
        const Core::Point &lCenter,
        const Core::Point &rCenter,
        const float &lSize,
        const float &rSize)
    {
        const float distance = lCenter.distanceTo(rCenter);
        return distance < (lSize + rSize);
    }

    Core::Point AntManager::calcVelocityTowards(const Core::Point &oldPosition, const Core::Point &newPosition, const float strength)
    {
        auto dx = newPosition.x - oldPosition.x;
        auto dy = newPosition.y - oldPosition.y;

        const auto distance = oldPosition.distanceTo(newPosition);

        if (distance > 0.0f)
        {
            dx /= distance;
            dy /= distance;
        }

        return Core::Point(dx * strength, dy * strength);
    }

    Core::Point AntManager::calcRepulsion(const Ant &ant, size_t currentIndex) const
    {
        const auto minSpacing = ant.getSize() * COLLISION_COEF;
        const auto currentPosition = ant.getPosition();
        const auto currentVelocity = ant.getVelocity();

        // Use RandomGenerator singleton instead of static random device
        auto &random = Utils::RandomGenerator::getInstance();

        // Initialize velocity and random components
        Core::Point velocityComponent(0.0f, 0.0f);
        Core::Point randomComponent(
            random.getFloat(-RANDOM_MOVEMENT, RANDOM_MOVEMENT),
            random.getFloat(-RANDOM_MOVEMENT, RANDOM_MOVEMENT));

        // Calculate repulsion from each other ant
        for (auto i = 0; i < ants.size(); i++)
        {
            if (i == currentIndex)
                continue;

            const auto &otherAnt = ants[i];
            const auto otherPosition = otherAnt.getPosition();

            auto dx = otherPosition.x - currentPosition.x;
            auto dy = otherPosition.y - currentPosition.y;
            auto distance = currentPosition.distanceTo(otherPosition);

            // Apply repulsion ONLY if ant is getting closer to another ant
            auto futureDistance = (currentPosition + currentVelocity).distanceTo(otherPosition);
            if (distance < minSpacing && futureDistance < distance)
            {
                auto strength = std::pow((minSpacing - distance) / minSpacing, 2);
                velocityComponent.x -= strength * dx / distance;
                velocityComponent.y -= strength * dy / distance;
            }
        }

        // Blend repulsion based on current velocity magnitude
        auto velocityMagnitude = std::sqrt(currentVelocity.x * currentVelocity.x + currentVelocity.y * currentVelocity.y);
        auto velocityInfluence = std::min(1.0f, velocityMagnitude / VELOCITY_SCALING_THRESHOLD);

        // Combine velocity-based repulsion with random movement
        auto totalRepulsion = (velocityComponent * velocityInfluence) + (randomComponent * (1.0f - velocityInfluence));

        // Normalize repulsion strength if it exceeds maximum
        auto magnitude = std::sqrt(totalRepulsion.x * totalRepulsion.x + totalRepulsion.y * totalRepulsion.y);
        if (magnitude > MAX_REPULSION_MAGNITUDE)
        {
            totalRepulsion.x *= NORMALIZED_REPULSION_STRENGTH / magnitude;
            totalRepulsion.y *= NORMALIZED_REPULSION_STRENGTH / magnitude;
        }

        // Apply final scaling to repulsion force
        return totalRepulsion * REPULSION_SCALING;
    }

    void AntManager::updateAnt(const Colony &colony, std::vector<Food> &food, size_t currentIndex)
    {
        const auto colonyPosition = colony.getPosition();
        const auto colonySize = colony.getSize();

        auto &ant = ants[currentIndex];
        const auto currentVelocity = ant.getVelocity();
        const auto currentPosition = ant.getPosition();
        const auto antSize = ant.getSize();

        // If carrying food, move toward colony
        if (ant.isBusy())
        {
            const auto newVelocity = calcVelocityTowards(currentPosition, colonyPosition, COLONY_TARGET_ATTRACTION);
            ant.setVelocity(newVelocity);
        }

        if (ant.isMoving())
        {
            // Try moving with current velocity
            const auto newPosition = currentPosition + currentVelocity;
            bool validPosition = !checkAntCollisions(newPosition, antSize, currentIndex, ants) && viewPort.checkViewportBoundaries(newPosition);

            if (validPosition)
            {
                // Check for food collision
                auto *collidedFood = checkFoodCollisions(newPosition, antSize, food);
                if (collidedFood && !ant.isBusy())
                {
                    // Stop moving when food is found
                    ant.setVelocity(Core::Point(0.0f, 0.0f));

                    // Collect food
                    ant.biteFood(*collidedFood);
                }

                // Check for colony collision (to drop food)
                if (checkCollision(newPosition, colonyPosition, antSize, colonySize))
                {
                    ant.dropFood();
                }

                // Update position and return early
                ant.setPosition(newPosition);
                return;
            }
        }

        // If normal movement failed, try alternative positions with repulsion
        for (auto attempt = 0; attempt < MAX_POSITION_ATTEMPTS; attempt++)
        {
            // Calculate repulsion to avoid other ants
            const auto repulsion = calcRepulsion(ant, currentIndex);
            const auto newPosition = currentPosition + repulsion;

            // Check if this position is valid
            auto validPosition = !checkAntCollisions(newPosition, antSize, currentIndex, ants) && viewPort.checkViewportBoundaries(newPosition);

            if (validPosition)
            {
                // Check for food interactions
                auto *collidedFood = checkFoodCollisions(newPosition, antSize, food);
                if (collidedFood && !ant.isBusy())
                {
                    // Stop to collect food
                    ant.setVelocity(Core::Point(0.0f, 0.0f));

                    // Collect food
                    ant.biteFood(*collidedFood);
                }
                else
                {
                    // Set new velocity from repulsion
                    ant.setVelocity(repulsion);
                }

                // Check for colony interaction (to drop food)
                if (checkCollision(newPosition, colonyPosition, antSize, colonySize))
                {
                    ant.dropFood();
                }

                // Update position and break out of retry loop
                ant.setPosition(newPosition);
                break;
            }
        }
    }

    void AntManager::update(const Colony &colony, std::vector<Food> &food)
    {
        for (auto i = 0; i < ants.size(); i++)
        {
            // Pass index instead of filtering in advance
            updateAnt(colony, food, i);
        }
    }

    void AntManager::render(const Render::Renderer &renderer) const
    {
        for (const auto &ant : ants)
            ant.render(renderer);
    }
}