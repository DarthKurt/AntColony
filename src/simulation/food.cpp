#include "food.hpp"

namespace AntColony::Simulation
{
    Food::Food(Core::Point position, float size, int capacity, std::function<void(Food*)> onEmpty)
        : BaseEntity(position, 0x387d27, size), capacity(capacity), onEmpty(onEmpty) {}

    void Food::take()
    {
        if (capacity > 0)
            capacity -= 1;

        if (capacity <= 0 && onEmpty)
            onEmpty(this);
    }

    void Food::render(const Render::Renderer &renderer) const
    {
        renderer.drawCircleInPosition(position, entitySize * capacity, Core::Color(mainColor));
    }

    Core::Point Food::getPosition() const { return position; }
    int Food::getCapacity() const { return capacity; }
    float Food::getSize() const { return entitySize * capacity; }
}