#pragma once

#include "../../src/core/point.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>

namespace AntColony::Test::Simulation
{
    // Class to create an SVG visualization of food distribution
    class FoodDistributionVisualizer
    {
    public:
        FoodDistributionVisualizer(
            const Core::Point &center,
            float colonyRadius,
            float minAnalysisRadius,
            float maxAnalysisRadius,
            int numRings)
            : center(center),
              colonyRadius(colonyRadius),
              minAnalysisRadius(minAnalysisRadius),
              maxAnalysisRadius(maxAnalysisRadius),
              numRings(numRings)
        {
            // Calculate visualization dimensions
            visualizationSize = 2.5f * maxAnalysisRadius; // Leave some margin

            // Calculate ring boundaries for visualization
            ringBoundaries.resize(numRings + 1);
            ringBoundaries[0] = minAnalysisRadius;
            ringBoundaries[numRings] = maxAnalysisRadius;

            for (int i = 1; i < numRings; i++)
            {
                // Formula for equal-area rings: r_i = sqrt(r_min² + i*(r_max² - r_min²)/numRings)
                ringBoundaries[i] = std::sqrt(minAnalysisRadius * minAnalysisRadius +
                                              i * (maxAnalysisRadius * maxAnalysisRadius - minAnalysisRadius * minAnalysisRadius) / numRings);
            }

            // Calculate counts per ring for visualization
            ringCounts.resize(numRings, 0);
        }

        void addPoint(const Core::Point &position)
        {
            foodParticles.push_back(position);

            // Update ring counts
            float dx = position.x - center.x;
            float dy = position.y - center.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            for (int i = 0; i < numRings; i++)
            {
                if (distance >= ringBoundaries[i] && distance < ringBoundaries[i + 1])
                {
                    ringCounts[i]++;
                    break;
                }
            }
        }

        void saveToSVG(const std::string &filename)
        {
            std::ofstream file(filename);
            if (!file)
            {
                std::cerr << "Failed to open file: " << filename << std::endl;
                return;
            }

            // SVG header
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
            file << "<svg width=\"800\" height=\"800\" viewBox=\""
                 << -visualizationSize << " " << -visualizationSize << " "
                 << 2 * visualizationSize << " " << 2 * visualizationSize
                 << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

            // Background
            file << "  <rect x=\"" << -visualizationSize << "\" y=\"" << -visualizationSize
                 << "\" width=\"" << 2 * visualizationSize << "\" height=\"" << 2 * visualizationSize
                 << "\" fill=\"#f0f0f0\" />\n";

            // Draw analysis rings
            for (int i = 0; i < numRings; i++)
            {
                // Calculate color based on density (normalize by area)
                float innerRadius = ringBoundaries[i];
                float outerRadius = ringBoundaries[i + 1];
                float ringArea = M_PI * (outerRadius * outerRadius - innerRadius * innerRadius);
                float density = ringCounts[i] / ringArea;

                // Normalize by maximum density
                float maxDensity = 0;
                for (int j = 0; j < numRings; j++)
                {
                    float r1 = ringBoundaries[j];
                    float r2 = ringBoundaries[j + 1];
                    float area = M_PI * (r2 * r2 - r1 * r1);
                    float d = ringCounts[j] / area;
                    if (d > maxDensity)
                        maxDensity = d;
                }

                float normalizedDensity = maxDensity > 0 ? density / maxDensity : 0;

                // Color from light blue (low density) to dark blue (high density)
                int colorValue = 255 - static_cast<int>(normalizedDensity * 200);
                std::string ringColor = "rgba(100, 100, " + std::to_string(colorValue) + ", 0.3)";

                file << "  <circle cx=\"" << center.x << "\" cy=\"" << center.y
                     << "\" r=\"" << outerRadius
                     << "\" fill=\"" << ringColor << "\" stroke=\"#888\" stroke-width=\"0.5\" />\n";
            }

            // Draw colony
            file << "  <circle cx=\"" << center.x << "\" cy=\"" << center.y
                 << "\" r=\"" << colonyRadius
                 << "\" fill=\"#aa6633\" stroke=\"#000\" stroke-width=\"1\" />\n";

            // Draw food particles
            for (const auto &food : foodParticles)
            {
                file << "  <circle cx=\"" << food.x << "\" cy=\"" << food.y
                     << "\" r=\"2\" fill=\"#ff0000\" />\n";
            }

            // Add legend for ring counts
            file << "  <text x=\"" << -visualizationSize + 20 << "\" y=\""
                 << -visualizationSize + 30 << "\" font-family=\"Arial\" font-size=\"12\">Ring Counts:</text>\n";

            for (int i = 0; i < numRings; i++)
            {
                float innerRadius = ringBoundaries[i];
                float outerRadius = ringBoundaries[i + 1];

                file << "  <text x=\"" << -visualizationSize + 20 << "\" y=\""
                     << -visualizationSize + 50 + i * 20 << "\" font-family=\"Arial\" font-size=\"12\">"
                     << "Ring " << i + 1 << " (" << innerRadius << "-" << outerRadius << "): "
                     << ringCounts[i] << " food items</text>\n";
            }

            // End SVG
            file << "</svg>\n";
            file.close();

            std::cout << "Visualization saved to " << filename << std::endl;
        }

    private:
        Core::Point center;
        float colonyRadius;
        float minAnalysisRadius;
        float maxAnalysisRadius;
        int numRings;
        float visualizationSize;

        std::vector<Core::Point> foodParticles;
        std::vector<float> ringBoundaries;
        std::vector<int> ringCounts;
    };
}
