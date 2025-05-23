#pragma once

#include "ant.hpp"
#include "colony.hpp"
#include "food.hpp"

#include "../core/logger.hpp"

#include <vector>
#include <memory>

namespace AntColony::Simulation
{
    /**
     * @class AntManager
     * @brief Manages the simulation of ant behavior and interactions.
     *
     * This class is responsible for creating, updating, and rendering ants.
     * It handles ant spawning, movement, collision detection, and interactions
     * with food and the colony.
     */
    class AntManager
    {
    public:
        /**
         * @brief Default constructor
         */
        AntManager();

        /**
         * @brief Constructor with custom logger
         */
        explicit AntManager(std::shared_ptr<Core::Logger> logger);

        /**
         * @brief Spawns ants at the provided colony
         * @param colony The colony where ants will spawn
         * @param antSize The desired size of ants
         */
        void spawnAnts(const Colony &colony, const float antSize);

        /**
         * @brief Updates all ants' positions and states
         * @param colony The colony that ants interact with
         * @param food Vector of food sources that ants can interact with
         */
        void update(const Colony &colony, std::vector<Food> &food);

        /**
         * @brief Renders all ants to the window
         * @param renderer The renderer to be used
         */
        void render(const Render::Renderer &renderer) const;

    private:
        /**
         * @brief Stores all ants in the simulation
         */
        std::vector<Ant> ants;

        /**
         * @brief Stores logger utility
         */
        std::shared_ptr<Core::Logger> logger;

        /**
         * @brief Generates a grid of hexagonal cells inside a circle
         * @param center Center point of the circle
         * @param radius Radius of the circle
         * @param cellSize Size of each hexagonal cell
         * @return Vector of points representing positions for the hexagonal grid
         */
        static std::vector<Core::Point> generateHexGrid(Core::Point center, float radius, float cellSize);

        /**
         * @brief Checks if a position will collide with any other ant
         * @param newPosition Position to check for collisions
         * @param antSize Size of the ant
         * @param currentIndex Index of the current ant (to avoid self-collision)
         * @param ants Vector of ants to check against
         * @return True if collision detected, false otherwise
         */
        static bool checkAntCollisions(const Core::Point &newPosition, float antSize, size_t currentIndex, const std::vector<Ant> &ants);

        /**
         * @brief Checks if a position collides with food and returns the first food encountered
         * @param newPosition Position to check for collisions
         * @param antSize Size of the ant
         * @param food Vector of food sources to check against
         * @return Pointer to collided food or nullptr if no collision
         */
        static Food *checkFoodCollisions(const Core::Point &newPosition, float antSize, std::vector<Food> &food);

        /**
         * @brief Calculates velocity vector towards a target
         * @param oldPosition Current position
         * @param newPosition Target position
         * @param strength Strength of the movement
         * @return Velocity vector pointing towards the target
         */
        static Core::Point calcVelocityTowards(const Core::Point &oldPosition, const Core::Point &newPosition, float strength);

        /**
         * @brief Computes repulsion force to avoid collisions with nearby ants
         * @param ant The ant for which to calculate repulsion
         * @param currentIndex Index of the current ant
         * @return Repulsion force vector
         */
        Core::Point calcRepulsion(const Ant &ant, size_t currentIndex) const;

        /**
         * @brief Updates a single ant's position and state
         * @param colony The colony that ants interact with
         * @param food Vector of food sources
         * @param currentIndex Index of the ant to update
         */
        void updateAnt(const Colony &colony, std::vector<Food> &food, size_t currentIndex);

        /**
         * @brief Checks if two circles collide
         * @param lCenter Center of the first circle
         * @param rCenter Center of the second circle
         * @param lSize Radius of the first circle
         * @param rSize Radius of the second circle
         * @return True if circles collide, false otherwise
         */
        static bool checkCollision(
            const Core::Point &lCenter,
            const Core::Point &rCenter,
            const float &lSize,
            const float &rSize);
    };
}