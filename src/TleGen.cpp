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

#include "CppProject/TleGen.hpp"


namespace TleGen{

	typedef double Real;
	typedef std::vector< Real > Vector6;
	typedef std::vector< Real > Vector3;
	typedef std::vector< Real > Vector2;
	typedef std::vector < std::vector < Real > > Vector2D;

	void TleGen( Vector6 randKepElem, std::string& SolverStatus, int& IterationCount )
	{
		// grav. parameter 'mu' of earth
    	const double muEarth = kMU*( pow( 10, 9 ) ); // unit m^3/s^2
	    // generate sets of cartesian elements corresponding to each of the psuedo random orbital element set. 
	    // The conversion from keplerian elements to the cartesian elements is done using the PyKep library from ESA.
	    Vector3 CartPos( 3 ); // empty vector to store position coordinates
	    Vector3 CartVel( 3 ); // empty vector to store velocity components
	    // Real rangeMag = 0; // magnitude range
	    // Real velocityMag = 0; // velocity magnitude
	    kep_toolbox::par2ic( randKepElem, muEarth, CartPos, CartVel );
	   
	    // verification of the conversion process using Ron Noomen's lecture notes from TUDelft
	    // Document ID ae4878.basics.v4-16.pdf
	    // Vector6 testKep = { 6787746.891, 0.000731104, sml::convertDegreesToRadians( 51.68714486 ), 
	    //     sml::convertDegreesToRadians( 127.5486706 ), sml::convertDegreesToRadians( 74.21987137 ), sml::convertDegreesToRadians( 24.08317766 ) };
	    // Vector3 testPos( 3 );
	    // Vector3 testVel( 3 );
	    // kep_toolbox::par2ic( testKep, muEarth, testPos, testVel );    
	    // std::cout << testPos[ 0 ] << std::endl;
	    // std::cout << testPos[ 1 ] << std::endl;
	    // std::cout << testPos[ 2 ] << std::endl;
	    // std::cout << testVel[ 0 ] << std::endl;
	    // std::cout << testVel[ 1 ] << std::endl;
	    // std::cout << testVel[ 2 ] << std::endl;

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
	    const Real absTol = 1.0e-10; // absolute tolerance
	    const Real relTol = 1.0e-5; // relative tolerance
	    const int maxItr = 100; // maximum allowed iterations per conversion run
	    
	    // important note, the atom function converting cartesian to TLEs takes in values in km and km/s.
	    cartesianState[ 0 ] = CartPos[ 0 ]/1000;
	    cartesianState[ 1 ] = CartPos[ 1 ]/1000;
	    cartesianState[ 2 ] = CartPos[ 2 ]/1000;
	    cartesianState[ 3 ] = CartVel[ 0 ]/1000;
	    cartesianState[ 4 ] = CartVel[ 1 ]/1000;
	    cartesianState[ 5 ] = CartVel[ 2 ]/1000;
		convertedTle = atom::convertCartesianStateToTwoLineElements< Real, Vector6 >( cartesianState, DateTime( ), SolverStatus, 
	    	IterationCount, referenceTle, kMU, kXKMPER, absTol, relTol, maxItr );
	}
}