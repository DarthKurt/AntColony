#include "baseEntityManager.hpp"

namespace AntColony::Simulation
{
    BaseEntityManager::BaseEntityManager(std::shared_ptr<Core::Logger> logger)
        : logger(logger) {}

    void BaseEntityManager::debug(const std::chrono::steady_clock::time_point *startTime, const std::string &message)
    {
        if (startTime)
        {
            const auto current = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(current - *startTime).count();

            logger->debug("[" + std::to_string(elapsed) + "] " + message);
        }
        else
        {
            logger->debug(message);
        }
    }
}