#ifndef POINT_HPP
#define POINT_HPP

#include <cmath>

struct Point
{
    float x, y;

    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}

    Point operator+(const Point &other) const
    {
        return {x + other.x, y + other.y};
    }

    Point operator*(const float scalar) const
    {
        return {x * scalar, y * scalar};
    }

    Point &operator+=(const Point &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point &operator*=(const float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    float distanceTo(const Point &other) const
    {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

#endif
