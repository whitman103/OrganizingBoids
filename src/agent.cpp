#include "agent.hpp"
#include <iostream>

#include "utils.hpp"

void Agent::update_pos(const AgentList &nearbyAgents)
{
    std::cout << "Using a base Agent class." << std::endl;
    assert(false);
};

void Boid::update_pos(const AgentList &nearbyAgents)
{
    this->standard_update_pos(nearbyAgents);
};

void Boid::sense_alignment(const AgentList &nearbyAgents)
{
    valarray<double> averageVelocity(0., NDIM);
    int closeBoids(0);
    for (auto &other_boid : nearbyAgents)
    {
        if (!(get_distance(this->position, other_boid->position) > this->align_sense_distance))
            continue;
        averageVelocity += other_boid->position;
        closeBoids++;
    }
    averageVelocity /= closeBoids;
    this->velocity += (averageVelocity - this->velocity) * this->align_match_distance;
};

void Boid::sense_separation(const AgentList &nearbyAgents)
{
    valarray<double> close(0., NDIM);
    for (auto &other_boid : nearbyAgents)
    {
        if (!(get_distance(this->position, other_boid->position) < this->sep_sense_distance))
            continue;
        close += (this->position - other_boid->position);
    }
    this->velocity += (close * this->sense_match_factor);
};

void Boid::sense_cohesion(const AgentList &nearbyAgents)
{
    valarray<double> groupPosition(0., NDIM);
    int neighboringBoids(0);
    for (auto &other_boid : nearbyAgents)
    {
        if (!(get_distance(this->position, other_boid->position) < this->align_sense_distance))
            continue;
        groupPosition += other_boid->position;
        neighboringBoids++;
    }
    groupPosition /= neighboringBoids;
    this->velocity += (groupPosition - this->position) * this->cohese_match_factor;
};

// Optimized function since we don't need to keep calculating the distance for each sensing function.
void Boid::standard_update_pos(const AgentList &nearbyAgents)
{
    valarray<double> alignment_average_velocity(0., NDIM);
    valarray<double> cohese_group_position(0., NDIM);
    valarray<double> sep_close(0., NDIM);
    int neighboring_boids(0);
    for (auto &other_boid : nearbyAgents)
    {
        const double interboid_distance(get_distance(this->position, other_boid->position));
        // Cohesion step and alignment step

        if (interboid_distance < this->align_sense_distance)
        {
            cohese_group_position += other_boid->position;
            neighboring_boids++;
            alignment_average_velocity += other_boid->velocity;
        };

        // Separation step
        if (interboid_distance < this->sep_sense_distance)
        {
            sep_close += (this->position - other_boid->position);
        };
    }

    valarray<double> delta_velocity(0., NDIM);

    // Alignment update
    alignment_average_velocity /= neighboring_boids;
    delta_velocity += (alignment_average_velocity - this->velocity) * this->align_match_distance;

    // Separation update
    delta_velocity += (sep_close * this->sense_match_factor);

    // Cohesion update
    cohese_group_position /= neighboring_boids;
    delta_velocity += (cohese_group_position - this->position) * this->cohese_match_factor;
    this->velocity += delta_velocity;

    if (this->position[0] < avoid_margin_size)
    {
        this->velocity[0] += margin_turn_factor;
    }
    if (this->position[0] > (windowXSize - avoid_margin_size))
    {
        this->velocity[0] -= margin_turn_factor;
    }
    if (this->position[1] < avoid_margin_size)
    {
        this->velocity[1] += margin_turn_factor;
    }
    if (this->position[1] > (windowYSize - avoid_margin_size))
    {
        this->velocity[1] -= margin_turn_factor;
    }

    if (get_norm(this->velocity) > max_speed)
    {
        this->velocity = this->velocity / get_norm(this->velocity) * max_speed;
    };
    if (get_norm(this->velocity) < min_speed)
    {
        this->velocity = this->velocity / get_norm(this->velocity) * min_speed;
    }
}