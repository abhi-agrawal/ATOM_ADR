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

#include "CppProject/randomGen.hpp"

namespace randomGen
{
	typedef double Real;
	typedef std::vector< Real > Vector2;
	typedef std::vector< Real > VectorLong;
	typedef boost::mt19937 generator_type; // mt19937 is a psuedo random number generator algorithm present in the boost library

	void randomGen( const Vector2 range, const int limit, VectorLong& output)
	{
		// generator type defined and initialized by a seed value
		generator_type generator( 100 ); // don't change the seed value while debugging otherwise the psuedo random generated numbers will change
		// define uniform random number distribution which produces type double values
		// between [min,max) for each orbital element. For more details please refer to 
		// the following web link:
		// http://www.boost.org/doc/libs/1_60_0/libs/random/example/random_demo.cpp
		boost::uniform_real<> distribution( range[ 0 ], range[ 1 ] );
		boost::variate_generator<generator_type&, boost::uniform_real<> > uniform(generator, distribution);
		for(int i = 0; i < limit; i++)
		{
			output[ i ]	= uniform();
		} 
	}
} // namespace randomGen