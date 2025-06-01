#include "counter.hpp"

#include "../core/point.hpp"
#include "../core/color.hpp"

namespace AntColony::Simulation
{
    Counter::Counter(Core::Point position, float size)
        : BaseEntity(position, 0x7d4827, size), counter(0) {}

    void Counter::render(const Render::Renderer &renderer) const
    {

        auto countText = "Food: " + std::to_string(counter);
    
        // Calculate frame dimensions based on text length
        // Assuming average character width is 1/4 of size        
        auto charWidth = entitySize / 4.0f;
        auto padding = 0.01f;
    
        auto length = countText.length() * charWidth + padding;
    
        // Draw a frame and text
        renderer.drawFrame(position, length, entitySize, Core::Color(mainColor));
        renderer.drawText(position, countText, Core::Color(mainColor));
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