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
    this->position += this->velocity;
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
    int cohese_neighboring_boids(0);
    int align_close_boids(0);
    for (auto &other_boid : nearbyAgents)
    {
        const double interboid_distance(get_distance(this->position, other_boid->position));
        // Cohesion step

        if (interboid_distance < this->align_sense_distance)
        {
            cohese_group_position += other_boid->position;
            cohese_neighboring_boids++;
        };

        // Separation step
        if (interboid_distance < this->sep_sense_distance)
        {
            sep_close += (this->position - other_boid->position);
        };

        // Alignment step
        if (interboid_distance < this->align_sense_distance)
        {
            alignment_average_velocity += other_boid->position;
            align_close_boids++;
        }
    }

    valarray<double> delta_velocity(0., NDIM);

    // Alignment update
    alignment_average_velocity /= align_close_boids;
    delta_velocity += (alignment_average_velocity - this->velocity) * this->align_match_distance;

    // Separation update
    delta_velocity += (sep_close * this->sense_match_factor);

    // Cohesion update
    cohese_group_position /= cohese_neighboring_boids;
    delta_velocity += (cohese_group_position - this->position) * this->cohese_match_factor;
    this->velocity += delta_velocity;

    if (get_norm(this->velocity) > max_speed)
    {
        this->velocity /= get_norm(this->velocity) / max_speed;
    };
    if (get_norm(this->velocity) < min_speed)
    {
        this->velocity /= get_norm(this->velocity) / min_speed;
    }
}