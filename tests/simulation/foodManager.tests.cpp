#include "../../src/simulation/foodManager.hpp"
#include "../../src/core/point.hpp"
#include "../../src/core/viewPort.hpp"

#include "foodDistributionVisualizer.hpp"
#include "foodDistributionAnalyzer.hpp"

#include <catch2/catch_test_macros.hpp>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <iostream>
#include <fstream>

using namespace AntColony::Simulation;
using namespace AntColony::Core;

namespace SimulationTests = AntColony::Test::Simulation;

namespace AntColony::Test::Simulation
{
    TEST_CASE("FoodManager distributes food particles evenly", "[foodmanager]")
    {
        // Set up parameters for the test
        Point colonyCenter(0.0f, 0.0f);
        float colonyRadius = 50.0f;
        float foodRadius = 5.0f;
        ViewPort viewport(-250.0f, -250.0f, 250.0f, 250.0f);

        // Create food manager
        FoodManager foodManager(colonyCenter, colonyRadius, foodRadius, viewport);

        // Start from colony edge
        float minRadius = colonyRadius;
        // calc the most radial zone
        float maxRadius = std::min(
            std::min(
                std::abs(viewport.minX),
                std::abs(viewport.minY)),
            std::min(
                std::abs(viewport.maxX),
                std::abs(viewport.maxY)));

        // Divide into 10 equal-area rings
        int numRings = 10;

        // Generate a large number of food particles to ensure statistical significance
        const int numFrameCycles = 1000000; // Reduced for faster testing with visualization
        for (int i = 0; i < numFrameCycles; ++i)
        {
            foodManager.update();
        }

        // Get the generated food particles
        const auto &foodParticles = foodManager.getFoodParticles();

        // Define parameters for radial analysis
        SimulationTests::RadialDistributionAnalyzer analyzer(minRadius, maxRadius, numRings, colonyCenter);
        // Create visualizer for the distribution
        SimulationTests::FoodDistributionVisualizer visualizer(colonyCenter, colonyRadius, minRadius, maxRadius, numRings);

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
                std::pow(foodPos.x - colonyCenter.x, 2) +
                std::pow(foodPos.y - colonyCenter.y, 2));

            REQUIRE(distanceFromColony >= colonyRadius);
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