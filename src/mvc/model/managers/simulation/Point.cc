/**
 * @file Point.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the Point's class methods
 * @version 0.1
 * @date 2020-01-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <cmath>
#include <string>
#include <sstream>
#include "Point.h"

namespace ul = boost::numeric::ublas;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Point::Point(unsigned int max_velocity,
             unsigned int crash_probabiliy,
             unsigned int traffic_probability,
             boost::numeric::ublas::vector<float> coordinates) :
    __MAX_VELOCITY(max_velocity),
    __CRASH_PROBABILITY(crash_probabiliy),
    __TRAFFIC_PROBABILITY(traffic_probability),
    __coordinates(coordinates)
{   
    /* Check arguments correctness */

    // Check if probabilities are in [0; 100] range
    if(crash_probabiliy > 100 || traffic_probability > 100){
        throw std::runtime_error("Probability cannot be higher than 100%");
    }

    // Check if coordinates are given as 2D vector
    if(coordinates.size() != 2){
        std::stringstream error_message;
        error_message << "Point constructor should be givent with 2D vector coordinates, "
                      << "but given dimension of coordinates is: " << std::to_string(coordinates.size());
        throw std::runtime_error(error_message.str());
    }

}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool Point::isClose(Point point) const{

    double distance = std::sqrt(
        std::pow(__coordinates[0] - point.toVector()[0], 2) +
        std::pow(__coordinates[0] - point.toVector()[0], 2)
    );

    if(static_cast<unsigned int>(distance) > TOLERANCE)
        return false;
    else
        return true;

}

/*-------------------------------- Getters & Setters -----------------------------*/

boost::numeric::ublas::vector<float> Point::toVector() const{
    return __coordinates;
}

int Point::getMaxVelocity() const{
    return __MAX_VELOCITY;
}

float Point::getCrashProbability() const{
    return __CRASH_PROBABILITY;
}

float Point::getTrafficProbability() const{
    return __TRAFFIC_PROBABILITY;
}

bool Point::isTraffic() const{
    return __isTraffic;
}

void Point::setTraffic(bool isTraffic){
    __isTraffic = isTraffic;
}