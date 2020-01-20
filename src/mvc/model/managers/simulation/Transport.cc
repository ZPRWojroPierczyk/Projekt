/**
 * @file Transport.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of Transport class' methods
 * @version 0.1
 * @date 2020-01-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Transport.h"

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Transport::Transport(const std::string& id,
                     unsigned int load,
                     const std::string& origin,
                     const std::string& destination)
    : __ID(id)
    , __LOAD(load)
    , __ORIGIN(origin)
    , __DESTINATION(destination)
    , __isAccomplishable(false)
{}

Transport::Transport(const Transport& transport)
    : __ID(transport.__ID)
    , __LOAD(transport.__LOAD)
    , __ORIGIN(transport.__ORIGIN)
    , __DESTINATION(transport.__DESTINATION)
{
    __isAccomplishable = transport.__isAccomplishable;
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/
    
std::string Transport::getID() const{
    return __ID;
}

unsigned int Transport::getLoad() const{
    return __LOAD;
}

std::string Transport::getOrigin() const{
    return __ORIGIN;
}

std::string Transport::getDestination() const{
    return __DESTINATION;
}

void Transport::setAccomplishability(bool accomplishability){
    __isAccomplishable = accomplishability;
}