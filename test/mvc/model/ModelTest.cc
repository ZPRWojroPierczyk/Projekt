/**
 * @file ModelTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Set of tests for Model class
 * @version 0.1
 * @date 2020-01-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <boost/test/unit_test.hpp>
#include "Model.h"

BOOST_AUTO_TEST_SUITE( MVCSuite )
BOOST_AUTO_TEST_SUITE( ModelClassSuite )

BOOST_AUTO_TEST_CASE( modelStatesTest )
{
    Model model;    
    // Check credits transition
    model.openCredits();
    BOOST_CHECK(model.getState() == Model::State::Credits);
    // Check load transition
    model.openLoad();
    BOOST_CHECK(model.getState() == Model::State::Load);
    // Check menu transition
    model.openMenu();
    BOOST_CHECK(model.getState() == Model::State::Menu);
    // Check settings transition
    model.openSettings();
    BOOST_CHECK(model.getState() == Model::State::Settings);
    // Check simulation creator transition
    model.openSimulationCreator();
    BOOST_CHECK(model.getState() == Model::State::SimulationCreator);
    
    /** Check simulation transition
    * @note Here state should not change, as simulation has not been
    *       initialized and user has no acces to it yet.
    */
    model.openSimulation();
    BOOST_CHECK(model.getState() == Model::State::SimulationCreator);
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()