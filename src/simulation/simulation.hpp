#pragma once

#include "colony.hpp"
#include "foodManager.hpp"
#include "antManager.hpp"
#include "pheromoneManager.hpp"
#include "counter.hpp"

#include "../core/logger.hpp"
#include "../render/frameContext.hpp"

namespace AntColony::Simulation
{
    class Simulation
    {
    public:
        /**
         * @brief Default constructor
         */
        explicit Simulation();

        /**
         * @brief Constructor with custom logger
         */
        explicit Simulation(std::shared_ptr<Core::Logger> logger);

        void update(const Render::FrameContext &ctx);
        void render(const Render::FrameContext &ctx) const;

    private:
        Simulation(
            std::shared_ptr<Core::Logger> logger,
            Core::ViewPort viewPort,
            Core::Point colonyCenter,
            float colonySize,
            float foodSize,
            float antSize,
            float pheromoneSize);

        std::shared_ptr<Core::Logger> logger;
        Colony colony;
        FoodManager foodManager;
        AntManager antManager;
        PheromoneManager pheromoneManager;
        Counter foodCounter;
    };
}