#pragma once

#include "../core/point.hpp"
#include "../core/color.hpp"

#include <string>

namespace AntColony::Render
{
    class Renderer
    {
    public:
        virtual ~Renderer() {}

        /**
         * @brief Draws a circle at the position with a given size and color
         */
        virtual void drawCircleInPosition(const Core::Point &position, const float radius, const Core::Color &color) const = 0;

        /**
         * @brief Draws a given text at the position with a given color
         */
        virtual void drawText(const Core::Point &position, const std::string &text, const Core::Color &color) const = 0;

        /**
         * @brief Draws a frame around position with length and height
         */
        virtual void drawFrame(const Core::Point &position, const float length, const float height, const Core::Color &color) const = 0;
    };
}