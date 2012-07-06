/*
 * $Id: Fuzzy.hpp,v 1.1 2011/05/14 11:46:59 kpharris Exp $
 *
 * Part of "Graphlib"
 * Copyright (C) 2011 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if       !defined(GRAPHLIB_FUZZY_HPP)
#define            GRAPHLIB_FUZZY_HPP

#include <limits>

namespace graphlib
{
	const int FUZZ_FACTOR = 4;

	template <typename T>
	inline T fuzzy(const T& t)
	{
		if( std::numeric_limits<T>::is_exact )
		{
			return t;
		}
		return t + (FUZZ_FACTOR * std::numeric_limits<T>::epsilon() );
	}

	template <typename T>
	inline bool fuzzyLess(const T& t1, const T& t2)
	{
		return t1 < fuzzy(t2);
	}

	template <typename T>
	inline bool fuzzyGreater(const T& t1, const T& t2)
	{
		return fuzzy(t1) > t2;
	}

	template <typename T>
	inline bool fuzzyEquals(const T& t1, const T& t2)
	{
		if( std::numeric_limits<T>::is_exact )
		{
			return t1 == t2;
		}
		return (fuzzy(t1) >= t2) && (fuzzy(t2 >= t1));
	}
}

#endif /* !defined(GRAPHLIB_FUZZY_HPP) */
