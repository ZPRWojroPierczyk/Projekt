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

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

SimulationManager::SimulationManager(){
    
}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool SimulationManager::isSimulationInitialized(){
    return __isInitialized;
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

void SimulationManager::__initialize(){
    __isInitialized = true;
}