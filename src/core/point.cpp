#include "point.hpp"

Point::Point() : x(0), y(0)
{
}

Point::Point(float x, float y) : x(x), y(y)
{
}

Point Point::operator+(const Point &other) const
{
    return {x + other.x, y + other.y};
}

Point Point::operator*(const float scalar) const
{
    return {x * scalar, y * scalar};
}

Point &Point::operator+=(const Point &other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Point &Point::operator*=(const float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

float Point::distanceTo(const Point &other) const
{
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}
