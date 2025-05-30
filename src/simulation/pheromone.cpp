#include "pheromone.hpp"

namespace AntColony::Simulation
{
    Pheromone::Pheromone(PheromoneSignal signal, float size, int strength)
        : BaseEntity(signal.position, 0x0335fc, size), strength(strength * signal.excitement) {}


    void Pheromone::evaporate()
    {
        if (strength > 0)
            strength -= 1;
    }

    void Pheromone::render(const Render::Renderer &renderer) const
    {
        renderer.drawCircleInPosition(position, entitySize, Core::Color(mainColor));
    }

    Core::Point Pheromone::getPosition() const { return position; }
    int Pheromone::getStrength() const { return strength; }
    float Pheromone::getSize() const { return entitySize; }
}