#ifndef ANT_H
#define ANT_H

#include "entity.h"
#include "food.h"

#include <GLFW/glfw3.h>
#include <vector>

class Ant : public Entity
{
public:
    Ant(Point position, float size);
    Ant(Point position, Point velocity, float size);

    void render(GLFWwindow *window) override;

    void biteFood(Food &food);
    void dropFood();

    void setPosition(Point position);
    void setVelocity(Point velocity);
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

#endif
