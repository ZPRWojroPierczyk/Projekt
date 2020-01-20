/**
 * @file Point.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the Agent's class methods
 * @version 0.1
 * @date 2020-01-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Agent.h"

namespace ul = boost::numeric::ublas;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Agent::Agent(const std::string& home, unsigned int id,
             boost::property_tree::ptree params)
    : __MAX_VELOCITY(params.get<unsigned int>("agent.maxSpeed"))
    , __MAX_LOAD(params.get<unsigned int>("agent.maxLoad"))
    , __MAX_DRIVE_TIME(params.get<unsigned int>("agent.maxDrivingTime"))
    , __GAP_TIME(params.get<unsigned int>("agent.breakTime"))
    , __BREAK_PROBABILITY(params.get<unsigned int>("agent.malfunctionProbability"))

    , __HOME(home)
    , __ID(id)
    
    ,__coordinates(ul::vector<float>(2))
{
    __transportID = std::move(std::string(""));
    
    __coordinates[0] = __coordinates[1] = 0;
    __towardPointIndex = 0;

    /**
     * @todo As speed limits informations from google maps are not
     *       available, some simplification is taken and by default
     *       all agents move with maximum velocity all the time.
     * 
     *       It should be changed so that agents match their velocities
     *       to the actual road.
     */
    __velocity = __MAX_VELOCITY;
    __load = 0;

    __origin = std::move(std::string(""));
    __destination = std::move(std::string(""));

    __isGap = false;
    __isBroken = false;
    __isCrashed = false;
}

Agent::Agent(const Agent& agent)
    : __MAX_VELOCITY(agent.__MAX_VELOCITY)
    , __MAX_LOAD(agent.__MAX_LOAD)
    , __MAX_DRIVE_TIME(agent.__MAX_DRIVE_TIME)
    , __GAP_TIME(agent.__GAP_TIME)
    , __BREAK_PROBABILITY(agent.__BREAK_PROBABILITY)

    , __HOME(agent.__HOME)
    , __ID(agent.__ID)

    ,__coordinates(ul::vector<float>(2))
{
    __transportID = agent.__transportID;
    
    __coordinates[0] = agent.__coordinates[0];
    __coordinates[1] = agent.__coordinates[1];
    __towardPointIndex = agent.__towardPointIndex;
    
    __velocity = agent.__velocity;
    __load = agent.__load;

    __origin = agent.__origin;
    __destination = agent.__destination;

    __isGap = agent.__isGap;
    __isBroken = agent.__isBroken;
    __isCrashed = agent.__isCrashed;   
}

/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool Agent::initializeTransport(const std::string& transport_id,
                                const std::string& origin,
                                const std::string& destination,
                                unsigned int load,
                                boost::property_tree::ptree map_params)
{
    /* --- If load is larger than agent's capacity transport is impossible --- */
    
    if(load > __MAX_LOAD)
        return false;
    
    /* --- Find route for the transport if possible--- */


    /* --- Initialize transport's values --- */

    __transportID = transport_id;
    __origin = origin;
    __destination = destination;
    __load = load;

    return true;
}

/*------------------------------- Getters & Setters ------------------------------*/

std::string Agent::getHome() const{
    return __HOME;
}

unsigned int Agent::getMaxLoad() const{
    return __MAX_LOAD;
}

unsigned int Agent::getLoad() const{
    return __load;
}                       

std::string Agent::getTransportID() const{
    return __transportID;
}