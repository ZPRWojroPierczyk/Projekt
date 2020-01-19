/**
 * @file SettingsManager.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief 
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <iostream>
#include <string>
#include <boost/property_tree/json_parser.hpp>
#include "SimulationCreatorManager.h"

namespace pt = boost::property_tree;

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

SimulationCreatorManager::SimulationCreatorManager(
    const std::string& client_id, SimulationManager& simulation_manager) :
    __clientID(client_id),
    __simulationManager(simulation_manager)
{
    __loadCreator();
}

SimulationCreatorManager::~SimulationCreatorManager(){
    __saveCreator();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

void SimulationCreatorManager::initializeSimulation(){
    __simulationManager.__initialize();
    return;
}

/*------------------------------- Getters & Setters ------------------------------*/

std::string SimulationCreatorManager::getCities(){
    return __getJSON(__cities);
}

std::string SimulationCreatorManager::getTransports(){
    return __getJSON(__transports);
}

std::string SimulationCreatorManager::getAgentsParam(){
    return __getJSON(__agentsParams);
}

std::string SimulationCreatorManager::getMapParams(){
    return __getJSON(__mapParams);
}

void SimulationCreatorManager::setCities(const std::string& cities){
    std::stringstream stream(cities);
    pt::read_json(stream, __cities);
}

void SimulationCreatorManager::setTransports(const std::string& transports){
    std::stringstream stream(transports);
    pt::read_json(stream, __transports);
}

void SimulationCreatorManager::setAgentsParams(const std::string& agents_params){
    std::stringstream stream(agents_params);
    pt::read_json(stream, __agentsParams);
}

void SimulationCreatorManager::setMapParams(const std::string& map_params){
    std::stringstream stream(map_params);
    pt::read_json(stream, __mapParams);
}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

void SimulationCreatorManager::__loadCreator(){
    
    // Open a databes with users' configuartions
    sqlite3 *db;
    int rc;
    std::string db_name = std::string(ROOT) + "/data/users/users_config.sqlite3";
    rc = sqlite3_open( db_name.c_str() ,&db);

    // If canot open database, load default configuration of creator 
    if(rc){
        __loadDefaultCreator();
        std::cerr << std::endl
                  << "Cannot open database containing users' configs!"
                  << std::endl
                  << "Default creator loaded."
                  << std::endl;
        sqlite3_close_v2(db);
        return;
    }

    // Check if 'Creator' table exists in database
    bool table_exists = __isTable(db);


    // If no table exists in the database, create one
    if(!table_exists){

        // Load default parameters from forms
        __loadDefaultCreator();

        // Close database
        sqlite3_close_v2(db);

    }
    // Else, try to load configuration from database
    else{

        bool record_exists = __isClientRecord(db);

        // If no client's record exists, load default configuration
        if(!record_exists){

            // Load default parameters from forms
            __loadDefaultCreator();

            // Close database connection
            sqlite3_close_v2(db);

        }
        // Else, load existing configuartion
        else{
            
            bool configuration_loaded = __loadRecord(db);

            if(!configuration_loaded)
                // Load default parameters from forms
                __loadDefaultCreator();

            // Close database connection
            sqlite3_close_v2(db);

        }
    }
}

void SimulationCreatorManager::__saveCreator(){

    // Open a databes with users' configuartions
    sqlite3 *db;
    int rc;
    std::string db_name = std::string(ROOT) + "/data/users/users_config.sqlite3";
    rc = sqlite3_open( db_name.c_str() ,&db);

    // If canot open database, load default configuration of creator 
    if(rc){
        __loadDefaultCreator();
        std::cerr << std::endl
                  << "Cannot open database containing users' configs!"
                  << std::endl
                  << "Default creator loaded."
                  << std::endl;
        sqlite3_close_v2(db);
        return;
    }

    // Check if 'Creator' table exists in database
    bool table_exists = __isTable(db);

    // If table does not exists create one and create new record for the client
    if(table_exists == 0){
    
        /* ----------------- Create table ----------------- */

        bool was_table_created = __createTable(db);
        
        if(!was_table_created){
            sqlite3_close_v2(db);
            return;
        }

        /* ---------------- Create record ----------------- */

        __saveNew(db);

        // Close database connection
        sqlite3_close_v2(db);

    }
    // Else, if table exists, check if client's record exists
    else{

        bool record_exists = __isClientRecord(db); 

        // If client's record does not exist yet, create one
        if(record_exists == 0){

            __saveNew(db);

            // Close database connection
            sqlite3_close_v2(db);

        }
        // Else, if client's record exists, update it
        else{
            
            __saveUpdate(db);

            // Close database connection
            sqlite3_close_v2(db);
            
        }
    }
}

