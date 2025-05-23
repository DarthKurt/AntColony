#ifndef FOOD_H
#define FOOD_H

#include "entity.h"

#include <GLFW/glfw3.h>

class Food : public Entity
{
public:
    Food(Point position, float size, int capacity);

    void bite();
    void render(GLFWwindow *window);

    int getCapacity() const;

private:
    float size;
    int capacity;
};

#endif