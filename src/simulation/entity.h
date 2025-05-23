#ifndef ENTITY_H
#define ENTITY_H

#include "../core/point.hpp"

#include <GLFW/glfw3.h>

class Entity
{
protected:
    // Common position attribute
    Point position;
    float entitySize;
    unsigned int mainColor;

    // Draws a circle at the entity's position with a given size and color
    virtual void drawCircleInPosition(unsigned int hexColor, float size, GLFWwindow *window) const;

    virtual ~Entity() = default;

public:
    Entity(Point pos, unsigned int hexColor, float size);
    // Abstract method for rendering
    virtual void render(GLFWwindow *window) const = 0;

    virtual Point getPosition() const = 0;
    virtual float getSize() const = 0;
};

#endif
