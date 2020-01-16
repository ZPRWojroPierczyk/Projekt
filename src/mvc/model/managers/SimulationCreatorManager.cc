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
    const std::string& clientID, SimulationManager& simulationManager) :
    __simulationManager(simulationManager)
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
    
    // if (__clientID found in database)
        // Load creator from database
    // else
        __loadDefaultCreator();
}

void SimulationCreatorManager::__loadDefaultCreator(){
    // pt::read_json(std::string(ROOT) + "/data/json-structures/cities_form.json", __cities);
    // pt::read_json(std::string(ROOT) + "/data/json-structures/transport_form.json", __transports);
    // pt::read_json(std::string(ROOT) + "/data/json-structures/agents_form.json", __agentsParams);
    // pt::read_json(std::string(ROOT) + "/data/json-structures/map_form.json", __mapParams);
    // pt::read_json(std::string(ROOT) + "/data/json-structures/time_form.json", __timeParams);
}

void SimulationCreatorManager::__saveCreator(){
    // pt::write_json("spike/ptree_json_save/cities.json", __cities);
    // pt::write_json("spike/ptree_json_save/transports.json", __transports);
    // pt::write_json("spike/ptree_json_save/agents.json", __agentsParams);
    // pt::write_json("spike/ptree_json_save/map.json", __mapParams);
    // pt::write_json("spike/ptree_json_save/time.json", __timeParams);
}