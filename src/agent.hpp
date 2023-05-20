#ifndef AGENT_HPP
#define AGENT_HPP

#include <valarray>
#include <vector>
#include <memory>

#include "constants.hpp"

using std::valarray, std::vector, std::unique_ptr;


struct Agent{
    valarray<double> position, velocity;

    Agent(){};
    Agent(valarray<double> pos, valarray<double> vel): position(pos), velocity(vel){};
    virtual void update_pos();
};

using AgentList = vector<unique_ptr<Agent> >;


struct Boid: public Agent {
    void update_pos(const AgentList& nearbyAgents);
    void sense_separation(const AgentList& nearbyAgents);
    void sense_alignment(const AgentList& nearbyAgents);
    void sense_cohesion(const AgentList& nearbyAgents);

    void standard_update_pos(const AgentList& nearbyAgents);
    Boid(valarray<double> pos, valarray<double> vel): Agent(pos, vel){};

    double align_sense_distance = boid_align_sense_distance_default;
    double align_match_distance = boid_align_match_distance_default;
    double sep_sense_distance = boid_sep_sense_distance_default;
    double sense_match_factor = boid_sense_match_factor_default;
    double cohese_sense_distance = boid_cohese_sense_distance_default;
    double cohese_match_factor = boid_cohese_match_factor_default;
};

struct Wolf: public Agent{
    void update_pos();
};


#endif