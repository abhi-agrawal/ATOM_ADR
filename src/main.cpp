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
#include <iterator>

#include <utility>

#include <libsgp4/Globals.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Tle.h>

#include <Atom/atom.hpp>
#include <Atom/convertCartesianStateToTwoLineElements.hpp>

#include <SML/sml.hpp>
#include <SML/constants.hpp>
#include <SML/basicFunctions.hpp>
#include <SML/linearAlgebra.hpp>

#include <boost/array.hpp>

#include <pykep/src/lambert_problem.cpp>
#include <pykep/src/lambert_problem.h>
#include <pykep/src/keplerian_toolbox.h>


typedef double Real;
typedef std::vector < Real > Vector6;
typedef std::vector < Real > Vector3;
typedef std::vector < Real > Vector2;
typedef std::vector < std::vector < Real > > Vector2D;
typedef boost::array < Real, 3 > array3; 

//! Remove newline characters from string.
void removeNewline( std::string& string )
{
    string.erase( std::remove( string.begin( ), string.end( ), '\r' ), string.end( ) );
    string.erase( std::remove( string.begin( ), string.end( ), '\n' ), string.end( ) );
}

//! Convert SGP4 ECI object to state vector.
Vector6 getStateVector( const Eci state )
{
    Vector6 result( 6 );
    result[ 0 ] = state.Position( ).x;
    result[ 1 ] = state.Position( ).y;
    result[ 2 ] = state.Position( ).z;
    result[ 3 ] = state.Velocity( ).x;
    result[ 4 ] = state.Velocity( ).y;
    result[ 5 ] = state.Velocity( ).z;
    return result;
}

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
    
    std::ifstream tlefile( "../../src/catalog_rocketbodiesLEO_SAFE1000km.txt" );
    
    if( !tlefile.is_open() )
        perror("error while opening file");

    std::vector < Tle > tleObjects; // vector of TLE objects
    

    while( !tlefile.eof( ) )
    {
        std::vector < std::string > tleStrings;

        std::getline( tlefile, line ); // read line from the catalog file
        removeNewline( line ); // remove new line characters such as '/n'
        tleStrings.push_back( line );
        // std::cout << line << std::endl;

        std::getline( tlefile, line );
        removeNewline( line );
        tleStrings.push_back( line );
        // std::cout << line << std::endl;

        std::getline( tlefile, line );
        removeNewline( line );
        tleStrings.push_back( line );
        // std::cout << line << std::endl << std::endl;

        tleObjects.push_back( Tle( tleStrings[ 0 ], tleStrings[ 1 ], tleStrings[ 2 ] ) );
    }
    tlefile.close( );
    
    const int DebrisObjects = tleObjects.size( );
    std::cout << "Total debris objects = " << DebrisObjects << std::endl; 
    // for( int k = 0; k < DebrisObjects; k++ )
    // {
    //     std::cout << tleObjects[ k ].NoradNumber( ) << std::endl;
    // }

    // Real TOF = 1000.0; // time of flight in seconds
    const Real coasting = 500.0; // coasting time at a debris in seconds

    int counter = 0;
    int failCount = 0;
    int catchDepartureID;
    int catchArrivalID;
    int i;
    const int timeOfFlightSteps = 1000;

    while( counter != DebrisObjects - 1 )
    {
        
        try
        {
                i = counter;      
            // for( int i = counter; i < DebrisObjects; )
            // {
                Tle departureObject = tleObjects[ i ]; // first tle element is assumed to be the starting point and not the debris itself
                SGP4 sgp4Departure( departureObject );

                const DateTime departureEpoch( 2016, 2, 1 ); // year month day

                const Eci tleDepartureState = sgp4Departure.FindPosition( departureEpoch );
                const Vector6 departureState = getStateVector( tleDepartureState );

                array3 departurePosition;
                array3 departureVelocity;
                for( int j = 0; j < 3; j++ )
                {
                    departurePosition[ j ] = departureState[ j ];
                    departureVelocity[ j ] = departureState[ j + 3 ];
                }
                const int departureObjectId = static_cast< int >( departureObject.NoradNumber( ) );
                // std::cout << "departure Object ID = " << departureObjectId << std::endl;
                catchDepartureID = departureObjectId; // for the catch segment of the program
                
                // Loop over arrival objects.
                // for ( int m = 0; m < DebrisObjects - 1; m++ )
                // {
                    // Skip the case of the departure and arrival objects being the same.
                    // if ( i == m )
                    // {
                        // continue;
                    // }

                    Tle arrivalObject = tleObjects[ i + 1 ];
                    SGP4 sgp4Arrival( arrivalObject );

                    const int arrivalObjectId = static_cast< int >( arrivalObject.NoradNumber( ) );
                    // std::cout << "Arrival Object ID = " << arrivalObjectId << std::endl;
                    catchArrivalID = arrivalObjectId;

                            // Loop over time-of-flight grid.
                    // for ( int p = 0; p < timeOfFlightSteps; p++ )
                    // {
                        const double TOF = 1000;

                        const DateTime arrivalEpoch = departureEpoch.AddSeconds( TOF );
                        const Eci tleArrivalState = sgp4Arrival.FindPosition( arrivalEpoch );
                        const Vector6 arrivalState = getStateVector( tleArrivalState );

                        array3 arrivalPosition;
                        array3 arrivalVelocity;
                        for( int j = 0; j < 3; j++ )
                        {
                            arrivalPosition[ j ] = arrivalState[ j ];
                            arrivalVelocity[ j ] = arrivalState[ j + 3 ];
                        } 

                        kep_toolbox::lambert_problem targeter( departurePosition, arrivalPosition, TOF, kMU, 0, 5 );
                        const int numberOfSolutions = targeter.get_v1( ).size( );
                        std::vector< array3 > departureDeltaVs( numberOfSolutions ); // delta-V components at the departure point
                        std::vector< array3 > arrivalDeltaVs( numberOfSolutions );                
                        std::vector< Real > transferDeltaVs( numberOfSolutions ); // magnitude of the total delta-V of one transfer between two points

                        for ( int j = 0; j < numberOfSolutions; j++ )
                        {
                            array3 transferDepartureVelocity = targeter.get_v1( )[ j ]; // velocity of the s/c at the departure point in the transfer orbit
                            array3 transferArrivalVelocity = targeter.get_v2( )[ j ];

                            departureDeltaVs[ j ] = sml::add( transferDepartureVelocity, sml::multiply( departureVelocity, -1.0 ) );
                            arrivalDeltaVs[ j ] = sml::add( transferArrivalVelocity, sml::multiply( arrivalVelocity, -1.0 ) );

                            transferDeltaVs[ j ] = sml::norm< Real >( departureDeltaVs[ j ] ) + sml::norm< Real >( arrivalDeltaVs[ j ] );
                        }

                        const std::vector< Real >::iterator minDeltaVIterator = std::min_element( transferDeltaVs.begin( ), transferDeltaVs.end( ) );
                        const int minimumDeltaVIndex = std::distance( transferDeltaVs.begin( ), minDeltaVIterator );

                        
                        array3 minIndexDepartureVelocity = targeter.get_v1( )[ minimumDeltaVIndex ]; // best guess for velocity in transfer orbit at the departure point
                        Vector3 departureVelocityGuess( 3 );
                        Vector3 atomDeparturePosition( 3 );
                        Vector3 atomArrivalPosition( 3 );
                        
                        for( int j = 0; j < 3; j++ )
                        {
                            departureVelocityGuess[ j ] = minIndexDepartureVelocity[ j ];
                            atomDeparturePosition[ j ] = departurePosition[ j ];
                            atomArrivalPosition[ j ] = arrivalPosition[ j ];
                        }

                        array3 atomDepartureVelocity;
                        array3 atomArrivalVelocity;
                        // typedef std::pair < Vector3, Vector3 > Velocities;
                        std::string SolverStatusSummary;
                        int numberOfIterations;
                        const int maxIterations = 100;
                        const Tle referenceTle = departureObject;
                        Vector6 atomVelocities( 6 );

                        atomVelocities = atom::executeAtomSolver< Real, Vector3, Vector6 >( atomDeparturePosition, departureEpoch, 
                                                                                            atomArrivalPosition, TOF, departureVelocityGuess, 
                                                                                            SolverStatusSummary, numberOfIterations, 
                                                                                            referenceTle, kMU, kXKMPER, 1.0e-10, 1.0e-5, maxIterations );

                        std::cout << "!!!!!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!" << std::endl;
                        std::cout << "Departure ID = " << departureObjectId << std::endl;
                        std::cout << "Arrival ID = " << arrivalObjectId << std::endl;
                        

                        for( int k = 0; k < 3; k++)
                        {
                            atomDepartureVelocity[ k ] = atomVelocities[ k ];
                            atomArrivalVelocity[ k ] = atomVelocities[ k + 3 ];
                        }
                        
                        array3 atomDepartureDeltaV;
                        array3 atomArrivalDeltaV;
                        Real AtomDeltaV;

                        atomDepartureDeltaV = sml::add( atomDepartureVelocity, sml::multiply( departureVelocity, -1.0 ) );
                        atomArrivalDeltaV = sml::add( atomArrivalVelocity, sml::multiply( arrivalVelocity, -1.0 ) );

                        AtomDeltaV = sml::norm< Real >( atomDepartureDeltaV ) + sml::norm< Real >( atomArrivalDeltaV );

                        std::cout << "Atom Total Delta V = " << AtomDeltaV << std::endl << std::endl;
                        std::cout << "Lambert Delta V = " << transferDeltaVs[ minimumDeltaVIndex ] << std::endl;
                        std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
                // }
            // }
                // ++counter;
            // }
        }
          
        catch( const std::exception& err )   
        {
            ++failCount;
            // std::cout << "Exception Caught = " << err.what( ) << std::endl;
            // std::cout << "For departure ID = " << catchDepartureID << " " << "For Arrival ID = " << catchArrivalID << std::endl << std::endl;
            // std::cout << "Fail count = " << failCount << std::endl;
        }  
        ++counter;
    } 
   return EXIT_SUCCESS;
}