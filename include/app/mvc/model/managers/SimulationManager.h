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

class SimulationCreatorManager;

class SimulationManager
{
// Constructors & Destructors
public:

    SimulationManager() = default;

//Interface
public:

    bool isSimulationRunning();

// Private friends
private:

    friend class SimulationCreatorManager;

// Private members
private:



// Private member methods
private:

    void __runSimulation();

};

#endif