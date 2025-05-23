#pragma once

#include "entity.hpp"
#include "food.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Ant : public Entity
{
public:
    Ant(Point position, float size);
    Ant(Point position, Point velocity, float size);

    void render(GLFWwindow *window) const override;

    void biteFood(Food &food);
    void dropFood();

    void setPosition(Point newPosition);
    void setVelocity(Point newVelocity);
    Point getVelocity() const;

    bool isBusy() const;
    bool isMoving() const;

    Point getPosition() const override;
    float getSize() const override;

private:
    // Stores previous movement direction
    Point velocity;
    bool carryFood;
};

