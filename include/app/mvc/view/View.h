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
// Public Types
public:
    enum class DataType{
        None,
        // Data send to simulation creator
        CreatorCities,
        CreatorTransports,
        CreatorAgents,
        CreatorMap,
        CreatorTime
    };

// Constructors
public:
    View(const std::shared_ptr<Model>& model,
         const std::string& docRoot);

// Interface
public:
    void setDocRoot(const std::string& docRoot);
    std::string getResource(const std::string& target);
    std::string getData(const DataType& requestedData);

// Private members
private:
    /// Pointer to the model
    const std::shared_ptr<Model>& __model;
    /// Path to the folder containing web static files
    std::string __docRoot;

// Private member methods
private:
    std::string __pathCat(const std::string& base, const std::string& path);
};

#endif