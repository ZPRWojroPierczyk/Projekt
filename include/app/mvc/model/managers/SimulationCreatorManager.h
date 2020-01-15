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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "SimulationManager.h"

/**
 * @brief 
 * 
 */
class SimulationCreatorManager
{
// Constructors & Destructors
public:
    SimulationCreatorManager(const std::string& clientID, SimulationManager& simulationManager);
    ~SimulationCreatorManager();

// Interface
public:
    /// Runs simulation with set parameters
    void runSimulation();

    /// Getters used to transform simulation attributes into JSON format
    std::string getCities();
    std::string getTransports();
    std::string getAgentsParam();
    std::string getMapParams();
    std::string getTimeParams(); 

    /// Setters used to load attributes from the JSON format
    void getCities(const std::string& cities);
    void getTransports(const std::string& transports);
    void getAgentsParams(const std::string& agetnsParams);
    void getMapParams(const std::string& mapParams);
    void getTimeParams(const std::string& timeParams); 


// Private member fields
private:
    /// Identifier of the client's owning app instance 
    const std::string __clientID;

    /// Reference to the Simulation Manager
    SimulationManager& __simulationManager;

    /* --- Simulation's parameters --- */

    /// Tree of cities with their attributes
    boost::property_tree::ptree __cities;
    /// Tre of planned transports
    boost::property_tree::ptree __transports;
    /// Agent's attributes
    boost::property_tree::ptree __agentsParams;
    /// Map's parameters
    boost::property_tree::ptree __mapParams;
    /// Simulation's time parameters
    boost::property_tree::ptree __timeParams;

// Private member methods
private:
    void __loadCreator();
    void __loadDefaultCreator();
    void __saveCreator();        
};

#endif