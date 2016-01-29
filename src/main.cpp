/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

// This program will make use of the ATOM solver to construct a transfer trajectory
// between two points in space.  

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <cstdlib>

#include <libsgp4/Globals.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Tle.h>

#include <SML/sml.hpp>
#include <SML/constants.hpp>
#include <SML/basicFunctions.hpp>


typedef double Real;
typedef std::vector< Real > Vector6;
typedef std::vector< Real > Vector3;
typedef std::vector< Real > Vector2;
typedef std::vector < std::vector < Real > > Vector2D;

int main(void)
{
    // conversion from km to m
    const double km2m = 1000; 
    
    // earth radius and diameter
    const double EarthRadius = kXKMPER * km2m; // unit m
    const double EarthDiam = 2 * EarthRadius;
    
    // grav. parameter 'mu' of earth
    const double muEarth = kMU*( pow( 10, 9 ) ); // unit m^3/s^2
    
    // vectors to store arrival and departure velocities for the transfer trajectory
    Vector3 DepartureVelocity( 3 );
    Vector3 ArrivalVelocity( 3 );

    // read the TLE file. Line based parsing, using string streams
    std::string line;
    int lineCounter = 1;
    std::ifstream tlefile( "../../src/ADRcatalog.txt" );
    
    if( !tlefile.is_open() )
        perror("error while opening file");

    while( std::getline(tlefile, line) )
    {
        std::cout << line.data() << std::endl;
        ++lineCounter;
        if( lineCounter == 4 )
        {
            lineCounter = 1;
            std::cout << std::endl;
        }
    }
    
   return EXIT_SUCCESS;
}

    


