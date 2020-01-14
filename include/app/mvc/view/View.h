/**
 * @file View.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing View declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef VIEW_H 
#define VIEW_H
#include <memory>
#include <thread>
#include "Model.h"

class View{
// Constructors
public:
    View(std::shared_ptr<Model>& model,
         const std::string& docRoot);

// Interface
public:
    std::string getDocRoot();
    void setDocRoot(const std::string& docRoot);

// Private members
private:
    /// Pointer to the model
    const std::shared_ptr<Model>& __model;
    /// Path to the folder containing web static files
    std::string __docRoot;
};

#endif