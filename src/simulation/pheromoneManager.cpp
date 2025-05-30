#include "pheromoneManager.hpp"

namespace AntColony::Simulation
{
    PheromoneManager::PheromoneManager(float pheromoneSize)
        : PheromoneManager(std::make_shared<Utils::ConsoleLogger>(), pheromoneSize) {}

    PheromoneManager::PheromoneManager(std::shared_ptr<Core::Logger> logger, float pheromoneSize)
        : BaseEntityManager(logger), pheromoneSize(pheromoneSize) {}

    void PheromoneManager::update(const std::stack<PheromoneSignal> &signals)
    {
        std::stack<Pheromone *> toDelete;
        for (const auto &[ptr, pheromone] : pheromones)
        {
            ptr->evaporate();
            if (ptr->getStrength() <= 0)
            {
                toDelete.push(ptr);
            }
        }

        while (!toDelete.empty())
        {
            auto ptr = toDelete.top();
            pheromones.erase(ptr);
            toDelete.pop();
        }

        depositPheromones(signals);
    }

    void PheromoneManager::render(const Render::Renderer &renderer) const
    {
        for (const auto &[ptr, pheromone] : pheromones)
        {
            ptr->render(renderer);
        }
    }

    const std::vector<PheromoneSignal> PheromoneManager::getPheromones() const
    {
        std::vector<PheromoneSignal> result;
        result.reserve(pheromones.size());

        for (const auto &[ptr, pheromone] : pheromones)
        {
            result.emplace_back(pheromone->getPosition(), pheromone->getStrength());
        }

        return result;
    }

    void PheromoneManager::depositPheromones(std::stack<PheromoneSignal> positions)
    {
        while (!positions.empty())
        {
            auto p = positions.top();
            depositPheromone(p);
            positions.pop();
        }
    }

    void PheromoneManager::depositPheromone(PheromoneSignal signal)
    {
        auto pheromone = std::make_unique<Pheromone>(signal,pheromoneSize,50);
        pheromones[pheromone.get()] = std::move(pheromone);
    }
}
