/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <execinfo.h>

#include <boost/exception/info.hpp>

#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_vector.h>
 	
#include <libsgp4/DateTime.h>
#include <libsgp4/Eci.h>
#include <libsgp4/Globals.h>
#include <libsgp4/SGP4.h>
#include <libsgp4/Tle.h>

#include <Astro/astro.hpp>
#include <SML/sml.hpp>
#include <SML/constants.hpp>
#include <SML/basicFunctions.hpp>

#include <Atom/printFunctions.hpp>

#include <Astro/orbitalElementConversions.hpp>
#include <Atom/convertCartesianStateToTwoLineElements.hpp>

#include "/media/abhishek/work/TU delft/INTERNSHIP at DINAMICA/work/github/pykep/src/core_functions/par2ic.h"

#include "CppProject/KepToCartToTLE.hpp"


namespace KepToCartToTLE{

	typedef double Real;
	typedef std::vector< Real > Vector6;
	typedef std::vector< Real > Vector3;
	typedef std::vector< Real > Vector2;
	typedef std::vector < std::vector < Real > > Vector2D;

	void KepToCartToTLE( int newLimit, Vector2D randKepElem )
	{
		// grav. parameter 'mu' of earth
    	const double muEarth = kMU*( pow( 10, 9 ) ); // unit m^3/s^2
	    // generate sets of cartesian elements corresponding to each of the psuedo random orbital element set. 
	    // The conversion from keplerian elements to the cartesian elements is done using the PyKep library from ESA.
	    Vector2D CartPos( newLimit, std::vector< Real >( 3 ) ); // empty 2D vector to store position coordinates
	    Vector2D CartVel( newLimit, std::vector< Real >( 3 ) ); // empty 2D vector to store velocity components
	    Vector6 tempKep( 6 ); // temporary storage vector to store a given set of keplerian elements
	    Vector3 tempPos( 3 ); // temp storage vector for cartesian position
	    Vector3 tempVel( 3 ); // temp storage vector for cartesian velocity
	    Real rangeMag = 0; // magnitude range
	    Real velocityMag = 0; // velocity magnitude
	    for(int k = 0; k < newLimit; k++)
	    {
	        tempKep = randKepElem[ k ]; // transferring an entire row of the 2D vector which contains a single set of orbital elements
	        kep_toolbox::par2ic( tempKep, muEarth, tempPos, tempVel );
	        CartPos[ k ] = tempPos; // storing the output position coordinates in a row of the final 2D vector
	        CartVel[ k ] = tempVel; // same as above comment but this time for velocity    
	    }
	    // verification of the conversion process using Ron Noomen's lecture notes from TUDelft
	    // Document ID ae4878.basics.v4-16.pdf
	    Vector6 testKep = { 6787746.891, 0.000731104, sml::convertDegreesToRadians( 51.68714486 ), 
	        sml::convertDegreesToRadians( 127.5486706 ), sml::convertDegreesToRadians( 74.21987137 ), sml::convertDegreesToRadians( 24.08317766 ) };
	    Vector3 testPos( 3 );
	    Vector3 testVel( 3 );
	    kep_toolbox::par2ic( testKep, muEarth, testPos, testVel );    
	    std::cout << testPos[ 0 ] << std::endl;
	    std::cout << testPos[ 1 ] << std::endl;
	    std::cout << testPos[ 2 ] << std::endl;
	    std::cout << testVel[ 0 ] << std::endl;
	    std::cout << testVel[ 1 ] << std::endl;
	    std::cout << testVel[ 2 ] << std::endl;

	    
	    //store randomly generated keplerian element sets into a CSV file for easy viewing and use in future debugging of ATOM
	    std::ofstream RandomKepElemFile;
	    RandomKepElemFile.open("RandomKepElemFile_deletedScenes.csv"); //file will be overwritten each time the code is run unless the name is changed here and the code recompiled
	    RandomKepElemFile << "semi-major axis [km]" << "," << "eccentricity" << ",";
	    RandomKepElemFile << "Inclination [deg]" << "," << "RAAN [deg]" << ",";
	    RandomKepElemFile << "AOP [deg]" << "," << "Eccentric Anomaly [deg]" << std::endl;
	    for(int i = 0; i < newLimit; i++)
	    {
	        RandomKepElemFile << ( randKepElem[ i ][ 0 ]/1000 ) << ",";
	        RandomKepElemFile << randKepElem[ i ][ 1 ] << ",";
	        RandomKepElemFile << sml::convertRadiansToDegrees( randKepElem[ i ][ 2 ] ) << ",";
	        RandomKepElemFile << sml::convertRadiansToDegrees( randKepElem[ i ][ 3 ] ) << ",";
	        RandomKepElemFile << sml::convertRadiansToDegrees( randKepElem[ i ][ 4 ] ) << ",";
	        RandomKepElemFile << sml::convertRadiansToDegrees( randKepElem[ i ][ 5 ] ) << std::endl;
	    }
	    RandomKepElemFile.close();


	    //store the converted cartesian elements in a CSV file 
	    std::ofstream RandomCartesianFile;
	    RandomCartesianFile.open("RandomCartesianFile_deletedScenes.csv");
	    RandomCartesianFile << "X [km]" << "," << "Y [km]" << "," << "Z [km]" << "," << "Range [km]" << ",";
	    RandomCartesianFile << "Vx [km/s]" << "," << "Vy [km/s]" << "," << "Vz [km/s]" << "," << "Velocity [km/s]" << std::endl;
	    for(int j = 0; j < newLimit; j++)
	    {
	        RandomCartesianFile << ( CartPos[ j ][ 0 ]/1000 ) << ",";
	        RandomCartesianFile << ( CartPos[ j ][ 1 ]/1000 ) << ",";
	        RandomCartesianFile << ( CartPos[ j ][ 2 ]/1000 ) << ",";
	        rangeMag = sqrt( pow( CartPos[ j ][ 0 ], 2 ) + pow( CartPos[ j ][ 1 ], 2 ) + pow( CartPos[ j ][ 2 ], 2 ) );
	        RandomCartesianFile << ( rangeMag/1000 ) << ",";

	        RandomCartesianFile << ( CartVel[ j ][ 0 ]/1000 ) << ",";
	        RandomCartesianFile << ( CartVel[ j ][ 1 ]/1000 ) << ",";
	        RandomCartesianFile << ( CartVel[ j ][ 2 ]/1000 ) << ",";
	        velocityMag = sqrt( pow( CartVel[ j ][ 0 ], 2 ) + pow( CartVel[ j ][ 1 ], 2 ) + pow( CartVel[ j ][ 2 ], 2 ) );
	        RandomCartesianFile << ( velocityMag/1000 ) << std::endl;
	    }
	    RandomCartesianFile.close();

	    // convert the cartesian elements to the corresponding TLE format using the ATOM toolbox
	    Vector6 cartesianState( 6 );
	    // cartesianState[ 0 ] = -7.1e3;
	    // cartesianState[ 1 ] = 2.7e3;
	    // cartesianState[ 2 ] = 1.3e3;
	    // cartesianState[ 3 ] = -2.5;
	    // cartesianState[ 4 ] = -5.5;
	    // cartesianState[ 5 ] = 5.5;
	    Tle convertedTle;
	    Tle referenceTle = Tle(); // empty TLE for reference
	    // std::cout << referenceTle << std::endl << std::endl;
	    std::string SolverStatus;
	    const Real absTol = 1.0e-10; // absolute tolerance
	    const Real relTol = 1.0e-5; // relative tolerance
	    const int maxItr = 100; // maximum allowed iterations per conversion run
	    // some other bookkeeping variables, these are not used in the convert to tle function
	    std::size_t findSuccess;
	    // file storage
	    std::ofstream tlefile;
	    tlefile.open("TLEfile.csv");
	    tlefile << "Conversion Status" << "," << "Iteration Count" << "," << "," << "Converted TLE" << "," << "Failure/Success Index" << std::endl;

	    for(int i = 0; i < newLimit; i++)
	    {
	        // std::cout << "loop count = " << i << std::endl;
	        int IterationCount = 0; // counter for total iterations undertaken in a given instance of cartesian to TLE conversion
	        // important note, the atom function converting cartesian to TLEs takes in values in km and km/s.
	        cartesianState[ 0 ] = CartPos[ i ][ 0 ]/1000;
	        cartesianState[ 1 ] = CartPos[ i ][ 1 ]/1000;
	        cartesianState[ 2 ] = CartPos[ i ][ 2 ]/1000;
	        cartesianState[ 3 ] = CartVel[ i ][ 0 ]/1000;
	        cartesianState[ 4 ] = CartVel[ i ][ 1 ]/1000;
	        cartesianState[ 5 ] = CartVel[ i ][ 2 ]/1000;
	        convertedTle = atom::convertCartesianStateToTwoLineElements< Real, Vector6 >( cartesianState, DateTime( ), SolverStatus, 
	            IterationCount, referenceTle, kMU, kXKMPER, absTol, relTol, maxItr );
	        findSuccess = SolverStatus.find("success");    
	        if(findSuccess == std::string::npos)
	        {
	            std::cout << "Cartesian to TLE Conversion Failed" << std::endl;
	            tlefile << "Failure" << ",";
	            tlefile << "," << "," << "," << "," << "," << i+2 << std::endl;

	        }
	        else
	        {
	            tlefile << "Success" << ",";
	            tlefile << IterationCount << "," << ",";
	            tlefile << "Epoch = " << convertedTle.Epoch() << "," << i+2 << std::endl;
	            tlefile << "," << "," << "," << "Mean motion Dt2 = " << convertedTle.MeanMotionDt2() << std::endl;
	            tlefile << "," << "," << "," << "Mean motion Ddt6 = " << convertedTle.MeanMotionDdt6() << std::endl;
	            tlefile << "," << "," << "," << "B(*) = " << convertedTle.BStar() << std::endl;
	            tlefile << "," << "," << "," << "Inclination = " << convertedTle.Inclination(1) << std::endl;
	            tlefile << "," << "," << "," << "RAAN = " << convertedTle.RightAscendingNode(1) << std::endl;
	            tlefile << "," << "," << "," << "Eccentricity = " << convertedTle.Eccentricity() << std::endl;
	            tlefile << "," << "," << "," << "AOP = " << convertedTle.ArgumentPerigee(1) << std::endl;
	            tlefile << "," << "," << "," << "Mean Anomaly = " << convertedTle.MeanAnomaly(1) << std::endl;
	            tlefile << "," << "," << "," << "Mean motion = " << convertedTle.MeanMotion() << std::endl << std::endl;
	        }   
	    }
	    tlefile.close();
	}
}