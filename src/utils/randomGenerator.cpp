#include "randomGenerator.hpp"

namespace AntColony::Utils
{

    RandomGenerator::RandomGenerator()
    {
        seed();
    }

    RandomGenerator &RandomGenerator::getInstance()
    {
        static RandomGenerator instance;
        return instance;
    }

    void RandomGenerator::seed(unsigned int seed)
    {
        engine.seed(seed);
    }

    int RandomGenerator::getInt(int min, int max)
    {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(engine);
    }

    float RandomGenerator::getFloat(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(engine);
    }
}
