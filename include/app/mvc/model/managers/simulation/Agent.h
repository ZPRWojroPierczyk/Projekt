/**
 * @file Agent.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing Agent class declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef AGENT_H 
#define AGENT_H
#include <utility>
#include <boost/numeric/ublas/vector.hpp>
#include "Route.h"

namespace ul = boost::numeric::ublas;

class Agent
{
// Constructors & Destructors
public:
    Agent() = default;

// Interface
public:

// Private constants
private:

    /* --- Agent's limits --- */

    /// Maximum agent's velocity
    const int __MAX__VELOCITY;
    
    /// Maximum agent's load in [kg]
    const int __MAX__LOAD;

    /// Maximum agent's continuant drive time in [min]
    const int __MAX__DRIVE_TIME;

    /// Agent's gap time (gaps are taken once per __MAX_DRIVE_TIME)
    const int __GAP__TIME;

    /// Probability for agent's breakdown
    const float __BREAK_PROBABILITY;

//Private members
private:

    /* --- Agent's identificators --- */

    const std::string __HOME

    const std::string __ID;

    const std::string __TRANSPORT_ID;


    /* --- Localization informations --- */    

    /// Coordinates given in [latitude; longitude]
    std::pair<float, float> __cordinates;

    /// Agent's road
    Route __route;

    /// Index of the __route's point that
    int __towardPointIndex;

    /// Actual velocity in [km/h]
    int __velocity;


    /* --- Agent's informations --- */
    
    /// Agent's load in [kg]
    int __load;

    /// Start point of the route
    std::string __origin;

    /// End point of the route
    std::string __destination;


    /* --- Agent's state --- */

    /// States whether agent have a break at now
    bool __isGap;

    /// States whether agent has broken
    bool __isBroken;

    /// States whether agent is after an car accident
    bool __isCrashed;

};

#endif