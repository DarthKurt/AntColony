#include "../../src/simulation/foodManager.hpp"
#include "../../src/core/point.hpp"
#include "../../src/core/viewPort.hpp"

#include "../fakeLogger.hpp"

#include "foodDistributionVisualizer.hpp"
#include "foodDistributionAnalyzer.hpp"
#include "foodManagerFixture.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <memory>

using namespace AntColony::Simulation;
using namespace AntColony::Core;

namespace SimulationTests = AntColony::Test::Simulation;

namespace AntColony::Test::Simulation
{
    TEST_CASE("FoodManager distributes food particles evenly", "[foodmanager]")
    {
        FoodManagerFixture fixture;

        // Generate a large number of food particles to ensure statistical significance
        // And get the generated food particles
        const auto &foodParticles = fixture.runUpdateForFrames(1000000);

        // Setup analysis

        // calc the most radial zone
        float maxRadius = std::min(
            std::min(
                std::abs(fixture.viewport.minX),
                std::abs(fixture.viewport.minY)),
            std::min(
                std::abs(fixture.viewport.maxX),
                std::abs(fixture.viewport.maxY)));

        // Divide into 10 equal-area rings
        int numRings = 10;

        // Define parameters for radial analysis
        SimulationTests::RadialDistributionAnalyzer analyzer(fixture.colonyCenter, fixture.colonyRadius, maxRadius, numRings);
        // Create visualizer for the distribution
        SimulationTests::FoodDistributionVisualizer visualizer(fixture.colonyCenter, fixture.colonyRadius, fixture.colonyRadius, maxRadius, numRings);

        for (const auto &food : foodParticles)
        {
            const auto &point = food->getPosition();

            // Add all food particles to the analyzer
            analyzer.addPoint(point.x, point.y);

            // Add all food particles to the visualizer
            visualizer.addPoint(point);

            // Additionally, check that food particles are outside the colony
            Point foodPos = food->getPosition();
            float distanceFromColony = std::sqrt(
                std::pow(foodPos.x - fixture.colonyCenter.x, 2) +
                std::pow(foodPos.y - fixture.colonyCenter.y, 2));

            REQUIRE(distanceFromColony >= fixture.colonyRadius);
        }

        // Save the visualization
        visualizer.saveToSVG("food_distribution.svg");

        // Calculate coefficient of variation (CV)
        double coeffOfVariation = analyzer.calculateCoefficientOfVariation();

        // Log the result
        std::cout << "Coefficient of Variation: " << coeffOfVariation << std::endl;

        // For a uniform random distribution, we expect some variation, but not too much
        REQUIRE(coeffOfVariation < 0.5);
    }
}
