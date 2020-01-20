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

/**
 * @brief Class represents a single transport created by the client.
 * 
 * Every transport holds four values: origin, destination, amount
 * and information if the transport is accomplishable. These informations
 * can be displayed at the simulation's screen.
 * 
 */
class Transport
{
// Constructors & Destructors
public:

    /**
     * @brief Construct a new Transport. Default definition
     *        se __isAccomplishable field to false.
     * 
     * @param id
     * @param load 
     * @param origin 
     * @param destination 
     */
    Transport(const std::string& id,
              unsigned int load,
              const std::string& origin,
              const std::string& destination);

    /**
     * @brief Copying constructor
     * @param transport 
     */
    Transport(const Transport& transport);

// Interface
public:

    /* --------- Getters & setters --------- */

    /**
     * @return Transport's identifier 
     */
    std::string getID() const;

    /**
     * @return Amount of stuff to transport 
     */
    unsigned int getLoad() const;

    /**
     * @return Transport's origin 
     */
    std::string getOrigin() const;

    /**
     * @return Transport's destination 
     */
    std::string getDestination() const;

    /**
     * @param accomplishability 
     */
    void setAccomplishability(bool accomplishability);

// Private const members
private:

    /// Transport's identifier
    const std::string __ID;

    /// Amount of stuff to transport
    const unsigned int __LOAD;

    /// Transport's origin
    const std::string __ORIGIN;

    /// Transport's destination
    const std::string __DESTINATION;

//Private members
private:

    /// States if the transport is accomplishable
    bool __isAccomplishable;

};

#endif