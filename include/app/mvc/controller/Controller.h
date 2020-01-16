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
#include "View.h"


/**
 * @brief Controller class is an interface between RequestHandler and Model instance.
 *  
 * RequestHandler parses HTTP POST requests (that client's
 * action are sent with by the local JavaScript scripts) and passes
 * parsed action to the Controller. Controller's aim is to interpret
 * this action and call appropriate methods on the Model.  
 * 
 * @see Model.h
 * @see RequestHandler.h
 */
class Controller{
// Constructors
public:

    /**
     * @param model Shared pointer to the Model instance
     */
    Controller(const std::shared_ptr<Model>& model);

// Interface
public:

    /**
     * @brief Evaluates action performed by the the client. HTTP request
     *        needs to be parsed by the RequestHandler first that call's
     *        this method passing actionType (that is passed with the
     *        Content-Type header) and request's body.
     * 
     * @param actionType 
     * @param body Data associated with an action
     * @return Type of the data that View instance should be requested for.
     */
    View::DataType action(const std::string& actionType, const std::string& body);

// Private members
private:

    /// Pointer to the Model instance
    std::shared_ptr<Model> __model;

};

#endif