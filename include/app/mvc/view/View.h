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


/**
 * @brief View class constitutes and interface between a RequestHandler and the Model.
 * 
 * When RequestHandler gets a request from the client it parses it, optionally
 * calls Controller to inform Model about an action performed by the user and
 * calls an appropriate View's method to get response to be sent back.
 * 
 * There are two types of responses that View can provide RequestHandler with.
 * The first is a path to the static file, that should be sent back to the client
 * This kind of response is returned, when RequestHandler receives a GET request.
 * GET requests are always sent by the web browser and are associated with
 * some kind of static resources to be sent back to the client. In this scenario
 * RequestHandler calls View::getResource to get the Path to the file.
 * 
 * In the second scenario, RequestHandler receives POST request which indicates
 * user's action that was noticed by the local JavScript script and was sent to the
 * server. This situations doesn't have to require any data beeing sent back
 * to the client, but if so, RequestHandler calls View::getData that returns 
 * string containing a JSON-formatted data that should be sent back to the client.
 * 
 * All data that is returned to the RequestHandler is always based on the Model's
 * state and is independant from the Controller's state. 
 * 
 * @see RequestHandler.h
 * 
 */
class View
{
// Public Types
public:

    /**
     * @brief Enum of possible data types to be sent back to the
     *        client after receiving POST request. Every type is
     *        associated with exactly one action performed by the
     *        client.
     */
    enum class DataType{
        
        None,
        
        // Data send to simulation creator
        CreatorCities,
        CreatorTransports,
        CreatorAgents,
        CreatorMap,
        CreatorTime
    };

// Constructors & Destructors
public:

    /**
     * @brief Initializes a new instance of the View.
     * 
     * @param model Shared pointer to the Model instance
     * @param docRoot Absolute path to the folder containing web-files
     */
    View(const std::shared_ptr<Model>& model,
         const std::string& docRoot);

// Interface
public:


    /**
     * @param path : Path to the web-files root 
     */
    void setDocRoot(const std::string& docRoot);

    /**
     * @todo Implement Themes differencing
     * @todo Implement redirecting client back to the main site
     *       if simulation was not run and client tries to
     *       get to it by-hand.
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
    std::string getResource(const std::string& target);

    /**
     * @todo Implement fetching data from the Model
     * @brief Get the Data object
     * 
     * @param requestedData 
     * @return std::string 
     */
    std::string getData(const DataType& requestedData);

// Private members
private:

    /// Pointer to the model
    const std::shared_ptr<Model>& __model;

    /// Path to the folder containing web static files
    std::string __docRoot;

// Private member methods
private:

    /**
     * @brief Append an HTTP rel-path to a local filesystem path.
     * 
     * @param base Path to the doc's root folder
     * @param path Relative path to the resource
     * @return Returned path (normalized for the platform.)
     */
    std::string __pathCat(const std::string& base, const std::string& path);

};

#endif