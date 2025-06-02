#pragma once

#include "baseEntityManager.hpp"
#include "food.hpp"
#include "../core/logger.hpp"
#include "../core/viewPort.hpp"

#include <vector>
#include <memory>
#include <unordered_map>

namespace AntColony::Simulation
{
    class FoodManager : public BaseEntityManager
    {
    public:
        FoodManager(std::shared_ptr<Core::Logger> logger, Core::Point colonyCenter, float colonyRadius, float foodRadius, Core::ViewPort viewPort);
        FoodManager(Core::Point colonyCenter, float colonyRadius, float foodRadius, Core::ViewPort viewPort);

        void update();
        void render(Render::Renderer &renderer) const;

        std::vector<std::shared_ptr<Food>> getFoodParticles() const;

    private:
        Core::Point colonyCenter;
        float colonyRadius;
        float foodRadius;
        Core::ViewPort viewPort;

        void spawnFood();

        std::unordered_map<Food *, std::shared_ptr<Food>> foodParticles;
    };
}
