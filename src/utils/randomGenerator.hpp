#pragma once
#include <random>
#include <ctime>

namespace AntColony::Utils
{

    class RandomGenerator
    {
    public:
        static RandomGenerator &getInstance();

        // Delete copy/move operations to ensure singleton pattern
        RandomGenerator(const RandomGenerator &) = delete;
        RandomGenerator &operator=(const RandomGenerator &) = delete;
        RandomGenerator(RandomGenerator &&) = delete;
        RandomGenerator &operator=(RandomGenerator &&) = delete;

        // Seed the random engine
        void seed(unsigned int seed = static_cast<unsigned int>(std::time(nullptr)));

        // Generate a random integer in range [min, max]
        int getInt(int min, int max);

        // Generate a random float in range [min, max]
        float getFloat(float min, float max);

        // True with specified probability (0.0 to 1.0)
        bool getBool(float probability = 0.5f);

    private:
        RandomGenerator();
        std::mt19937 engine;
    };

}
