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

// Helper class to analyze spatial distribution
class SpatialDistributionAnalyzer
{
public:
    SpatialDistributionAnalyzer(float gridSize, float minX, float maxX, float minY, float maxY)
        : gridSize(gridSize), minX(minX), maxX(maxX), minY(minY), maxY(maxY)
    {

        gridWidth = static_cast<int>(std::ceil((maxX - minX) / gridSize));
        gridHeight = static_cast<int>(std::ceil((maxY - minY) / gridSize));

        grid.resize(gridWidth * gridHeight, 0);
    }

    void addPoint(float x, float y)
    {
        int gridX = static_cast<int>((x - minX) / gridSize);
        int gridY = static_cast<int>((y - minY) / gridSize);

        if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight)
        {
            grid[gridY * gridWidth + gridX]++;
        }
    }

    double calculateCoefficientOfVariation()
    {
        int totalPoints = 0;
        for (int count : grid)
        {
            totalPoints += count;
        }

        double mean = static_cast<double>(totalPoints) / grid.size();
        double stdDev = calculateStandardDeviation(grid, mean);

        // Coefficient of variation = std deviation / mean
        return (mean > 0) ? (stdDev / mean) : 0.0;
    }

    void printDistribution()
    {
        std::cout << "Grid distribution:" << std::endl;
        for (int y = 0; y < gridHeight; ++y)
        {
            for (int x = 0; x < gridWidth; ++x)
            {
                std::cout << grid[y * gridWidth + x] << " ";
            }
            std::cout << std::endl;
        }
    }

private:
    float gridSize;
    float minX, maxX, minY, maxY;
    int gridWidth, gridHeight;
    std::vector<int> grid;
};

TEST_CASE("FoodManager distributes food particles evenly", "[foodmanager]")
{
    // Set up parameters for the test
    Point colonyCenter(0.0f, 0.0f);
    float colonyRadius = 50.0f;
    float foodRadius = 5.0f;

    // Create food manager
    FoodManager foodManager(colonyCenter, colonyRadius, foodRadius, AntColony::Core::ViewPort(-100.0f, -100.0f, 100.0f, 100.0f));

    // Generate a large number of food particles to ensure statistical significance
    const int numFrameCycles = 1000000;
    for (int i = 0; i < numFrameCycles; ++i)
    {
        foodManager.update();
    }

    // Get the generated food particles
    const std::vector<Food> &foodParticles = foodManager.getFoodParticles();

    // Define grid size for analyzing distribution
    float gridSize = 5.0f; // Adjust based on the expected density

    // Define boundaries for analysis (adjust as needed based on your simulation)
    float minX = colonyCenter.x - colonyRadius * 3;
    float maxX = colonyCenter.x + colonyRadius * 3;
    float minY = colonyCenter.y - colonyRadius * 3;
    float maxY = colonyCenter.y + colonyRadius * 3;

    SpatialDistributionAnalyzer analyzer(gridSize, minX, maxX, minY, maxY);

    // Add all food particles to the analyzer
    for (const auto &food : foodParticles)
    {
        analyzer.addPoint(food.getPosition().x, food.getPosition().y);
    }

    // Optional: Print the distribution for visual debugging
    analyzer.printDistribution();

    // Calculate coefficient of variation (CV)
    // Lower CV means more even distribution
    double coeffOfVariation = analyzer.calculateCoefficientOfVariation();

    // Log the result
    std::cout << "Coefficient of Variation: " << coeffOfVariation << std::endl;

    // For a uniform random distribution, we expect some variation, but not too much
    // The threshold can be adjusted based on expected distribution properties
    REQUIRE(coeffOfVariation < 0.5); // This is an example threshold, adjust as needed

    // Additionally, check that food particles are outside the colony
    for (const auto &food : foodParticles)
    {
        Point foodPos = food.getPosition();
        float distanceFromColony = std::sqrt(
            std::pow(foodPos.x - colonyCenter.x, 2) +
            std::pow(foodPos.y - colonyCenter.y, 2));

        REQUIRE(distanceFromColony >= colonyRadius);
    }
}
