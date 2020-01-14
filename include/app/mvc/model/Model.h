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
 *  - simulation creation
 *  - simulation processing
 *  - saved simulation loading
 *  - adjusting app's settings
 */
class Model{
// Constructors
public:
    Model(const std::string& clientID);

// Public member classes
public:
    /// Actual state of the application seen by the user
    enum class State {Menu,
                      SimulationCreator,
                      Simulation,
                      Load,
                      Settings,
                      Credits};

// Public methods                          
public:
    /// Methods changing displayed view of the application
    bool openMenu();
    bool openSimulationCreator();
    bool openSimulation();
    bool openLoad();
    bool openSettings();
    bool openCredits();

    /// Get actual state of the application (called by the View object)
    State getState();

// Public member fields
public:
    /// Module responsible for simulation creation (parameters introcuction)
    SimulationCreatorManager simulationCreatorManager_;
    /// Module managing simulation process
    SimulationManager simulationManager_;
    /// Module responsible for loading saved simulations
    LoadSimulationManager loadSimulationManager_;
    /// Module responsible for app's settings
    SettingsManager settingsManager_;

// Private member fields
private:
    /// ID of the client owning instance of the app
    std::string __clientID;

    /// Application's state
    State state_;

// Private methods:
private:
    void setState(Model::State);
};

#endif