#include "baseEntity.hpp"
#include "../utils/viewPort.hpp"
#include "../core/color.hpp"

namespace AntColony::Simulation
{
    BaseEntity::BaseEntity(Core::Point position, unsigned int hexColor, float size)
        : position(position), mainColor(hexColor), entitySize(size) {}
}