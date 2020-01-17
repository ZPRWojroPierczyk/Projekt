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

#ifndef POINT_H 
#define POINT_H
#include <utility>
#include <boost/numeric/ublas/vector.hpp>

/**
 * @brief Class represents a single point on the road.
 * 
 * Accumulates all essential informations about the route point as
 * velocity limits, accidents and traffics probabilities. As point
 * coordinates are represented by boost::numeric::ublas::vector instance
 * getVector() method makes it able to perform algebralical operations
 * on the points calculating e.g. their distance or direction pointed
 * by two Points
 * 
 */
class Point
{
// Constructors & Destructors
public:
    
    /**
     * @brief Construct a new Point object. Initializes Point with all
     *        required parameters. Throw exception if parameters are 
     *        invalid.
     * 
     * @param max_velocity Max velocity at the point of the route
     * @param crash_probabiliy Probability of the crash at the point of the route
     * @param traffic_probability Probability of the traffic at the point of the route
     * @param coordinates Coordinates of the point in [latitude; longitude]
     * 
     * @throw std::runtime_exception 
     */
    Point(int unsigned max_velocity,
          int unsigned crash_probabiliy,
          int unsigned traffic_probability,
          boost::numeric::ublas::vector<float> coordinates);

// Interface
public:

    /**
     * @brief States whether two points are close each other
     * 
     * @param point Point to compare
     * 
     * @returns true Points are close to each other
     * @returns false Points are not close each other
     */
    bool isClose(Point point) const;


    /* --- Convertions --- */

    /**
     * @return Vector representation of the point 
     */
    boost::numeric::ublas::vector<float> toVector() const;


    /* --- Getters & Setters --- */

    /**
     * @return int Max velocity in [km/h] 
     */
    int getMaxVelocity() const;

    /**
     * @return float Crash probability in [%]
     */
    float getCrashProbability()  const;

    /**
     * @return float Traffic Probability in [%] 
     */
    float getTrafficProbability()  const;

    /**
     * @return true Point is under the traffic
     * @return false Point is not under the traffic
     */
    bool isTraffic()  const;

    /**
     * @param traffic Traffic state to set
     */
    void setTraffic(bool isTraffic);

//Private constant members
private:

    /**
     * @brief Distance in [km] that indicates whether or not
     *        two Routes are running close to each other at
     *        some point.
     */
    static const unsigned int TOLERANCE = 1;

    /// Max velocity at the point
    const unsigned int __MAX_VELOCITY;

    /// Probability of the car crash in [%]
    const unsigned int __CRASH_PROBABILITY;

    /// Probability of the traffic in [%]
    const unsigned int __TRAFFIC_PROBABILITY;

    /// Coordinates of the point in [latitude; longitude]
    const boost::numeric::ublas::vector<float> __coordinates;

//Private members
private:

    /// States if traffic occured at the point
    bool __isTraffic;

};

#endif