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

/**
 * @param path : Path to the web-files root 
 */
void View::setDocRoot(const std::string& docRoot){
    __docRoot = docRoot;
}


/**
 * TODO#CPP : Implement Themes differencing
 * 
 * @brief Evaluates path to the requested resource. If resource is
 *        a .css files method modifies target basing on actual Theme
 *        set in the model. Else, it returns passed path without changes.
 *        In both cases _docRoot path is inserted at the beggining of
 *        the target.
 * 
 * @param target Relative path to the requested resource
 * @return Evaluated path to the requested reource
 */
std::string View::getResource(const std::string& target){

    // Build the path to the requested file
    std::string path = __pathCat(__docRoot, target);
    if(target.back() == '/')
        path.append("index.html");

    return path;
}



/**
 * @brief Get the Data object
 * 
 * @param requestedData 
 * @return std::string 
 */
std::string View::getData(const DataType& requestedData){
    return "";
}

/*--------------------------------------------------------------------------------*/
/*---------------------------- Private member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Append an HTTP rel-path to a local filesystem path.
 * 
 * @param base Path to the doc's root folder
 * @param path Relative path to the resource
 * @return std::string Returned path (normalized for the platform.)
 */
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