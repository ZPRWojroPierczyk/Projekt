#include <cmath>
#include <string>
#include <strstream>
#include "Point.h"

namespace ul = boost::numeric::ublas;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Point::Point(int unsigned max_velocity,
             int unsigned crash_probabiliy,
             int unsigned traffic_probability,
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

    int distance = sqrt(
        pow(__coordinates[0] - point.toVector()[0], 2) +
        pow(__coordinates[0] - point.toVector()[0], 2)
    );

    if(distance > TOLERANCE)
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
    return __TRASFFIC_PROBABILITY;
}

bool Point::isTraffic() const{
    return __isTraffic;
}

void Point::setTraffic(bool isTraffic) const{
    __isTraffic = isTraffic;
}