void SimulationCreatorManager::__loadDefaultCreator(){
    pt::read_json(std::string(ROOT) + "/data/json-structures/creator-forms/cities.json", __cities);
    pt::read_json(std::string(ROOT) + "/data/json-structures/creator-forms/transports.json", __transports);
    pt::read_json(std::string(ROOT) + "/data/json-structures/creator-forms/agents.json", __agentsParams);
    pt::read_json(std::string(ROOT) + "/data/json-structures/creator-forms/map.json", __mapParams);
}

std::string SimulationCreatorManager::__getJSON(boost::property_tree::ptree attribute){
    std::stringstream stream;
    pt::json_parser::write_json(stream, attribute);
    std::string result;
    stream >> result;
    return result;
}


/*----------------------------- SQLight operations -------------------------------*/

bool SimulationCreatorManager::__isTable(sqlite3* databse){

    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;   

    // Variable used to count number of records in the database
    int count = 0;

    // Prepare a querry to check whether 'Creator' tables already exists
    std::string statement_string = 
        std::string("SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name='Creator';");
    
    const char* check_table_statement = statement_string.c_str();

    // Perform querry
    rc = sqlite3_exec(
            databse,
            check_table_statement,
            [](void *count, int argc, char **argv, char **azColName)
            {
                // Save result of the 'COUNT' querry to 'count' variable
                int *c = reinterpret_cast<int*>(count);
                *c = atoi(argv[0]);
                return 0;
            },
            &count,
            &err_msg
    );
    
    // Load default parameters and close database if execution failed
    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot prepare sql statement to use!"
                  << std::endl
                  << "Default creator loaded."
                  << std::endl;
        count = 0;
    }

    // Free variable alocated by the sqlite3
    sqlite3_free(err_msg);

    // Return bool basing on the count value
    return (count ? true : false);
}

bool SimulationCreatorManager::__createTable(sqlite3* database){

    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;       
    
    // Prepare SQL querry to create table
    std::string statement_string = 
        std::string("CREATE TABLE Creator(") +
                    "ID TEXT PRIMARY       KEY      NOT NULL," +
                    "Cities                TEXT     NOT NULL," +
                    "Transports            TEXT     NOT NULL," +
                    "Agents                TEXT     NOT NULL," +
                    "MapParams             TEXT     NOT NULL);";

    const char* create_table_statement = statement_string.c_str();
    
    // Perform querry
    rc = sqlite3_exec(database, create_table_statement, nullptr, nullptr, nullptr);

    // Report if table could not be created
    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot create table in the database!"
                  << std::endl
                  << "Creator's configuration has not been saved."
                  << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    sqlite3_free(err_msg);
    return true;
}

bool SimulationCreatorManager::__isClientRecord(sqlite3* database){

    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;   

    // Variable used to count number of records in the database
    int count = 0;

    // Perform a querry to check if Client's row exists in the table
    std::string statement_string = 
        std::string("SELECT COUNT(ID) FROM Creator WHERE ID='") + 
                    __clientID + std::string("';");

    const char* record_check_statement = statement_string.c_str();

    // Perform querry
    rc = sqlite3_exec(
            database,
            record_check_statement,
            [](void *count, int argc, char **argv, char **azColName)
            {
                int *c = reinterpret_cast<int*>(count);
                *c = atoi(argv[0]);
                return 0;
            },
            &count,
            &err_msg
    );

    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot check for client's existance in the database!"
                  << std::endl
                  << "Default creator loaded."
                  << std::endl;
        count = 0;
    }

    // Free variable allocated by the sqlite3
    sqlite3_free(err_msg);

    // Return bool basing on the count value
    return (count ? true : false);
}

