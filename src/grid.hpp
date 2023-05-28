#ifndef GRID_HPP
#define GRID_HPP

#include "agent.hpp"
#include <iostream>
#include <map>
#include <tuple>

using std::get, std::make_tuple;

enum BoundaryConditions
{
    reflecting,
    periodic
};

enum BoundaryTypes
{
    LowerX = 1,
    UpperX = 2,
    LowerY = 4,
    UpperY = 8
};

template <class BoundaryTypes> inline bool operator|(BoundaryTypes a, BoundaryTypes b)
{
    return (BoundaryTypes)((int)a | (int)b) != 0;
}
template <class BoundaryTypes> inline bool operator&(BoundaryTypes a, BoundaryTypes b)
{
    return (BoundaryTypes)((int)a & (int)b) == (int)b;
}

struct Region
{
    AgentList agentList = {};
    std::pair<int, int> xBounds, yBounds;
    int boundary = 0;

    void apply_periodic_conditions(const std::pair<int, int> sizes);
    void apply_reflecting_conditions(const std::pair<int, int> sizes);

    Region(std::pair<int, int> xBounds, std::pair<int, int> yBounds, const std::pair<int, int> sizes)
        : xBounds(xBounds), yBounds(yBounds)
    {
        this->boundary = (get<0>(xBounds) == 0) * BoundaryTypes::LowerX +
                         (get<1>(xBounds) == get<0>(sizes)) * BoundaryTypes::UpperX +
                         (get<0>(yBounds) == 0) * BoundaryTypes::LowerY +
                         (get<1>(yBounds) == get<1>(sizes)) * BoundaryTypes::UpperY;
    };
    void run_simulation_step();

    Region(Region &&other)
        : agentList(std::move(other.agentList)), boundary(std::move(other.boundary)), xBounds(std::move(other.xBounds)),
          yBounds(std::move(other.yBounds)){};
};

struct GlobalGrid
{
    std::unordered_map<int, Region> regionList;
    BoundaryConditions globalBCs = periodic;
    std::pair<int, int> globalSizes;
    const int divisionSize = 50;
    void apply_boundary_conditions();
    GlobalGrid(const int xSize, const int ySize) : globalSizes(std::make_tuple(xSize, ySize))
    {
        if (std::get<0>(globalSizes) % divisionSize != 0 || std::get<1>(globalSizes) % divisionSize != 0)
        {
            std::cout << "Pick a different image size that evenly divides the size you want." << std::endl;
            assert(false);
        }
        for (int curX{0}; curX < get<0>(globalSizes); curX += divisionSize)
        {
            for (int curY{0}; curY < get<1>(globalSizes); curY += divisionSize)
            {

                this->regionList.insert(make_tuple(
                    this->get_region_label(make_tuple(curX, curY)),
                    Region(make_tuple(curX, curX + divisionSize), make_tuple(curY, curY + divisionSize), globalSizes)));
                ;
            }
        }
    };

    void run_simulation_step();
    void add_agent(valarray<double> init_pos, valarray<double> init_velo);
    const int get_region_label(const std::pair<double, double> position);
    const int get_region_label(const valarray<double> position);
};

#endif