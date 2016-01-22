/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef CPP_PROJECT_KEP_TO_CART_TO_TLE_HPP
#define CPP_PROJECT_KEP_TO_CART_TO_TLE_HPP

namespace KepToCartToTLE
{

//! Compute TLEs from a given or random orbital element set(s) with an intermediate step of conversion to Cartesian coordinates
/*!
 *  
 * @param  	int newLimit 			defines the total number of keplerian element sets for which TLE has to be generated
 * @param	Vector2D randKepElem 	the two dimensional vector containing the sets of orbital elements, each set laid down row wise
 * 
 */
typedef double Real;
typedef std::vector< std::vector < Real > > Vector2D;

void KepToCartToTLE( int newLimit, Vector2D randKepElem );
} // namespace KepToCartToTLE

#endif // CPP_PROJECT_KEP_TO_CART_TO_TLE_HPP