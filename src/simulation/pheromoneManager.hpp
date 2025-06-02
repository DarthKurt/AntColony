#pragma once

#include "baseEntityManager.hpp"
#include "pheromone.hpp"
#include "pheromoneSignal.hpp"

#include "../core/logger.hpp"

#include <vector>
#include <stack>
#include <memory>
#include <unordered_map>

namespace AntColony::Simulation
{
    class PheromoneManager : public BaseEntityManager
    {
    public:
        explicit PheromoneManager(float pheromoneSize);

        explicit PheromoneManager(std::shared_ptr<Core::Logger> logger, float pheromoneSize);

        void update(const std::stack<PheromoneSignal> &signals);
        void render(Render::Renderer &renderer) const;

        const std::vector<PheromoneSignal> getPheromones() const;

    private:
        std::unordered_map<Pheromone *, std::shared_ptr<Pheromone>> pheromones;
        float pheromoneSize;
        void depositPheromone(PheromoneSignal signal);
        void depositPheromones(std::stack<PheromoneSignal> positions);
    };
}