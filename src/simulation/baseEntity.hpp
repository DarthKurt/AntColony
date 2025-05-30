#pragma once

#include "../core/point.hpp"

namespace AntColony::Simulation
{
    class BaseEntity
    {
    public:
        virtual ~BaseEntity() {}

    protected:
        BaseEntity(Core::Point position, unsigned int hexColor, float size);

        Core::Point position;
        unsigned int mainColor;
        const float entitySize;
    };
}
