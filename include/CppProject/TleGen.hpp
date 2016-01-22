/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef CPP_PROJECT_TLE_GEN_HPP
#define CPP_PROJECT_TLE_GEN_HPP

namespace TleGen
{

//! Compute TLEs from a given or random orbital element set(s) with an intermediate step of conversion to Cartesian coordinates
/*!
 *  
 * @param  	int newLimit 					defines the total number of keplerian element sets for which TLE has to be generated
 * @param	Vector6randKepElem 	    		the vector containing the set of orbital elements
 * @param 	std::string& SolverStatus		contains the status of the non linear solver inside ATOM
 * @param	int& IterationCount				stores the total number of iterations taken by the non linear solver in ATOM
 */
typedef double Real;
typedef std::vector< Real > Vector6;

void TleGen( Vector6 randKepElem, std::string& SolverStatus, int& IterationCount );
} // TleGen

#endif // CPP_PROJECT_TLE_GEN_HPP