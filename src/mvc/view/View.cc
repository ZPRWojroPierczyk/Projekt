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

View::View(const std::shared_ptr<Model>& model,
           const std::string& docRoot) :
    __model(model),
    __docRoot(docRoot)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

void View::setDocRoot(const std::string& docRoot){
    __docRoot = docRoot;
}

std::string View::getResource(const std::string& target){

    // Build the path to the requested file
    std::string path = __pathCat(__docRoot, target);
    if(target.back() == '/')
        path.append("index.html");

    return path;
}

std::string View::getData(const DataType& requestedData){
    return "";
}



/*--------------------------------------------------------------------------------*/
/*---------------------------- Private member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

std::string View::__pathCat(const std::string& base, const std::string& path){
    
    // If base is empty or "/" there is no need to change path
    if(base.empty() || base == "/")
        return path;

    std::string result = base;

#if BOOST_MSVC

    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;

#else

    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(std::string(path));

#endif
    return result;
}