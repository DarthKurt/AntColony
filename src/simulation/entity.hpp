#pragma once

#include "../core/point.hpp"

#include <GLFW/glfw3.h>

class Entity
{
protected:
    // Common position attribute
    Point position;
    unsigned int mainColor;
    float entitySize;

    // Draws a circle at the entity's position with a given size and color
    virtual void drawCircleInPosition(unsigned int hexColor, float size, GLFWwindow *window) const;

    virtual ~Entity() = default;

public:
    Entity(Point position, unsigned int hexColor, float size);
    // Abstract method for rendering
    virtual void render(GLFWwindow *window) const = 0;

    virtual Point getPosition() const = 0;
    virtual float getSize() const = 0;
};
