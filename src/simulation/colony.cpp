#include "colony.hpp"
#include "baseEntity.hpp"
#include "../core/point.hpp"

namespace AntColony::Simulation
{
    Colony::Colony(Core::Point position, float size)
        : BaseEntity(position, 0x7d4827, size) {}

    Colony::Colony(Core::Point position, float size, unsigned int hexColor)
        : BaseEntity(position, hexColor, size) {}

    void Colony::render(const Render::Renderer &renderer) const
    {
        renderer.drawCircleInPosition(position, entitySize, Core::Color(mainColor));
    }

    Core::Point Colony::getPosition() const { return position; }
    float Colony::getSize() const { return entitySize; }
}