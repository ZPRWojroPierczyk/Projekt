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
#include <cstring>
#include <iostream>
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


std::string Controller::action(const std::string& action_type, const std::string& body){
    
    /**
     * @brief There are 4 groups of actions depending on the application's section
     *        where an action occured. Action type comes in 'group_name/action_name'
     *        form, where 'group_name' can be one of:
     *              - menu
     *              - options
     *              - creator
     *              - simulation
     * 
     *        The first step of action's evaluation is to establish group of the action
     *        and pass it's servicing to an appropriate method
     */

    // Actions performed in menu
    if(action_type.rfind("menu", 0) == 0){
        return "";
    }
    // Action performed in options panel
    else if(action_type.rfind("options", 0) == 0){
        return "";
    }
    // Actions performed in a creator
    else if(action_type.rfind("creator", 0) == 0){

        return __creatorAction(
            action_type.substr(
                std::strlen("creator/"),
                action_type.length() - std::strlen("creator/")
            ),
            body
        );

    }
    // Actions performed in a simulation
    else if(action_type.rfind("simulation", 0) == 0){
        return "";
    }
    // Unknown action's group
    else{

        std::cerr << std::endl 
                  << "Unknown action: " << action_type << "." << std::endl
                  << "No action performed."
                  << std::endl;
        return "";

    }
}



/*--------------------------------------------------------------------------------*/
/*---------------------------- Private member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

std::string Controller::__creatorAction(const std::string& action_type, const std::string& body){

    /**
     * @brief Creator's page can send to the server a series of requests associated
     *        with forms defining simulations parameters. We can distinguish three
     *        groups of requests:
     * 
     *             - request for form's data recently introduced by the
     *               client. Those requests are sent, when creator's page
     *               is loading and client-side scripts try to load
     *               default values for forms
     *              
     *             - request for making a change in client's forms saved
     *               on the server. Thoso requests are sent, when client
     *               submits a form in the creator panel.
     * 
     *             - request for starting a simulation
     *  
     */


    /* ------------------ Requests for forms data ------------------ */
    
    /**
     * @todo Implement servicing requests for sending actually saved
     *       forms data.
     */
    if(action_type == ""){
        return "";
    }

    /* -------------- Requests for changin saved forms ------------- */

    else if(action_type == "cities"){
        __model->simulationCreatorManager.setCities(body);
        return "";
    }
    else if(action_type == "transports"){
        __model->simulationCreatorManager.setTransports(body);
        return "";
    }
    else if(action_type == "agent"){
        __model->simulationCreatorManager.setAgentsParams(body);
        return "";
    }
    else if(action_type == "map-params"){
        __model->simulationCreatorManager.setMapParams(body);
        return "";
    }

    /* ------------ Requests for initializing simulation ----------- */

    else if(action_type == "success"){
        __model->simulationCreatorManager.initializeSimulation();
        return "";
    }

    /* ---------------------- Unknown request ---------------------- */

    else{
        std::cerr << std::endl 
                  << "Unknown creator's action: " << action_type << "." << std::endl
                  << "No action performed."
                  << std::endl;
        return "";
    }

}