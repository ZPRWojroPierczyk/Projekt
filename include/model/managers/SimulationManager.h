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

/**
 * @brief Class representing the manager of simulation
 * @todo Think data structures over
 * @see SimulationData, SimulationParameters, SimulationState
 */
class SimulationManager{
    private:
        SimulationState state;

        SimulationParameters parameters;

        SimulationData data;
    
    public:
        SimulationManager();

        SimulationState getState();
        SimulationParameters getParameters();

        void setParameters();
        
        SimulationData simulate(SimulationState state, SimulationParameters parameters);
        SimulationData update();
};


class SimulationData{
    //some data structure
};

class SimulationParameters{
    //some parameters structure
};

enum SimulationState{
    //some simulation states
};

#endif