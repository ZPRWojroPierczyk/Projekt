#define BOOST_TEST_MODULE AppTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

namespace ut = boost::unit_test;

/* --- Model unit tests --- */
extern void modelStatesTest();


ut::test_suite* init_unit_test_suite( int, char*[] )
{
    // Check if test initialized properly
    if(ut::framework::master_test_suite().argc > 1 )
        return 0;

    /* Model tests */
    ut::test_suite* modelSuite = BOOST_TEST_SUITE("Model suite");
    modelSuite->add(BOOST_TEST_CASE(&modelStatesTest));

    return 0;
}




