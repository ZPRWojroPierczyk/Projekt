/**
 * @file App.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Main program creating MVC 
 * @details Single App Session (Single User)
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <iostream>

#include "Model.h"
#include "View.h"
#include "Controller.h"

/**
 * @brief Program main function
 * 
 * @return int 
 */
int main(){
    Model* model = new Model();
    View* view = new View();
    Controller* controller = new Controller();

    return 0;
}