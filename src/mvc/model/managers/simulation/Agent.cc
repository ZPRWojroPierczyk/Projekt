/**
 * @file Point.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the Agent's class methods
 * @version 0.1
 * @date 2020-01-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <limits>
#include <utility>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Agent.h"

namespace pt = boost::property_tree;
namespace ul = boost::numeric::ublas;
namespace asio = boost::asio;
namespace beast = boost::beast;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Agent::Agent(const std::string& home, unsigned int id,
             boost::property_tree::ptree params)
    : __MAX_VELOCITY(params.get<unsigned int>("agent.maxSpeed"))
    , __MAX_LOAD(params.get<unsigned int>("agent.maxLoad"))
    , __MAX_DRIVE_TIME(params.get<unsigned int>("agent.maxDrivingTime"))
    , __GAP_TIME(params.get<unsigned int>("agent.breakTime"))
    , __BREAK_PROBABILITY(params.get<unsigned int>("agent.malfunctionProbability"))

    , __HOME(home)
    , __ID(id)
    
    ,__coordinates(ul::vector<float>(2))
{
    __transport = nullptr;
    
    __coordinates[0] = __coordinates[1] = 0;
    __towardPointIndex = 0;

    /**
     * @todo As speed limits informations from google maps are not
     *       available, some simplification is taken and by default
     *       all agents move with maximum velocity all the time.
     * 
     *       It should be changed so that agents match their velocities
     *       to the actual road.
     */
    __velocity = __MAX_VELOCITY;
    __load = 0;

    __origin = std::move(std::string(""));
    __destination = std::move(std::string(""));

    __isGap = false;
    __isBroken = false;
    __isCrashed = false;
}

Agent::Agent(const Agent& agent)
    : __MAX_VELOCITY(agent.__MAX_VELOCITY)
    , __MAX_LOAD(agent.__MAX_LOAD)
    , __MAX_DRIVE_TIME(agent.__MAX_DRIVE_TIME)
    , __GAP_TIME(agent.__GAP_TIME)
    , __BREAK_PROBABILITY(agent.__BREAK_PROBABILITY)

    , __HOME(agent.__HOME)
    , __ID(agent.__ID)

    ,__coordinates(ul::vector<float>(2))
{
    __transport = agent.__transport;
    
    __coordinates[0] = agent.__coordinates[0];
    __coordinates[1] = agent.__coordinates[1];
    __towardPointIndex = agent.__towardPointIndex;
    
    __velocity = agent.__velocity;
    __load = agent.__load;

    __origin = agent.__origin;
    __destination = agent.__destination;

    __isGap = agent.__isGap;
    __isBroken = agent.__isBroken;
    __isCrashed = agent.__isCrashed;   
}

/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

