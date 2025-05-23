#pragma once

#include "colony.hpp"
#include "foodManager.hpp"
#include "antManager.hpp"

#include "../render/frameContext.hpp"

namespace AntColony::Simulation
{
    class Simulation
    {
    public:
        Simulation();

        void update(Render::FrameContext &ctx);
        void render(const Render::FrameContext &ctx) const;

    private:
        Colony colony;
        FoodManager foodManager;
        AntManager antManager;
    };
}
