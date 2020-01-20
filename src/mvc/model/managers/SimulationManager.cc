/**
 * @file SimulationManager.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of SimulationManager's methods
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "SimulationManager.h"

namespace pt = boost::property_tree;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

SimulationManager::SimulationManager(const std::string& client_id)
    : __clientID(client_id)
    , __isInitialized(false)
    , __isRunning(false)
    , __speedMultiplier(1)
    , __simulationTime(0)
{}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool SimulationManager::isSimulationInitialized(){
    return __isInitialized;
}

void SimulationManager::pause(){
    __isRunning = false;
}

void SimulationManager::reasume(){
    __isRunning = true;
}

void SimulationManager::changeSpeed(int speed){
    __speedMultiplier = speed;
}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

void SimulationManager::__initialize(boost::property_tree::ptree& cities,
                                     boost::property_tree::ptree& transports,
                                     boost::property_tree::ptree& agents,
                                     boost::property_tree::ptree& map_parameters)
{
    
    // Initialize basic values
    __isInitialized = true;
    __isRunning = false;
    __speedMultiplier = 1;
    __simulationTime = std::chrono::milliseconds(0);

    // Initialize Transport structures
    unsigned int transports_numer = transports.count("transports");
    __transports.reserve(transports_numer);

    int id = 0;
    for(auto transport : transports.get_child("transports")){
        ++id;
        __transports.push_back(Transport(
            std::to_string(id),
            transport.second.get<unsigned int>("load"),
            transport.second.get<std::string>("from"),
            transport.second.get<std::string>("to")
        ));
    }

    // Initialize agents
    for(auto city : cities.get_child("cities")){
        unsigned int vehicles = city.second.get<unsigned int>("vehicles");
        for(unsigned int i = 0; i < vehicles; ++i){
            __agents.push_back(Agent(
                city.first,
                i + 1,
                agents
            ));
        }
    }

    // Distribute transports
    for(auto& transport : __transports){
        unsigned int amount_to_transport = transport.getLoad();
        for(auto& agent : __agents){
            // If some load left to dispose
            if(amount_to_transport > 0){
                // For agents that reside in the transport's origin
                // and have no transport assigned yet
                if(agent.getHome() == transport.getOrigin() &&
                   agent.getTransport() == nullptr)
                {
                    agent.initializeTransport(
                        &transport,
                        transport.getOrigin(),
                        transport.getDestination(),
                        (amount_to_transport > agent.getMaxLoad()) ? agent.getMaxLoad() : amount_to_transport,
                        map_parameters
                    );
                    amount_to_transport -= agent.getLoad();
                }   
            }
            // Else, break from agents loop
            else
                break;
        }
        // If all stuff distributed, mark transport as accomplishable
        if(amount_to_transport == 0)
            transport.setAccomplishability(true);
    }

}