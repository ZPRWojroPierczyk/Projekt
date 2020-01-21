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

        return __simulationAction(
            action_type.substr(
                std::strlen("simulation/"),
                action_type.length() - std::strlen("simulation/")
            ),
            body
        );

    }
    // Unknown action's group
    else{

        std::cerr << "\n" 
                  << "Unknown action: " << action_type << "."
                  << "\n"
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
        std::cerr << "\n"
                  << "Unknown creator's action: " << action_type << "."
                  << "\n"
                  << "No action performed."
                  << std::endl;
        return "";
    }

}

std::string Controller::__simulationAction(const std::string& action_type, const std::string& body){

    /**
     * @brief From simulation window a series of requests can be send to the server.
     *        We can distinguish three groups of requests:
     * 
     *             - requests for changing simulation's speed or
     *               pausing/reasuming simulation
     *              
     *             - request for simulation's snapshot, i.e. JSON-formatted
     *               set informations about actual state of the simulation
     * 
     *             - request for applying simulation's modifications
     *               introduced by the client
     *  
     */


    /* ------------------ Requests for changing speed or pausing/reasuming ------------------ */
    
    /**
     * @todo Implement incremental speed changes instead of fixed ones
     */
    if(action_type == "pause"){
        __model->simulationManager.pause();
        return "";
    }
    else if(action_type == "play"){
        __model->simulationManager.reasume();
        return "";
    }
    else if(action_type == "forward"){
        __model->simulationManager.changeSpeed(8);
        return "";
    }
    else if(action_type == "backward"){
        __model->simulationManager.changeSpeed(-8);
        return "";
    }
    else if(action_type == "fast-forward"){
        __model->simulationManager.changeSpeed(32);
        return "";
    }
    else if(action_type == "fast-backward"){
        __model->simulationManager.changeSpeed(-32);
        return "";
    }

    /* -------------- Requests for simulation's snapshot ------------- */

    else if(action_type == "snapshot-request"){
        return "snapshot";
    }

    /* ------------ Requests for applying simulation's modifications ----------- */

    else if(action_type == "modifications"){
        return "";
    }

    /* ---------------------- Unknown request ---------------------- */

    else{
        std::cerr << "\n" 
                  << "Unknown simulations's action: " << action_type << "."
                  << "\n"
                  << "No action performed."
                  << std::endl;
        return "";
    }

}