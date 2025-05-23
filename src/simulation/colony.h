#ifndef COLONY_H
#define COLONY_H

#include "entity.h"

#include <GLFW/glfw3.h>
#include <vector>

class Colony : public Entity
{
public:
    Colony(Point position, float size);
    Colony(Point position, float size, unsigned int hexColor);

    void render(GLFWwindow *window) const override;

    Point getPosition() const override;
    float getSize() const override;
};

#endif