bool SimulationCreatorManager::__loadRecord(sqlite3* database){
    
    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;

    // Prepare SQL statement
    std::string statement_string = 
        std::string("SELECT * FROM Creator WHERE ID='") + 
                    __clientID + std::string("';");

    const char* load_record_statement = statement_string.c_str();

    // Prepare structure with pointers to all property_trees
    struct trees_references{
        pt::ptree* cities;
        pt::ptree* transports;
        pt::ptree* agents;
        pt::ptree* map_params; 
    } references;

    references.cities = &__cities;
    references.transports = &__transports;
    references.agents = &__agentsParams;
    references.map_params = &__mapParams;

    // Execute SQL statement - load configurations from database
    rc = sqlite3_exec(
            database,
            load_record_statement,
            [](void *data, int argc, char **argv, char **azColName)
            {
                    trees_references* references =
                        reinterpret_cast<trees_references*>(data);

                    // Read JSON configs from database
                    std::stringstream cities_conf(argv[1]);
                    std::stringstream transports_conf(argv[2]);
                    std::stringstream agents_conf(argv[3]);
                    std::stringstream map_params_conf(argv[4]);

                    // Load configs to parameters trees
                    pt::read_json(cities_conf, *(references->cities));
                    pt::read_json(transports_conf, *(references->transports));
                    pt::read_json(agents_conf, *(references->agents));
                    pt::read_json(map_params_conf, *(references->map_params));

                    return 0;
            },
            &references,
            &err_msg
    );    

    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot load configuration from the database record!"
                  << std::endl
                  << "Default creator loaded."
                  << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    // Close database connection
    sqlite3_free(err_msg);
    return true;
}

bool SimulationCreatorManager::__saveNew(sqlite3* database){

    // Prepare strings with configuration to save
    std::stringstream cities_stream;
    std::stringstream transports_stream;
    std::stringstream agents_stream;
    std::stringstream map_params_stream;

    pt::write_json(cities_stream, __cities);
    pt::write_json(transports_stream, __transports);
    pt::write_json(agents_stream, __agentsParams);
    pt::write_json(map_params_stream, __mapParams);

    std::string cities_conf(cities_stream.str());
    std::string transports_conf(transports_stream.str());
    std::string agents_conf(agents_stream.str());
    std::string map_params_conf(map_params_stream.str());

    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;

    // Prepare SQL statement to insert client
    std::string statement_string = 
        std::string("INSERT INTO Creator (ID, Cities, Transports, Agents, MapParams) VALUES (") 
                    + "'" + __clientID + "'"  + ","
                    + "'" + cities_conf + "'"  + ","
                    + "'" + transports_conf + "'"  +  ","
                    + "'" + agents_conf + "'"  + ","
                    + "'" + map_params_conf + "'"  + ");";

    const char* insert_record_statement = statement_string.c_str();
    
    // Perform querry
    rc = sqlite3_exec(database, insert_record_statement, nullptr, nullptr, nullptr);

    // Report if table could not be created
    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot insert record to the database!"
                  << std::endl
                  << "Creator's configuration has not been saved."
                  << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    sqlite3_free(err_msg);
    return true;
}

bool SimulationCreatorManager::__saveUpdate(sqlite3* database){

    // Prepare strings with configuration to save
    std::stringstream cities_stream;
    std::stringstream transports_stream;
    std::stringstream agents_stream;
    std::stringstream map_params_stream;

    pt::write_json(cities_stream, __cities);
    pt::write_json(transports_stream, __transports);
    pt::write_json(agents_stream, __agentsParams);
    pt::write_json(map_params_stream, __mapParams);

    std::string cities_conf(cities_stream.str());
    std::string transports_conf(transports_stream.str());
    std::string agents_conf(agents_stream.str());
    std::string map_params_conf(map_params_stream.str());

    // Variables used to perform SQL querries
    int rc;
    char* err_msg = nullptr;

    // Prepare SQL statement to insert client
    std::string statement_string = 
        std::string("UPDATE Creator SET ") 
                    +     "Cities='" + cities_conf + "', "
                    +     "Transports='" + transports_conf + "', "
                    +     "Agents='" + agents_conf + "', "
                    +     "MapParams='" + map_params_conf + "' "
                    + "WHERE ID='" + __clientID + "';";

    const char* update_record_statement = statement_string.c_str();

    // Perform querry
    rc = sqlite3_exec(database, update_record_statement, nullptr, nullptr, nullptr);

    // Report if table could not be created
    if(rc != SQLITE_OK){
        std::cerr << std::endl
                  << "Cannot update client's record to the database!"
                  << std::endl
                  << "Creator's configuration has not been saved."
                  << std::endl;
        sqlite3_free(err_msg);
        return false;
    }

    sqlite3_free(err_msg);
    return true;

}