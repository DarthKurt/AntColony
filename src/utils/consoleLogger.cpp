#include "consoleLogger.hpp"
#include <iostream>

namespace AntColony::Utils
{
    // ANSI color codes for terminal output
    const char *RESET = "\033[0m";
    const char *CYAN = "\033[36m";
    const char *GREEN = "\033[32m";
    const char *YELLOW = "\033[33m";
    const char *RED = "\033[31m";

    ConsoleLogger::ConsoleLogger() {}

    void ConsoleLogger::debug(const std::string &message)
    {
        std::cout << CYAN << "[DEBUG]" << " " << message << RESET<< std::endl;
    }

    void ConsoleLogger::info(const std::string &message)
    {
        std::cout << GREEN << "[INFO]" << " " << message << RESET << std::endl;
    }

    void ConsoleLogger::warning(const std::string &message)
    {
        std::cout << YELLOW << "[WARNING]" << " " << message << RESET << std::endl;
    }

    void ConsoleLogger::error(const std::string &message)
    {
        std::cerr << RED << "[ERROR]" << " " << message << RESET << std::endl;
    }
}
