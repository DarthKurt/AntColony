#pragma once

#include "../utils/consoleLogger.hpp"
#include "../core/point.hpp"

#include <memory>
#include <chrono>

namespace AntColony::Simulation
{
    class BaseEntityManager
    {
    public:
        virtual ~BaseEntityManager() {}

    protected:
        explicit BaseEntityManager(std::shared_ptr<Core::Logger> logger);

        virtual void debug(const std::chrono::steady_clock::time_point *startTime, const std::string &message);

        /**
         * @brief Stores logger utility
         */
        std::shared_ptr<Core::Logger> logger;
    };
}
