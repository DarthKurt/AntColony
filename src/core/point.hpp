#pragma once

#include <cmath>

namespace AntColony::Core
{
    struct Point
    {
        float x, y;

        Point();
        Point(float x, float y);

        Point operator+(const Point &other) const;
        Point operator*(const float scalar) const;
        Point operator-(const Point &other) const;
        Point &operator+=(const Point &other);
        Point &operator*=(const float scalar);
        float distanceTo(const Point &other) const;
    };
}