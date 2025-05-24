#include "../../src/simulation/foodManager.hpp"
#include "../../src/core/point.hpp"
#include "../../src/core/viewPort.hpp"

#include "../fakeLogger.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory>

using namespace AntColony::Simulation;
using namespace AntColony::Core;

namespace AntColony::Test::Simulation
{
    class FoodManagerFixture
    {
    public:
        FoodManagerFixture()
            : colonyCenter(0.0f, 0.0f),
              colonyRadius(50.0f),
              foodRadius(5.0f),
              viewport(-250.0f, -250.0f, 250.0f, 250.0f)
        {
            foodManager = std::make_unique<FoodManager>(
                std::make_shared<FakeLogger>(),
                colonyCenter,
                colonyRadius,
                foodRadius,
                viewport);
        }

        std::unique_ptr<FoodManager> createDefaultManager() const
        {
            return std::make_unique<FoodManager>(
                std::make_shared<FakeLogger>(),
                colonyCenter,
                colonyRadius,
                foodRadius,
                viewport);
        }

        const std::vector<Food *> runUpdateForFrames(const int frames)
        {
            for (int i = 0; i < frames; ++i)
            {
                foodManager->update();
            }

            return foodManager->getFoodParticles();
        }

        const int runUpdateForFramesAndRemove(const int frames)
        {
            const auto &foodParticles = runUpdateForFrames(frames);

            for (auto piece : foodParticles)
            {
                do
                {
                    piece->take();
                } while ((piece->getCapacity() > 0));
            }

            return foodManager->getFoodParticles().size();
        }

        Point colonyCenter;
        float colonyRadius;
        float foodRadius;
        ViewPort viewport;
        std::unique_ptr<FoodManager> foodManager;
    };
}
