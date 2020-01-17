/**
 * @file Model.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing Model declarations
 * @details The whole logic
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MODEL_H 
#define MODEL_H

#include <memory>
#include "SimulationCreatorManager.h"
#include "SimulationManager.h"
#include "LoadSimulationManager.h"
#include "SettingsManager.h"


/**
 * @brief Main model of the application. Top layer of the application state interface.
 * 
 * Model class represents a single instance of the application that can interact with
 * a user. It manages states of the application and legal transitions between them.
 * It contains modules responsible for particular functionalities like:
 *  
 *    - simulation creation
 *    - simulation processing
 *    - saved simulation loading
 *    - adjusting app's settings
 * 
 */
class Model
{
// Constructors
public:

    /**
     * @param client_id Client's ID used to interact with and appropriate
     *                 set of data from database, associated with the client.
     */
    Model(const std::string& client_id);

// Interface                 
public:

// Public member fields
public:

    /// Module responsible for app's settings
    SettingsManager settingsManager;
    /// Module managing simulation process
    SimulationManager simulationManager;
    /// Module responsible for simulation creation (parameters introcuction)
    SimulationCreatorManager simulationCreatorManager;
    /// Module responsible for loading saved simulations
    LoadSimulationManager loadSimulationManager;

};

#endif