/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <vector>

#include "CppProject/randomKepElem.hpp"

namespace randomKepElem
{
typedef double Real;
typedef std::vector< Real > Vector6; 
typedef std::vector< Real > Vector2;
typedef std::vector< Real > Vector3;
typedef std::vector< Real > VectorOrb;
typedef std::vector< std::vector <Real > > Vector2D;
typedef boost::mt19937 generator_type; // mt19937 is a psuedo random number generator algorithm present in the boost library

//! compute random orbital elements from specific bounds
void randomKepElem( const Vector2 range_a,
					const Vector2 range_e,
					const Vector2 range_i,
					const Vector2 range_raan,
					const Vector2 range_w,
					const Vector2 range_E,
					const int limit,
					Vector2D& returnKepElem )
{
// generator type defined and initialized by a seed value
generator_type generator( 100 ); // don't change the seed value while debugging otherwise the psuedo random generated numbers will change

// define uniform random number distribution which produces type double values
// between [min,max) for each orbital element. For more details please refer to 
// the following web link:
// http://www.boost.org/doc/libs/1_60_0/libs/random/example/random_demo.cpp
boost::uniform_real<> uni_dist_a( range_a[ 0 ], range_a[ 1 ] ); // distribution range for semi major axis
boost::uniform_real<> uni_dist_e( range_e[ 0 ], range_e[ 1 ] ); // distribution range for eccentricity
boost::uniform_real<> uni_dist_i( range_i[ 0 ], range_i[ 1 ] ); // distribution range for inclination
boost::uniform_real<> uni_dist_raan( range_raan[ 0 ], range_raan[ 1 ] ); // distribution range for right ascension of ascending node
boost::uniform_real<> uni_dist_w( range_w[ 0 ], range_w[ 1 ] ); // distribution range for argument of perigee
boost::uniform_real<> uni_dist_E( range_E[ 0 ], range_E[ 1 ] ); // distribution range for eccentric anomaly

boost::variate_generator<generator_type&, boost::uniform_real<> > a_uni(generator, uni_dist_a);
boost::variate_generator<generator_type&, boost::uniform_real<> > e_uni(generator, uni_dist_e);
boost::variate_generator<generator_type&, boost::uniform_real<> > i_uni(generator, uni_dist_i);
boost::variate_generator<generator_type&, boost::uniform_real<> > raan_uni(generator, uni_dist_raan);
boost::variate_generator<generator_type&, boost::uniform_real<> > w_uni(generator, uni_dist_w);
boost::variate_generator<generator_type&, boost::uniform_real<> > E_uni(generator, uni_dist_E);

// obtain random orbital elements. Random numbers are obtained from the uniform distribution
// by calling the generator function with empty arguments.
for(int i = 0; i < limit; i++)
{
	returnKepElem[ i ][ 0 ]	= a_uni();
	returnKepElem[ i ][ 1 ] = e_uni();
	returnKepElem[ i ][ 2 ] = i_uni();
	returnKepElem[ i ][ 3 ] = raan_uni();
	returnKepElem[ i ][ 4 ] = w_uni();
	returnKepElem[ i ][ 5 ] = E_uni(); 	 
}  
// function ends
}


} // namespace randomKepElem
