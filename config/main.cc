#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <boost/chrono.hpp>
#include <unordered_map>
#include <functional>
#include <algorithm>

using namespace std;

int main(int argc, char const *argv[])
{
    char val[] {'a', 'b', 'c', 'd', 'e'};

    unordered_map<string, char*> map {
        {"A", val},
        {"B", val+1},
        {"D", val+2},
        {"C", val+3},
        {"E", val+4}
    };

    for_each(
        map.begin(), map.end(),
        [](pair<string, char*> record){
            *record.second += 1;
        }
    );

    for (auto x : map){
        std::cout << x.first << ":" << *x.second << std::endl;
    }
}
