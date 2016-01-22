/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef CPP_PROJECT_RANDOM_GEN_HPP
#define CPP_PROJECT_RANDOM_GEN_HPP

namespace randomGen
{

//! Compute random elements from a uniform distribution using a generator from the boost library
/*!
 *  
 * @param  	const Vector2 Range 	defines the min and max value or range between which the random elements have to be generated
 * @param	const int newLimit		defines the total number of random elements required
 * @param 	VectorLong& output		the randomly generated elements are stored in the vector
 */
typedef double Real;
typedef std::vector< Real > Vector2;
typedef std::vector< Real > VectorLong;

void randomGen( const Vector2 range, const int limit, VectorLong& output);
} // namespace randomGen

#endif // CPP_PROJECT_RANDOM_GEN_HPP