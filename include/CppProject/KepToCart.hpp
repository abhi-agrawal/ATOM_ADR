/*
 * Copyright (c) 2016 Abhishek Agrawal (abhishek.agrawal@protonmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#ifndef CPP_PROJECT_KEP_TO_CART_HPP
#define CPP_PROJECT_KEP_TO_CART_HPP

namespace KepToCart
{

//! Compute ECI cartesian coordinates from orbital elements
/*!
 * @param  Vector6 KepElem 	6 element vector containing the orbital elements - a,e,i,raan,w,EA
 * @param  Real mu 			earths gravitational parameter
 * @param  Vector3& Pos		used to store and return by reference the ECI cartesian position coordinates
 * @param  Vector3& Vel		used to store and return by reference the ECI velocity components
 */
	typedef double Real;
	typedef std::vector< Real > Vector6;
	typedef std::vector< Real > Vector3;

	void KepToCart(Vector6 KepElem, Real mu, Vector3& Pos, Vector3& Vel);

} // namespace KepToCart

#endif // CPP_PROJECT_KEP_TO_CART_HPP