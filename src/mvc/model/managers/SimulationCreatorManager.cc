/**
 * @file SettingsManager.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief 
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "SimulationCreatorManager.h"

namespace pt = boost::property_tree;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

SimulationCreatorManager::SimulationCreatorManager(
    const std::string& client_id, SimulationManager& simulation_manager) :
    __clientID(client_id),
    __simulationManager(simulation_manager)
{
    __loadCreator();
}

SimulationCreatorManager::~SimulationCreatorManager(){
    __saveCreator();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

void runSimulation(){

}

/*------------------------------------ Getters -----------------------------------*/

std::string SimulationCreatorManager::getCities(){
    std::stringstream stream;
    pt::json_parser::write_json(stream, __cities);
    std::string cities;
    stream >> cities;
    return cities;
}

std::string getTransports();
std::string getAgentsParam();
std::string getMapParams();
std::string getTimeParams(); 



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

void SimulationCreatorManager::__loadCreator(){

}

void SimulationCreatorManager::__loadDefaultCreator(){

}

void SimulationCreatorManager::__saveCreator(){

}