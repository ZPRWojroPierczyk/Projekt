#define BOOST_TEST_MODULE AppTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

namespace ut = boost::unit_test;

/* --- Server unit tests --- */
extern void serverConstructorTest();
extern void serverRunTest();
extern void serverJoinLeaveTest();
extern void serverLoadConfigTest();

extern void listenerConstructorTest();

extern void httpSessionConstructorTest();

/* --- Model unit tests --- */
extern void modelStatesTest();

ut::test_suite* init_unit_test_suite( int, char*[] )
{
    // Check if test initialized properly
    if(ut::framework::master_test_suite().argc > 1 )
        return 0;

    /* Server tests */
    ut::test_suite* serverSuite = BOOST_TEST_SUITE("Server suite");
    serverSuite->add(BOOST_TEST_CASE(&serverConstructorTest));
    serverSuite->add(BOOST_TEST_CASE(&serverRunTest));
    serverSuite->add(BOOST_TEST_CASE(&serverJoinLeaveTest));
    serverSuite->add(BOOST_TEST_CASE(&serverLoadConfigTest));

    serverSuite->add(BOOST_TEST_CASE(&listenerConstructorTest));

    serverSuite->add(BOOST_TEST_CASE(&httpSessionConstructorTest));
    
    /* Model tests */
    ut::test_suite* modelSuite = BOOST_TEST_SUITE("Model suite");
    modelSuite->add(BOOST_TEST_CASE(&modelStatesTest));

    return 0;
}




