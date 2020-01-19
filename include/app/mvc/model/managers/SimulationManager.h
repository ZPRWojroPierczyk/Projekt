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
#include <chrono>
#include "Agent.h"
#include "Transport.h"

class SimulationCreatorManager;
class LoadSimulationManager;

class SimulationManager
{
// Constructors & Destructors
public:

    /**
     * @brief Constructs uninitialized simulation
     */
    SimulationManager();

//Interface
public:

    /* --- Simulation's state manipulation --- */

    bool isSimulationInitialized();
    void pause();
    void reasume();
    void changeSpeed();

    /* --- Semantic actions --- */

    void optimise();

// Private friends
private:

    friend class SimulationCreatorManager;
    friend class LoadSimulationManager;

// Private members
private:

    /* --- Simulation's state --- */

    bool __isInitialized;
    bool __isRunning;
    
    /// Simulated time (between 10 p.m. and 6 a.m.)
    std::chrono::system_clock::time_point __time;

    /// Indicates speed of the simulated time flow
    int __speedMultiplier;


    /* --- Semantic informations --- */

    /// Table of active agents
    std::vector<Agent> __agents;

    /// Table of planned transports
    std::vector<Transport> __transports;

// Private member methods
private:

    void __initialize();
    void __run();

};

#endif