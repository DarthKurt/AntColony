#include "food.hpp"
#include "baseEntity.hpp"

#include "../core/point.hpp"
#include "../core/color.hpp"

namespace AntColony::Simulation
{
    Food::Food(Core::Point position, float size, int capacity)
        : BaseEntity(position, 0x387d27, size), capacity(capacity) {}

    void Food::bite()
    {
        if (capacity > 0)
        {
            capacity -= 1;
        }
    }

    void Food::render(const Render::Renderer &renderer) const
    {
        renderer.drawCircleInPosition(position, entitySize, Core::Color(mainColor));
    }

    Core::Point Food::getPosition() const { return position; }
    int Food::getCapacity() const { return capacity; }
    float Food::getSize() const { return entitySize; }
}