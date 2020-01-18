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

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Agent::Agent(const std::string& home, unsigned int id,
             boost::property_tree::ptree params):

    __MAX_VELOCITY(params.get<unsigned int>("params.maxVelocity")),
    __MAX_LOAD(params.get<unsigned int>("params.maxLoad")),
    __MAX_DRIVE_TIME(params.get<unsigned int>("params.maxDriveTime")),
    __GAP_TIME(params.get<unsigned int>("params.gapTime")),
    __BREAK_PROBABILITY(params.get<unsigned int>("params.breakProbability")),

    __HOME(home),
    __ID(id)
{
    __TRANSPORT_ID = std::move(std::string(""));
    
    __cordinates[0] = __cordinates[1] = 0;
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


/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool Agent::initializeTransport(const std::string& origin,
                                const std::string& destination,
                                unsigned int load)
{
    // If load is larger than agent's capacity transport is impossible
    if(load > __MAX_LOAD)
        return false;

    return true;
}
                        


/*------------------------------- Getters & Setters ------------------------------*/