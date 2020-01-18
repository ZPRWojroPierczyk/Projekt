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
#include <boost/property_tree/ptree.hpp>
#include "Route.h"

/**
 * @brief Agent class represents a single Agent (a lorry) responsible for delivering transports
 * 
 * Class incorporates basic informations about an agent, involving machine's
 * and driver's limits, identificators, actual transport specification or
 * technical condition of the machine. 
 * 
 */
class Agent
{
// Constructors & Destructors
public:
    
    /**
     * @brief Construct a new Agent object initialized
     *        with basic parameters and identificators
     *        but without any transport assigned.
     * 
     * @param home Agent's home city
     * @param id Agent's identifier
     * @param params Tree of parameters defining basic agent's
     *               behaviour
     */
    Agent(const std::string& home, unsigned int id,
          boost::property_tree::ptree params);

// Interface
public:

    /**
     * @brief Initializes a new transport between given cities
     *        if possible.
     * 
     * @param origin Start point of the transport
     * @param destination Transport's destination
     * @param load Amount of stuff to transport
     * @return true Transport was sucesfullt allocated
     * @return false It is not possible for agent to perform transport
     */
    bool initializeTransport(const std::string& origin,
                             const std::string& destination,
                             unsigned int load);


// Private constants
private:

    /* --- Agent's limits --- */

    /// Maximum agent's velocity
    const unsigned int __MAX_VELOCITY;
    
    /// Maximum agent's load in [kg]
    const unsigned int __MAX_LOAD;

    /// Maximum agent's continuant drive time in [min]
    const unsigned int __MAX_DRIVE_TIME;

    /// Agent's gap time (gaps are taken once per __MAX_DRIVE_TIME)
    const unsigned int __GAP_TIME;

    /// Probability for agent's breakdown
    const unsigned int __BREAK_PROBABILITY;

//Private members
private:

    /* --- Agent's identificators --- */

    const std::string __HOME;

    const unsigned int __ID;

    std::string __TRANSPORT_ID;


    /* --- Localization informations --- */    

    /// Coordinates given in [latitude; longitude]
     boost::numeric::ublas::vector<float> __cordinates;

    /// Agent's road
    Route __route;

    /// Index of the __route's point that Agent's moving toward
    unsigned int __towardPointIndex;

    /// Actual velocity in [km/h]
    unsigned int __velocity;


    /* --- Agent's informations --- */
    
    /// Agent's load in [kg]
    unsigned int __load;

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