bool Agent::initializeTransport(Transport* transport,
                                const std::string& origin,
                                const std::string& destination,
                                unsigned int load,
                                boost::property_tree::ptree map_params)
{
    /* --- If load is larger than agent's capacity transport is impossible --- */
    
    if(load > __MAX_LOAD)
        return false;
    else{
        __origin = origin;
        __destination = destination;
        __load = load;
        __transport = transport;

        __isGap = false;
        __isBroken = false;
        __isCrashed = false;
    }
    
    /* --- Send request to google maps Directions API for a route --- */
    
    // Prepare contexts
    asio::io_context context;
    asio::ssl::context ssl_context(asio::ssl::context::tlsv12_client);

    // Request parameters
    std::string host = "maps.googleapis.com";
    std::string port = "443";
    int version = 11;
    // Request too google maps api
    std::string target = "/maps/api/directions/json?";
    target += "origin=" + origin + "&";
    target += "destination=" + destination + "&";
    target += "key=" + std::string(API_KEY);
    
    ssl_context.set_verify_mode(asio::ssl::verify_peer);
    ssl_context.set_default_verify_paths();

    asio::ip::tcp::resolver resolver(context);
    beast::ssl_stream<beast::tcp_stream> stream(context, ssl_context);

    if(! SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))    {
        beast::error_code ec{static_cast<int>(::ERR_get_error()), asio::error::get_ssl_category()};
        throw beast::system_error{ec};
    }
    
    // Look up the domain name
    auto const results = resolver.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    beast::get_lowest_layer(stream).connect(results);

    // Perform the SSL handshake
    stream.handshake(asio::ssl::stream_base::client);

    // Set up an HTTP GET request message
    beast::http::request<beast::http::string_body> req{
        beast::http::verb::get,
        target,
        version
    };
    req.set(beast::http::field::host, host);
    req.set(beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    // Send the HTTP request to the remote host
    beast::http::write(stream, req);

    // This buffer is used for reading and must be persisted
    beast::flat_buffer buffer;

    // Declare a container to hold the response
    beast::http::response<beast::http::dynamic_body> res;

    // Receive the HTTP response
    beast::http::read(stream, buffer, res);

    // Write the response to a stream
    std::stringstream response_stream(
        beast::buffers_to_string(res.body().data())
    );

    // Gracefully close the stream
    beast::error_code ec;
    stream.shutdown(ec);

    /**
     * @todo Usually, Google Maps API perform no-gracefull connection
     *       close that results in 'scocket truncated' error. We should
     *       handle ec though but in the way that would distinguish
     *       short read from a real error.
     */

    /* --- Find route for the transport from the API's response if possible --- */

    // Parse JSON response from Google Directions API
    pt::ptree directions_response;
    pt::read_json(response_stream, directions_response);

    // Check if tranport is accomplishable
    
    auto routes = directions_response.get_child("routes");
    auto legs = routes.begin()->second.get_child("legs");
    // Get distance in meters in both sides 
    unsigned int distance = 
        2 * legs.begin()->second.get<unsigned int>("distance.value");
    
    /**
     * @brief  Compute minimal time to perform transport. Minimal time is
     *         computed with an assumption that agent drive's with their
     *         maximum velocity all the time and does not have breaks
     *         during a whole route. Minimal time is calculated in minutes
    */
    unsigned int minimal_time_min = 
        distance / (__MAX_VELOCITY * 1000.0f / 60.0f);

    /**
     * @todo Here, transport is accountes as impossible, if a single agent
     *       cannot accomplish it. In a target app there will be possible
     *       to change vehicles between drivers so that it can be possible
     *       that such a transport could be done.
     */

    /**
     * @brief Minimal number of breaks during a route. We count one additional
     *        break for a driver after delivering transport to the destination.
     * 
     */
    unsigned int breaks_num =
        2 * ((minimal_time_min / 2) / (__MAX_DRIVE_TIME * 60)) + 1;

    // Minimum transport time with breaks in minutes
    unsigned int time_min = 
        minimal_time_min + breaks_num * __GAP_TIME;

    /**
     * @brief If optimisticly estimated transport time is greater than 8h
     *        (8 * 60 minutes) we can classify transport as impossible.
     */
    if(time_min > 8 * 60){
        __transport->setAccomplishability(false);
        return false;
    }
    // Otherwise, we initialize agent to go into the route
    else{

        /**
         * @todo In a target application each point of the route should have
         *       max_velocity set to the real-data-based value. For now,
         *       these data is not available, so velocity is unlimited. 
         */

        unsigned int max_velocity = std::numeric_limits<unsigned int>::max();
        unsigned int crash_probability = map_params.get<unsigned int>("accidentProbability");
        unsigned int traffic_probability = map_params.get<unsigned int>("congestionProbability");

        // Rserve space for vector of points used to initialize __route
        unsigned int steps_num = legs.begin()->second.count("steps");
        std::vector<Point> points;
        points.reserve(steps_num + 1);
        // Fill 'points'
        for(auto step :  legs.begin()->second.get_child("steps")){
            boost::numeric::ublas::vector<float> coordinates(2);
            coordinates[0] = step.second.get<float>("start_loaction.lat");
            coordinates[1] = step.second.get<float>("start_loaction.lng");
            points.push_back(Point(
                max_velocity,
                crash_probability,
                traffic_probability,
                coordinates
            ));
        }
        // Get the final point of the route
        auto last_step = --(legs.get_child("steps").end());
        boost::numeric::ublas::vector<float> coordinates(2);
        coordinates[0] = last_step->second.get<float>("end_location.lat");
        coordinates[1] = last_step->second.get<float>("end_location.lng");
        points.push_back(Point(
            max_velocity,
            crash_probability,
            traffic_probability,
            coordinates
        ));

        // Construct '__route' basing on the data from Google Directions API
        __route = std::move(Route(
            points,
            distance
        ));

        // Initialize start coordinates
        __coordinates = __route[0].toVector();

        // Initialize start direction
        __towardPointIndex = 1;

        // Reset drive tim
        __continuousDriveTime = std::chrono::milliseconds(0);

        return true;
    }
}

/*------------------------------- Getters & Setters ------------------------------*/

std::string Agent::getHome() const{
    return __HOME;
}

unsigned int Agent::getMaxLoad() const{
    return __MAX_LOAD;
}

unsigned int Agent::getLoad() const{
    return __load;
}                       

const Transport* Agent::getTransport() const{
    return __transport;
}