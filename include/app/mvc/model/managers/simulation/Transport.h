/**
 * @file Agent.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing Transprt class declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef TRANSPORT_H 
#define TRANSPORT_H
#include <string>

class Transport
{
// Constructors & Destructors
public:
    Transport() = default;

// Interface
public:

//Private members
private:

    /// States if the transport is accomplishable
    bool __isAccomplishable;

    /// Amount of stuff to transport
    int __load;

    /// Transport's origin
    std::string __origin;

    /// Transport's destination
    std::string __destination;

};

#endif