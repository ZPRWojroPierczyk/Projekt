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
 * @brief Class responsible for app's behaviour in the simulation creator
 * 
 * Class holds all parameters required by the creator in a shape
 * of parameters tress. It facilitates methods responsible for serializing
 * and deserializing parameters (within JSON format) tress as well as methods
 * performing loading and saving parameters to the database tables assiociated
 * with a client.
 * 
 * @note SimulationCreatorManager is the only one object that simulation can be
 *       started from in a valid way.
 * 
 * @see SimulationManager.h
 * 
 */
class SimulationCreatorManager
{
// Constructors & Destructors
public:

    /**
     * @brief Creates a new settings menager for the app's instance and
     *        loads client's settings from the data base
     * @param client_id 
     */
    SimulationCreatorManager(const std::string& client_id, SimulationManager& simulation_manager);

    /**
     * @brief Destroy the Simulatoion Creator Manager and saves actual creator to the database
     */
    ~SimulationCreatorManager();

// Interface
public:

    /**
     * @todo Implement SimulationCreatorManager::runSimulation()
     * @brief Runs simulation with set parameters
     */
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
    void getAgentsParams(const std::string& agents_params);
    void getMapParams(const std::string& map_params);
    void getTimeParams(const std::string& time_params); 


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

// Private member methods
private:

    /**
     * @todo Implement creator's loading
     * @brief Loads client's creator's state from the database 
     */
    void __loadCreator();
    
    /**
     * @brief Loads a default creator 
     */
    void __loadDefaultCreator();
    
    /**
     * @todo Implement creator's saving
     * @brief Saves client's creator's state to the database
     */
    void __saveCreator(); 
           
};

#endif