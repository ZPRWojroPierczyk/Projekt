#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AppTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "Model.h"

BOOST_AUTO_TEST_CASE(ModelStatesTest){
    
    Model model;    

    model.openCredits();
    BOOST_CHECK(model.getState() == Model::State::Credits);
    model.openLoad();
    BOOST_CHECK(model.getState() == Model::State::Load);
    model.openMenu();
    BOOST_CHECK(model.getState() == Model::State::Menu);
    model.openSettings();
    BOOST_CHECK(model.getState() == Model::State::Settings);
    model.openSimulationCreator();
    BOOST_CHECK(model.getState() == Model::State::SimulationCreator);
    model.openSimulation();
    BOOST_CHECK(model.getState() == Model::State::SimulationCreator);
}