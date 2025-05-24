#include "../../src/core/point.hpp"

#include <vector>
#include <cmath>

namespace AntColony::Test::Simulation
{
    // Helper class to analyze radial spatial distribution using concentric rings
    class RadialDistributionAnalyzer
    {
    public:
        RadialDistributionAnalyzer(Core::Point center, float minRadius, float maxRadius, int numRings)
            : minRadius(minRadius), maxRadius(maxRadius), numRings(numRings), center(center)
        {
            rings.resize(numRings, 0);

            // Calculate the area of each ring (should be equal)
            ringArea = M_PI * (maxRadius * maxRadius - minRadius * minRadius) / numRings;

            // Calculate ring boundaries (equal area per ring)
            ringBoundaries.resize(numRings + 1);
            ringBoundaries[0] = minRadius;
            ringBoundaries[numRings] = maxRadius;

            for (int i = 1; i < numRings; i++)
            {
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
            for (int i = 0; i < numRings; i++)
            {
                if (distance >= ringBoundaries[i] && distance < ringBoundaries[i + 1])
                {
                    rings[i]++;
                    break;
                }
            }
        }

        double calculateCoefficientOfVariation()
        {
            int totalPoints = 0;
            for (int count : rings)
            {
                totalPoints += count;
            }

            double mean = static_cast<double>(totalPoints) / rings.size();
            double stdDev = calculateStandardDeviation(rings, mean);

            // Coefficient of variation = std deviation / mean
            return (mean > 0) ? (stdDev / mean) : 0.0;
        }

    private:
        Core::Point center;
        float minRadius, maxRadius;
        int numRings;
        std::vector<int> rings;
        std::vector<float> ringBoundaries;
        float ringArea;

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
    };
}