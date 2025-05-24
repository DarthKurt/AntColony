#pragma once
#include "../src/core/logger.hpp"

namespace AntColony::Test
{
    class FakeLogger : public AntColony::Core::Logger
    {
    public:
        FakeLogger() {}

        void debug(const std::string &message) override {}
        void info(const std::string &message) override {}
        void warning(const std::string &message) override {}
        void error(const std::string &message) override {}
    };
}
