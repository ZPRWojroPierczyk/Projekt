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
#include "SimulationCreationManager.h"
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
        Model();
    // Public member classes
    public:
        /// Actual state of the application seen by the user
        enum class State {Menu,
                          SimulationCreation,
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

    // Private member fields
    private:

        State state;

        /// Module responsible for simulation creation (parameters introcuction)
        SimulationCreationManager simulationCreationManager;
        /// Module managing simulation process
        SimulationManager simulationManager;
        /// Module responsible for loading saved simulations
        LoadSimulationManager loadSimulationManager;
        /// Module responsible for app's settings
        SettingsManager settingsManager;
    // Private methods:
    private:
        bool setState(Model::State);
};

#endif