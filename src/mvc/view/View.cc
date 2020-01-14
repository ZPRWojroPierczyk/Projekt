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

View::View(std::shared_ptr<Model>& model,
           const std::string& docRoot) :
    __model(model),
    __docRoot(docRoot)
{}

/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @returns Path to the web-files root 
 */
std::string View::getDocRoot(){
    return __docRoot;
}

/**
 * @param path : Path to the web-files root 
 */
void View::setDocRoot(const std::string& docRoot){
    __docRoot = docRoot;
}
