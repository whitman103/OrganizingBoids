#include "grid.hpp"

using std::get, std::make_tuple, std::make_unique;

void GlobalGrid::apply_boundary_conditions()
{
    if (this->globalBCs == reflecting)
    {
        for (auto &region : this->regionList)
        {
            if (region.second.boundary == 0)
                continue;
            region.second.apply_reflecting_conditions(this->globalSizes);
        }
    }
    if (this->globalBCs == periodic)
    {
        for (auto &region : this->regionList)
        {
            if (region.second.boundary == 0)
                continue;
            region.second.apply_periodic_conditions(this->globalSizes);
        }
    }
}

void GlobalGrid::run_simulation_step()
{
    for (auto &regionKey : this->regionList)
    {
        regionKey.second.run_simulation_step();
    }
    this->apply_boundary_conditions();
};

const int GlobalGrid::get_region_label(std::pair<double, double> position)
{
    const int xPos(floor(get<0>(position)) / divisionSize);
    const int yPos(floor(get<1>(position)) / divisionSize);
    return xPos + get<1>(globalSizes) / divisionSize * yPos;
}

void GlobalGrid::add_agent(valarray<double> initPos, valarray<double> initVelo)
{
    this->regionList.at(this->get_region_label(make_tuple(initPos[0], initPos[1])))
        .agentList.push_back(make_unique<Boid>(initPos, initVelo));
}

void Region::apply_periodic_conditions(const std::pair<int, int> sizes)
{
    bool applyLowerX(this->boundary & BoundaryTypes::LowerX), applyUpperX(this->boundary & BoundaryTypes::UpperX),
        applyLowerY(this->boundary & BoundaryTypes::LowerY), applyUpperY(this->boundary & BoundaryTypes::UpperY);
    for (auto &agent : this->agentList)
    {
        if (applyLowerX & (agent->position[0] < 0))
            agent->position[0] += std::get<0>(sizes);
        if (applyLowerY & (agent->position[1] < 0))
            agent->position[1] += std::get<1>(sizes);
        if (applyUpperX & (agent->position[0] > std::get<0>(sizes)))
            agent->position[0] -= std::get<0>(sizes);
        if (applyUpperY & (agent->position[1] > std::get<1>(sizes)))
            agent->position[1] -= std::get<1>(sizes);
    }
};

void Region::apply_reflecting_conditions(const std::pair<int, int> sizes)
{
    bool applyLowerX(this->boundary & BoundaryTypes::LowerX), applyUpperX(this->boundary & BoundaryTypes::UpperX),
        applyLowerY(this->boundary & BoundaryTypes::LowerY), applyUpperY(this->boundary & BoundaryTypes::UpperY);
    for (auto &agent : this->agentList)
    {
        if (applyLowerX & (agent->position[0] < 0))
            agent->position[0] = fabs(agent->position[0]);
        if (applyLowerY & (agent->position[1] < 0))
            agent->position[1] = fabs(agent->position[1]);
        if (applyUpperX & (agent->position[0] > std::get<0>(sizes)))
            agent->position[0] = 2 * std::get<0>(sizes) - agent->position[0];
        if (applyUpperY & (agent->position[1] > std::get<1>(sizes)))
            agent->position[1] = 2 * std::get<1>(sizes) - agent->position[1];
    }
}

void Region::run_simulation_step()
{
    for (auto &agent : this->agentList)
    {
        agent->update_pos(this->agentList);
    }
}