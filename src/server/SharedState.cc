/**
 * @file SharedState.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief SharedState class' methods definitions
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "SharedState.h"

/**
 * @brief Construct a new Shared State:: Shared State object
 * 
 * @param doc_root Absolute path to the folder containing static files
 */
SharedState::SharedState(std::string doc_root)
    : doc_root_(std::move(doc_root))
{
}

/**
 * @brief Insert a new connection to the clients sessions' list
 * 
 * @param session 
 */
void SharedState::join(HttpSession& session){
    sessions_.insert(&session);
}

/**
 * @brief Remove a connection from the clients sessions' list
 * 
 * @param session 
 */
void SharedState::leave(HttpSession& session){
    sessions_.erase(&session);
}