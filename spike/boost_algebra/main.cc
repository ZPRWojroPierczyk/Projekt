#include <iostream>
#include <boost/numeric/ublas/vector.hpp>

namespace ub = boost::numeric::ublas;

int main(int argc, char const *argv[])
{
    ub::vector<float> a(2);
    a[0] = 1.0;
    a[1] = 2;

    ub::vector<float> b(2);
    b[0] = 1.0;
    b[1] = 2;

    b = (a + b) * 2;

    std::cout << b[0] << "," << b[1] << std::endl;
    
    return 0;
}
