/**
 * @file Model.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Model's methods implementations
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Model.h"
#include "SimulationManager.h"


/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

Model::Model(const std::string& client_id)
    : settingsManager(client_id)
    , simulationManager(client_id)
    , simulationCreatorManager(client_id, simulationManager)
{}

/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/
