#pragma once
#include "../core/logger.hpp"

namespace AntColony::Utils
{
    class ConsoleLogger : public AntColony::Core::Logger
    {
    public:
        ConsoleLogger();

        void debug(const std::string &message) override;
        void info(const std::string &message) override;
        void warning(const std::string &message) override;
        void error(const std::string &message) override;
    };
}
