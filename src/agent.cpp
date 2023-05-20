#include "agent.hpp"
#include <iostream>

#include "utils.hpp"

void Agent::update_pos(){
    std::cout<<"Using a base Agent class."<<std::endl;
    assert(false);
};

void Boid::update_pos(const AgentList& nearbyAgents){
    this->sense_separation(nearbyAgents);
    this->sense_cohesion(nearbyAgents);
    this->sense_alignment(nearbyAgents);
};

void Boid::sense_alignment(const AgentList& nearbyAgents){
    valarray<double> averageVelocity(NDIM,0);
    int closeBoids(0);
    for (auto& other_boid: nearbyAgents){
        if(!(get_distance(this->position,other_boid->position)>this->align_sense_distance)) continue;
        averageVelocity+=other_boid->position;
        closeBoids++;
    }
    averageVelocity/=closeBoids;
    this->velocity+=(averageVelocity-this->velocity)*this->align_match_distance;
};

void Boid::sense_separation(const AgentList& nearbyAgents){
    valarray<double> close(NDIM,0);
    for(auto& other_boid: nearbyAgents){
        if(!(get_distance(this->position,other_boid->position)<this->sep_sense_distance)) continue;
        close+=(this->position-other_boid->position);
    }
    this->velocity+=(close*this->sense_match_factor);
};

void Boid::sense_cohesion(const AgentList& nearbyAgents){
    valarray<double> groupPosition(NDIM,0);
    int neighboringBoids(0);
    for(auto& other_boid: nearbyAgents){
        if(!(get_distance(this->position,other_boid->position)<this->align_sense_distance)) continue;
        groupPosition+=other_boid->position;
        neighboringBoids++;
    }
    groupPosition/=neighboringBoids;
    this->velocity+=(groupPosition-this->position)*this->cohese_match_factor;
}; 

// Optimized function since we don't need to keep calculating the distance for each sensing function.
void Boid::standard_update_pos(const AgentList& nearbyAgents){
    valarray<double> alignment_average_velocity(NDIM,0);
    valarray<double> cohese_group_position(NDIM,0);
    valarray<double> sep_close(NDIM,0);
    int cohese_neighboring_boids(0);
    int align_close_boids(0);
    for(auto& other_boid: nearbyAgents){

        //Cohesion step
        if((get_distance(this->position,other_boid->position)<this->align_sense_distance)){
            cohese_group_position+=other_boid->position;
            cohese_neighboring_boids++;
        };

        //Separation step
        if((get_distance(this->position,other_boid->position)<this->sep_sense_distance)){
            sep_close+=(this->position-other_boid->position);
        };
        
        //Alignment step
        if((get_distance(this->position,other_boid->position)>this->align_sense_distance)){
            alignment_average_velocity+=other_boid->position;
            align_close_boids++;
        }
        
    }

    
    valarray<double> delta_velocity(NDIM,0);

    //Alignment update
    alignment_average_velocity/=align_close_boids;
    delta_velocity+=(alignment_average_velocity-this->velocity)*this->align_match_distance;

    //Separation update
    delta_velocity+=(sep_close*this->sense_match_factor);

    //Cohesion update
    cohese_group_position/=cohese_neighboring_boids;
    delta_velocity+=(cohese_group_position-this->position)*this->cohese_match_factor;
    this->velocity+=delta_velocity;
}