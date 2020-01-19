/**
 * @file View.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the View class
 * @version 0.1
 * @date 2020-01-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "View.h"


/*--------------------------------------------------------------------------------*/
/*------------------------ Constructors and destructors --------------------------*/
/*--------------------------------------------------------------------------------*/

View::View(const std::shared_ptr<Model>& model) :
    __model(model)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

std::string View::getResource(const std::string& target, bool& is_redirect){

    std::string response;

    /* Check whether request demands simulation site */
    if(target.find("simulate.html") != std::string::npos){

        // Check if simulation i running. If so, do not redirect request
        if(__model->simulationManager.isSimulationInitialized()){
            response = target;
            is_redirect = false;
        }
        // Else, set response to the index site and signal that redirection is required
        else{
            response = "/";
            is_redirect = true;
        }

    }
    /* Check whether request demands .css file (response to these requests can be changed basing on the theme)*/
    else if(target.find(".css") != std::string::npos){
        
        is_redirect = false;

        // Modify target
        auto actual_theme = __model->settingsManager.getTheme();
        switch(actual_theme){
            
            // Default theme
            case SettingsManager::Theme::DEFAULT:
                response = target;
                break;
            
            /**
             * @todo .css files constituting another themes should be placed
             *       in the /web/simple_client/css/themename_folder and should 
             *       be divided into index.css and sub-sites/file.css files.
             * 
             *       If theme other than default is set, the only thing to
             *       change in target string is to insert '/themename_folder'
             *       after '.../css'.
             * 
             */
        }

    }
    /* All other requests for files should be fullfilled unchanged*/
    else{
        response = target;
        is_redirect = false;
    }


    return response;
}

std::string View::getData(const std::string& requested_data){
    return "";
}



/*--------------------------------------------------------------------------------*/
/*---------------------------- Private member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/