#include "utils.hpp"

double get_distance(valarray<double> position1, valarray<double> position2)
{
    auto vec_distance = position1 - position2;
    return sqrt(vec_distance.sum());
};

double get_norm(valarray<double> inArray)
{
    return sqrt(pow(inArray, 2).sum());
}