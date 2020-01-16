/**
 * @file View.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the Controller class
 * @version 0.1
 * @date 2020-01-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Controller.h"


/*--------------------------------------------------------------------------------*/
/*------------------------ Constructors and destructors --------------------------*/
/*--------------------------------------------------------------------------------*/


Controller::Controller(const std::shared_ptr<Model>& model):
    __model(model)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/


View::DataType Controller::action(const std::string& actionType, const std::string& body){
    return View::DataType::None;
}