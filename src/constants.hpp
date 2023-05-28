#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

constexpr int windowXSize(1200);
constexpr int windowYSize(1200);

constexpr double boid_align_sense_distance_default = 40;
constexpr double boid_align_match_factor_default = 0.0005;
constexpr double boid_sep_sense_distance_default = 12;
constexpr double boid_sense_match_factor_default = 0.0005;
constexpr double boid_cohese_sense_distance_default = 40;
constexpr double boid_cohese_match_factor_default = 0.01;
constexpr int avoid_margin_size = 100;
constexpr double margin_turn_factor = 0;

constexpr double min_speed = 1;
constexpr double max_speed = 6;

constexpr int NDIM = 2;

#endif