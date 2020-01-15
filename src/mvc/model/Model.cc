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

Model::Model(const std::string& clientID) :
    simulationCreatorManager(clientID, simulationManager),
    settingsManager(clientID),
    __clientID(clientID),
    __state(State::Menu)
{}

/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Set's a new application's state
 * 
 * @param newState State to set
 */
void Model::setState(Model::State newState){
    __state = newState;
}

/**
 * @brief Returns current state of the application
 * 
 * @returns Model::State 
 */
Model::State Model::getState(){
    return __state;
}

/**
 * Undermentioned methods serve to switch application between available
 * states/views. Every switch validation should be served by the Model itself
 * and if such transition is not possible these methods should return
 * false (true otherwise). 
 */

/**
 * @brief Switch application to the main menu
 * 
 * @returns true Menu succesfully opened
 * @returns false Failed to open menu
 */
bool Model::openMenu(){
    setState(State::Menu);
    return true;
}

/**
 * @brief Switch application to the simulation creator
 * 
 * @returns true Simulation creator succesfully opened
 * @returns false Failed to open simulation creator
 */
bool Model::openSimulationCreator(){
    setState(State::SimulationCreator);
    return true;
}

/**
 * @brief Switch application to the simulation
 * 
 * @returns true Simulation succesfully opened ()
 * @returns false Failed to open simulation
 */
bool Model::openSimulation(){
    if(simulationManager.isSimulationRunning()){
        setState(State::Simulation);
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Switch application to the simulation loading window
 * 
 * @returns true Simulation load succesfully opened ()
 * @returns false Failed to oppen simulation load
 */
bool Model::openLoad(){
    setState(State::Load);
    return true;
}

/**
 * @brief Switch application to the settings
 * 
 * @returns true Settings succesfully opened ()
 * @returns false Failed to oppen settings
 */
bool Model::openSettings(){
    setState(State::Settings);
    return true;
}

/**
 * @brief Switch application to the credits
 * 
 * @returns true Credits succesfully opened
 * @returns false Failed to open creadits
 */
bool Model::openCredits(){
    setState(State::Credits);
    return true;
}