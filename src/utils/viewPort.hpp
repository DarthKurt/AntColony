#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include "../core/point.hpp"

const float LEFT_BOUNDARY = -0.95f;
const float RIGHT_BOUNDARY = 0.95f;

void setColor(const unsigned int hexColor);

bool checkViewportBoundaries(const Point &newPosition);

#endif