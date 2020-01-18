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
#include <sqlite3.h>
#include <string>
#include <boost/property_tree/ptree.hpp>

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


    /* --- Getters & setters --- */
    
    /**
     * @returns Cities form in JSON format 
     */
    std::string getCities();

    /**
     * @param cities Cities form in JSON format
     */
    void setCities(const std::string& cities);

    /**
     * @returns Transports form in JSON format 
     */
    std::string getTransports();
    
    /**
     * @param transports Transports form in JSON format 
     */
    void setTransports(const std::string& transports);

    /**
     * @returns Agents form in JSON format 
     */
    std::string getAgentsParam();
    
    /**
     * @param agent_params Agents form in JSON format 
     */
    void setAgentsParams(const std::string& agents_params);

    /**
     * @return Map form in JSON format 
     */
    std::string getMapParams();
    
    /**
     * @param map_params Map form in JSON format 
     */
    void setMapParams(const std::string& map_params);


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

    /**
     * @brief Converts given attribute to a JSON-formatted string
     * 
     * @param attribute Ptree to convert
     * @returns Converted tree
     */
    std::string __getJSON(boost::property_tree::ptree attribute);


    /*----------------------------- SQLight operations -------------------------------*/

    /**
     * @param databse Pointer to the database object
     * @returns true 'Creator' table is in database
     * @returns false No 'Creator' table in database
     */
    bool __isTable(sqlite3* databse);

    /**
     * @brief Creates 'Creator' table in the database
     * 
     * @param database Pointer to the database object
     * @return true Table has been created
     * @return false Failed to create table
     */
    bool __createTable(sqlite3* database);
    
    /**
     * @param databse Pointer to the database object
     * @return true Client's record exists in 'Creator' table in database
     * @return false No Client's record exists in 'Creator' table in database
     */
    bool __isClientRecord(sqlite3* database);

    /**
     * @brief Loads creator's configuration from the database
     * 
     * @param database Pointer to the database object
     * @return true Configuration was loaded
     * @return false Failed to load configuartion
     */
    bool __loadRecord(sqlite3* database);

    /**
     * @brief Save's client's creator's configuration to the database
     *        as a new record. 
     * 
     * @param database Pointer to the database object
     * @return true New record has been created
     * @return false Failed to create a new record
     */
    bool __saveNew(sqlite3* database);

    /**
     * @brief Save's client's creator's configuration to the database
     *        updating exiting record. 
     * 
     * @param database Pointer to the database object
     * @return true Record has been updated
     * @return false Failed to update a record
     */
    bool __saveUpdate(sqlite3* database);
};

#endif