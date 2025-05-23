#include <catch2/catch_test_macros.hpp>
#include "../../src/simulation/foodManager.hpp"
#include "../../src/core/point.hpp"
#include "../../src/core/viewPort.hpp"

#include <vector>
#include <cmath>
#include <unordered_map>
#include <iostream>

using namespace AntColony::Simulation;
using namespace AntColony::Core;

// Helper function to calculate standard deviation
double calculateStandardDeviation(const std::vector<int> &values, double mean)
{
    auto sumSquaredDiff = 0.0;
    for (auto value : values)
    {
        auto diff = value - mean;
        sumSquaredDiff += diff * diff;
    }
    return std::sqrt(sumSquaredDiff / values.size());
}

// Helper class to analyze radial spatial distribution using concentric rings
class RadialDistributionAnalyzer
{
public:
    RadialDistributionAnalyzer(float minRadius, float maxRadius, int numRings, Point center)
        : minRadius(minRadius), maxRadius(maxRadius), numRings(numRings), center(center)
    {
        rings.resize(numRings, 0);
        
        // Calculate the area of each ring (should be equal)
        ringArea = M_PI * (maxRadius * maxRadius - minRadius * minRadius) / numRings;
        
        // Calculate ring boundaries (equal area per ring)
        ringBoundaries.resize(numRings + 1);
        ringBoundaries[0] = minRadius;
        ringBoundaries[numRings] = maxRadius;
        
        for (int i = 1; i < numRings; i++) {
            // Formula for equal-area rings: r_i = sqrt(r_min² + i*(r_max² - r_min²)/numRings)
            ringBoundaries[i] = std::sqrt(minRadius * minRadius + 
                                i * (maxRadius * maxRadius - minRadius * minRadius) / numRings);
        }
    }

    void addPoint(float x, float y)
    {
        float dx = x - center.x;
        float dy = y - center.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        // Find which ring this point belongs to
        for (int i = 0; i < numRings; i++) {
            if (distance >= ringBoundaries[i] && distance < ringBoundaries[i + 1]) {
                rings[i]++;
                break;
            }
        }
    }

    double calculateCoefficientOfVariation()
    {
        int totalPoints = 0;
        for (int count : rings) {
            totalPoints += count;
        }

        double mean = static_cast<double>(totalPoints) / rings.size();
        double stdDev = calculateStandardDeviation(rings, mean);

        // Coefficient of variation = std deviation / mean
        return (mean > 0) ? (stdDev / mean) : 0.0;
    }

private:
    float minRadius, maxRadius;
    int numRings;
    Point center;
    std::vector<int> rings;
    std::vector<float> ringBoundaries;
    float ringArea;
};

TEST_CASE("FoodManager distributes food particles evenly", "[foodmanager]")
{
    // Set up parameters for the test
    Point colonyCenter(0.0f, 0.0f);
    float colonyRadius = 50.0f;
    float foodRadius = 5.0f;
    ViewPort viewport(-1000.0f, -1000.0f, 1000.0f, 1000.0f);

    // Create food manager
    FoodManager foodManager(colonyCenter, colonyRadius, foodRadius, viewport);

    // Generate a large number of food particles to ensure statistical significance
    const int numFrameCycles = 1000000;
    for (int i = 0; i < numFrameCycles; ++i)
    {
        foodManager.update();
    }

    // Get the generated food particles
    const auto &foodParticles = foodManager.getFoodParticles();

    // Define parameters for radial analysis
    float minRadius = colonyRadius; // Start from colony edge
    float maxRadius = 3 * colonyRadius; // Analyze up to 3x colony radius
    int numRings = 10; // Divide into 10 equal-area rings

    RadialDistributionAnalyzer analyzer(minRadius, maxRadius, numRings, colonyCenter);

    // Add all food particles to the analyzer
    for (const auto &food : foodParticles)
    {
        const auto &point = food->getPosition();
        analyzer.addPoint(point.x, point.y);
    }

    // Calculate coefficient of variation (CV)
    double coeffOfVariation = analyzer.calculateCoefficientOfVariation();

    // Log the result
    std::cout << "Coefficient of Variation: " << coeffOfVariation << std::endl;

    // For a uniform random distribution, we expect some variation, but not too much
    REQUIRE(coeffOfVariation < 0.5);

    // Additionally, check that food particles are outside the colony
    for (const auto food : foodParticles)
    {
        Point foodPos = food->getPosition();
        float distanceFromColony = std::sqrt(
            std::pow(foodPos.x - colonyCenter.x, 2) +
            std::pow(foodPos.y - colonyCenter.y, 2));

        REQUIRE(distanceFromColony >= colonyRadius);
    }
}
