#pragma once
#include <string>

namespace AntColony::Core
{
    class Logger
    {
    public:
        virtual ~Logger() {}
        virtual void debug(const std::string &message) = 0;
        virtual void info(const std::string &message) = 0;
        virtual void warning(const std::string &message) = 0;
        virtual void error(const std::string &message) = 0;
    };
}
