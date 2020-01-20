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
#include <chrono>
#include <utility>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Route.h"
#include "Transport.h"

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

    /**
     * @brief Simple copying constructor
     * @param agent 
     */
    Agent(const Agent& agent);

// Interface
public:

    /**
     * @brief Initializes a new transport between given cities
     *        if possible.
     * 
     * @param transport Pointer to the Transport object associated with an agent
     * @param origin Start point of the transport
     * @param destination Transport's destination
     * @param load Amount of stuff to transport
     * @param map_params Characteristic parameters of the map used during
     *                   route's planning
     * @returns true Transport was sucesfullt allocated
     * @returns false It is not possible for agent to perform transport
     */
    bool initializeTransport(Transport* transport,
                             const std::string& origin,
                             const std::string& destination,
                             unsigned int load,
                             boost::property_tree::ptree map_params);

    /* --------- Getters & setters --------- */

    /**
     * @returns Agent's home city 
     */
    std::string getHome() const;

    /**
     * @returns Agent's maximum load 
     */
    unsigned int getMaxLoad() const;

    /**
     * @returns Agent's actual load 
     */
    unsigned int getLoad() const;

    /**
     * @returns Identifier of the transport realized by the agent
     */
    const Transport* getTransport() const;

// Private constants
private:

    /* --- Agent's limits --- */

    /// Maximum agent's velocity in p[km/h]
    const unsigned int __MAX_VELOCITY;
    
    /// Maximum agent's load in [kg]
    const unsigned int __MAX_LOAD;

    /// Maximum agent's continuant drive time in [min]
    const unsigned int __MAX_DRIVE_TIME;

    /// Agent's gap time (gaps are taken once per __MAX_DRIVE_TIME) in [min]
    const unsigned int __GAP_TIME;

    /// Probability for agent's breakdown in [%]
    const unsigned int __BREAK_PROBABILITY;

//Private members
private:

    /* --- Agent's identificators --- */

    /// Agent's home city
    const std::string __HOME;

    /// Agent's identificator (unique per home city)
    const unsigned int __ID;

    /// Pointer to the Transport object associated with an agent
    Transport* __transport;


    /* --- Localization informations --- */    

    /// Coordinates given in [latitude; longitude]
     boost::numeric::ublas::vector<float> __coordinates;

    /// Agent's route
    Route __route;

    /// Index of the __route's point that Agent's moving toward
    unsigned int __towardPointIndex;

    /// Actual velocity in [km/h]
    unsigned int __velocity;


    /* --- Transport's informations --- */
    
    /// Agent's load in [kg]
    unsigned int __load;

    /// Start point of the route
    std::string __origin;

    /// End point of the route
    std::string __destination;


    /* --- Agent's state --- */

    /// Duration of the continuous drive
    std::chrono::milliseconds __continuousDriveTime;

    /// States whether agent have a break at now
    bool __isGap;

    /// States whether agent has broken
    bool __isBroken;

    /// States whether agent is after an car accident
    bool __isCrashed;

};

#endif