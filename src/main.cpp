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
    int lineCounter = 0;
    std::ifstream tlefile( "../../src/ADRcatalog.txt" );
    
    if( !tlefile.is_open() )
        perror("error while opening file");

    int counter = 0;
    std::string line1, line2, line3;
    Vector2D DebrisPosition( 20, std::vector < Real >( 3 ) ); // 20 TLEs
    while( !tlefile.eof( ) )
    {
        std::getline( tlefile, line1 );
        std::getline( tlefile, line2 );
        std::getline( tlefile, line3 );
        Tle tle( line2, line3 );
        SGP4 sgp4( tle );
        Eci StateVector = sgp4.FindPosition( 0.0 );    
        DebrisPosition[ counter ][ 0 ] = StateVector.Position( ).x;
        DebrisPosition[ counter ][ 1 ] = StateVector.Position( ).y;
        DebrisPosition[ counter ][ 2 ] = StateVector.Position( ).z;
        ++counter; 
    }
    tlefile.close( );

    Vector3 DeparturePosition( 3 ); // km
    Vector3 ArrivalPosition( 3 ); // km
    Vector3 DepartureVelocityGuess( 3 ); //km/s
        
   return EXIT_SUCCESS;
}

    


