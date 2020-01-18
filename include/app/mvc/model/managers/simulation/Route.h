/**
 * @file Agent.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Header file containing Route class declarations
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef ROUTE_H 
#define ROUTE_H
#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include "Point.h"

/**
 * @brief Route class represents an agent's route computed by the planner.
 * 
 * This class is a basic route containing only basic informations about
 * route involving set of points constituting aproximate shape of the road
 * and summary lenth. Class shares simple interface to get basic
 * informations about the route.
 * 
 */
class Route
{
// Constructors & Destructors
public:

    /**
     * @brief Construct a new empty Route object
     */
    Route();

    /**
     * @brief Construct a new Route object with a given path
     *        and length
     * 
     * @param __path Vector of points constituting route
     * @param __length Length of the route in [km]
     */
    Route(std::vector<Point> __path, int __length);

// Interface
public:

    /**
     * @brief Move assighnment operator
     * 
     * @param route Route to move
     * @returns Reference to this
     */
    Route& operator=(Route&& route);

    /* --- Semantic operations on the Route --- */

    /**
     * @brief Method return set of pair of indices from both
     *        routes points arrays indicating which pairs of points
     *        are close each other.
     * 
     * @returns Vector of pairs of indices. First index of each pair
     *          is index for this object and the second is index for
     *          object passed as argument.
     */
    std::vector<std::pair<unsigned int, unsigned int>> isClose(const Route& route) const;


    /* --- Geometrical operations on the Route --- */

    /**
     * @param index Index of the point in the intermediate points array 
     * @returns Point of the path with a given index
     */
    Point operator[](unsigned int index) const;


    /**
     * @returns Number of indices in the array of intermediate points
     */
    unsigned int size() const;

//Private members
private:

    /// List of points constituting the road
    std::vector<Point> __path;

    /// Length of the road in [km]
    unsigned int __length;

};

#endif