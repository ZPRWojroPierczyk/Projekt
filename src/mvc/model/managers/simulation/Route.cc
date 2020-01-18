/**
 * @file Point.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the Route's class methods
 * @version 0.1
 * @date 2020-01-18
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <algorithm>
#include "Route.h"

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

Route::Route() :
    __length(0)
{}

Route::Route(std::vector<Point> path, int length) :
    __path(path),
    __length(length)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

Route& Route::operator=(Route&& route){
    
    __path = std::move(route.__path);
    __length = route.__length;
    route.__length = 0;

    return *this;
}

std::vector<std::pair<unsigned int, unsigned int>> Route::isClose(const Route& route) const{
    
    std::vector<std::pair<unsigned int, unsigned int>> close_points;

    /* --- Check every pair of points whether are close each other --- */
    for(unsigned int i = 0; i < __path.size(); ++i)
        for(unsigned int j = 0; j < route.__path.size(); ++j)
            if(__path[i].isClose(route.__path[j]))
                close_points.push_back(
                    std::pair<unsigned int, unsigned int>(i, j)
                );

    return close_points;

}

Point Route::operator[](unsigned int index) const{
    return __path[index];
}

unsigned int Route::size() const{
    return __path.size();
}