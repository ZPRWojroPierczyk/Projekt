/**
 * @file SimulationManager.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Defining SimulationManager.h
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "SimulationManager.h"
#include "Simulation.h"

SimulationData SimulationManager::simulate(SimulationState state, SimulationParameters parameters){
    Simulation simulation;
    simulation.setup(state, parameters);
    simulation.run();

    SimulationData data;
    data = simulation.get();
    
    return data;
}

