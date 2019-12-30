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

#include "SimulationManager.h"
#include "OptionsManager.h"

class Model{
    public:
        Model();
    private:
        SimulationManager simulationManager;
        OptionsManager optionsManager;
};

#endif