/**
 * @file Controller.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing Controller declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef CONTROLLER_H 
#define CONTROLLER_H
#include <memory>
#include "Model.h"

class Controller{
// Constructors
public:
    Controller(const std::shared_ptr<Model>& model);

// Interface
public:
    bool action(const std::string& actionType, const std::string& actionBody);

// Private members
private:
    std::shared_ptr<Model> __model;
};

#endif