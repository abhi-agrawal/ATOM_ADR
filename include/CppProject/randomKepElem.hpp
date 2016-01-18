/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef CPP_PROJECT_RANDOM_KEP_ELEM_HPP
#define CPP_PROJECT_RANDOM_KEP_ELEM_HPP

namespace randomKepElem
{

//! Compute multiple sets of random keplerian orbital elements
/*!
 * Multiple sets of keplerian orbital elements are generated based on user
 * defined ranges for each orbital element and user defined total number of 
 * random sets required. The lower limit of the range is inclusive and the upper limit is exclusive
 * when the random number is being generated. 
 * @param  	const vector2 range_a( 2 ); // min max value for the range of semi major axis
 * @param	const vector2 range_e( 2 );  // min max value for the range of eccentricity
 * @param	const vector2 range_i( 2 );  // min max value for the range of inclination
 * @param	const vector2 range_raan( 2 );  // min max value for the range of raan
 * @param	const vector2 range_w( 2 );  // min max value for the range of argument of perigee
 * @param	const vector2 range_E( 2 );  // min max value for the range of eccentric anomoly
 * @param	const int limit; // number of random numbers to be generated  
 * @param  	std::vector< std::vector< Real > > returnKepElem( limit, std::vector< Real >( 6 ) ); // 
 * 			a two dimensional matrix to store multiple instances of random orbital element.
 * @return type of the function is void. Final result obtained by pass by reference method. 
 */
typedef double Real;
typedef std::vector< Real > Vector2;
typedef std::vector< std::vector <Real > > Vector2D;

void randomKepElem( const Vector2 range_a,
					const Vector2 range_e,
					const Vector2 range_i,
					const Vector2 range_raan,
					const Vector2 range_w,
					const Vector2 range_E,
					const int limit,
					Vector2D& returnKepElem);
} // namespace randomKepElem

#endif // CPP_PROJECT_RANDOM_KEP_ELEM_HPP
