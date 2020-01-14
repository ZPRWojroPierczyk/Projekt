/**
 * @file SimulationCreationManager.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class responsible for managing simulation's creator
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SIMULATION_CREATOR_MANAGER_H 
#define SIMULATION_CREATOR_MANAGER_H
#include <string>

class SimulationCreatorManager
{
// Constructors & Destructors
public:
    SimulationCreatorManager(const std::string& clientID);
    ~SimulationCreatorManager();

// Interface
public:

// Private member fields
private:
    /// Identifier of the client's owning app instance 
    const std::string __clientID;

// Private member methods
private:
    void __loadCreator();
    void __saveCreator();        
};

#endif