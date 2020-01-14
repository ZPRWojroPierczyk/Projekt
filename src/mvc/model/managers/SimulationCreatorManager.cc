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

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Creates a new settings menager for the app's instance and
 *        loads client's settings from the data base
 * @param clientID 
 */
SimulationCreatorManager::SimulationCreatorManager(const std::string& clientID) :
    __clientID(clientID)
{
    __loadCreator();
}

/**
 * @brief Destroy the Simulatoion Creator Manager and saves actual creator to the database
 */
SimulationCreatorManager::~SettingsManager(){
    __saveCreator();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @todo Implement creator's loading
 * @brief Loads client's creator's state from the database 
 */
void SimulationCreatorManager::__loadCreator(){
    
}

/**
 * @todo Implement creator's saving
 * @brief Saves client's creator's state to the database
 */
void SimulationCreatorManager::__saveCreator(){

}