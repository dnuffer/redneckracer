#ifndef BLOCXX_DIV_REM_HPP_INCLUDE_GUARD
#define BLOCXX_DIV_REM_HPP_INCLUDE_GUARD

/*******************************************************************************
* Copyright (C) 2008, QuestSoftware, Inc. All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
* 
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Quest Software, Inc.,
*       nor the names of its contributors or employees may be used to 
*       endorse or promote products derived from this software without 
*       specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/**
 * @author Kevin S. Van Horn
 */

#include "miniblocxx/BLOCXX_config.h"

namespace BLOCXX_NAMESPACE
{
	// Implementation detail
	// Have the compiler determine whether a T divided by a T rounds down
	// or towards zero.  If it rounds tozero, then down = 0, tozero = 1,
	// otherwise down = 1, tozero = 0.
	template <typename T>
	struct DivRounds
	{
		enum
		{
			down = ((T(-4) / T(3)) == T(-2)),
			tozero = ((T(-4) / T(3)) == T(-1))
		};
	};

	// REQUIRE: y > 0
	// RETURNS: x/y, rounded towards zero
	//
	template <typename T>
	T divTrunc(T x, T y)
	{
		return DivRounds<T>::tozero || x >= 0  ?  x / y  :  -((-x) / y);
	}

	// REQUIRE: y > 0
	// RETURNS: remainder of x/y corresponding to divTrunc,
	// i.e. the value r such that y * divTrunc(x, y) + r == x
	//
	template <typename T>
	T remTrunc(T x, T y)
	{
		return DivRounds<T>::tozero || x >= 0  ?  x % y  :  -((-x) % y);
	}

	// REQUIRE: y > 0
	// RETURNS: x/y, rounded down
	//
	template <typename T>
	T divFloor(T x, T y)
	{
		return DivRounds<T>::down || x >= 0 ?  x / y  :  (x + 1) / y - 1;
	}

	// REQUIRE: y > 0
	// RETURNS: remainder of x/y corresponding to divFloor,
	// i.e. the value r such that y * divFloor(x, y) + r == x
	//
	template <typename T>
	T remFloor(T x, T y)
	{
		return DivRounds<T>::down || x >= 0 ?  x % y  :  (x + 1) % y + y - 1;
	}

} // end namespace BLOCXX_NAMESPACE

#endif
