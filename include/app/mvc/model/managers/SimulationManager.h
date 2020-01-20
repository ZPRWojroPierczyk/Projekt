/**
 * @file SimulationManager.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing SimulationManager declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SIMULATION_MANAGER_H 
#define SIMULATION_MANAGER_H
#include <atomic>
#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include "Agent.h"
#include "Transport.h"

class SimulationCreatorManager;
class LoadSimulationManager;

/**
 * @brief Main program's class representing core of the simulation.
 * 
 * This class holds informations about simulation's core involving agents'
 * and transports' state. The class shares an interface that facilitates
 * simulation's flow controll. 
 * 
 */
class SimulationManager
{
// Constructors & Destructors
public:

    /**
     * @brief Constructs uninitialized simulation
     * 
     * @param client_id 
     */
    SimulationManager(const std::string& client_id);

//Interface
public:

    /* --- Simulation's state manipulation --- */

    /**
     * @returns true Simulation has been initialized
     * @returns false Simulation has not been initialized
     */
    bool isSimulationInitialized();

    /**
     * @brief Pauses simulation.
     */
    void pause();

    /**
     * @brief Reasumes simulation from a puse.
     */
    void reasume();

    /**
     * @param speed A new simulation's speed
     */
    void changeSpeed(int speed);


    /* --- Semantic actions --- */

    /**
     * @brief Optimises agents' routes to increase transports
     *        efficiency.
     */
    void optimise();

// Private friends
private:

    friend class SimulationCreatorManager;
    friend class LoadSimulationManager;

// Private constant members
private:

    /// Frequency of agents' positions refreshing
    const unsigned int __frequency = 20;

// Private members
private:

    const std::string __clientID;

    /* --- Simulation's state --- */

    bool __isInitialized;
    std::atomic<bool> __isRunning;
    
    /// Indicates speed of the simulated time flow
    std::atomic<int> __speedMultiplier;

    /// Simulated time in miliseconds since 10 p.m.
    std::chrono::milliseconds __simulationTime;


    /* --- Semantic informations --- */

    /// Table of active agents
    std::vector<Agent> __agents;

    /// Table of planned transports
    std::vector<Transport> __transports;

// Private member methods
private:

    /**
     * @brief Initializes the simulation with the given properties.
     * 
     * @param cities Informations about cities and available agents.
     * @param transports Informations about required transports.
     * @param agents Informations about agent's parameters.
     * @param map_parameters Informations about map's parameters.
     */
    void __initialize(boost::property_tree::ptree& cities,
                      boost::property_tree::ptree& transports,
                      boost::property_tree::ptree& agents,
                      boost::property_tree::ptree& map_parameters);

    /**
     * @brief Performs a single 'tick' of the simulation.
     *        Updates state of the agents and transpots.
     *        This method is designed to be thread-safe so that
     *        it could be run parallelly with the main thread.
     */
    void __tick();

};

#endif