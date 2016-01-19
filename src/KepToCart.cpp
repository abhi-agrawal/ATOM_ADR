// test program to convert orbital elements to ECI cartesian elements for elliptical orbit case.
// literature taken from ccar.colorado.edu/ASEN5070/handouts/kep2cart_2002.doc
// verification done through Ron Noomen TU Delft lecture notes
// argument inputs in SI units m and rads

#include <cmath>
#include <vector>

#include "CppProject/KepToCart.hpp"

namespace KepToCart{
	typedef double Real;
	typedef std::vector< Real > Vector6;
	typedef std::vector< Real > Vector3;

	void KepToCart(Vector6 KepElem, Real mu, Vector3& Pos, Vector3& Vel)
	{
		// extract the orbital elements
		Real a 		= KepElem[ 0 ]; // semi major axis
		Real e 		= KepElem[ 1 ]; // eccentricity
		Real i 		= KepElem[ 2 ]; // inclination
		Real raan 	= KepElem[ 3 ]; // right ascension
		Real w 		= KepElem[ 4 ]; // Argument of perigee
		Real E 		= KepElem[ 5 ]; // Eccentric anomaly

		//compute true anomaly from eccentric anomaly
		Real temp = std::sqrt( (1 + e)/(1 - e) ) * std::tan( E/2 ); // an intermediate calculation for true anomaly
		Real trueAnomalyRad = 2 * std::atan( temp ); // true anomaly in radians

		// compute the radius r
		Real r = a * (1 - e * std::cos( E ));

		// compute specific angular momentum
		Real h = std::sqrt( mu * a * (1 - std::pow( e, 2 )) );

		// compute the position components
		Pos[ 0 ] = r * ( std::cos( raan ) * std::cos( w + trueAnomalyRad ) - std::sin( raan ) * std::sin( w + trueAnomalyRad ) * std::cos( i ) ); // x coordinate
		Pos[ 1 ] = r * ( std::sin( raan ) * std::cos( w + trueAnomalyRad ) + std::cos( raan ) * std::sin( w + trueAnomalyRad ) * std::cos( i ) ); // y coordinate
		Pos[ 2 ] = r * ( std::sin( i ) * std::sin( w + trueAnomalyRad ) ); // zcoordinate

		// compute velocity components
		Real p = a * ( 1 - std::pow( e, 2 ) ); // semi latus rectum
		Vel[ 0 ] = ( Pos[ 0 ] * h * e ) / ( r * p ) * std::sin( trueAnomalyRad ) - ( h/r ) * ( std::cos( raan ) * std::sin( w + trueAnomalyRad ) + std::sin( raan ) * std::cos( w + trueAnomalyRad ) * std::cos( i ) );
		Vel[ 1 ] = ( Pos[ 1 ] * h * e ) / ( r * p ) * std::sin( trueAnomalyRad ) - ( h/r ) * ( std::sin( raan ) * std::sin( w + trueAnomalyRad ) - std::cos( raan ) * std::cos( w + trueAnomalyRad ) * std::cos( i ) );
		Vel[ 2 ] = ( Pos[ 1 ] * h * e ) / ( r * p ) * std::sin( trueAnomalyRad ) + ( h/r ) * ( std::sin( i ) * std::cos( w + trueAnomalyRad ) );
	}
}
