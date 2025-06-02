#include "counter.hpp"

#include "../core/point.hpp"
#include "../core/color.hpp"

namespace AntColony::Simulation
{
    Counter::Counter(Core::Point position, float size)
        : BaseEntity(position, 0x7d4827, size), counter(0) {}

    void Counter::render(Render::Renderer &renderer)
    {
        auto countText = "Food: " + std::to_string(counter);
        renderer.drawText(position, countText, Core::Color(mainColor), entitySize);
    }

    void Counter::increment(int count)
    {
        counter += count;
    }

    void Counter::increment()
    {
        increment(1);
    }

    Core::Point Counter::getPosition() const { return position; }
    float Counter::getSize() const { return entitySize; }
}