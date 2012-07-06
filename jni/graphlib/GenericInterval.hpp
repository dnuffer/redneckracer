/*
 * $Id: GenericInterval.hpp,v 1.1.2.1 2010/03/07 18:49:12 kpharris Exp $
 *
 * Part of GraphLib
 * Copyright (C) 2010 Kevin Harris
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

#if       !defined(GRAPHLIB_INTERVAL_HPP)
#define            GRAPHLIB_INTERVAL_HPP

/*
  GenericInterval.hpp -- A simple class for use in creating/testing intervals.
*/

/**
 * @author Kevin Harris
 */

/*
  Revision History:
  18May2001 Created this file, "Templatized" the class... Tested it some.
  Complicated it some more (allowing backwards intervals)...
  08Jan2002 Added quite a few comments, before I forget what is going on.
  23Sep2003 Changed the structure a little, moving member functions outside,
  setting some member functions to be templated.  Also, added the
  < and > operators.
  29Sep2003 Simplified things by removing the ability to be backwards
  (reversing an earlier change).  Also, all references to comparison
  of the given type by '>' were removed, so that only the '<'
  operator must be provided for comparisons ( >, <, and
  inside/outside ) to work.
  06Mar2010 Really removed all use of <=.  Simplified the logic in a few
  functions.  Cleaned up and renamed.
*/

#include "graphlib/StringDumpable.hpp"

#include <algorithm>

namespace graphlib
{
	//
	// GenericInterval class:
	// A class representing a range of numbers (between to constraints)
	//
	// It can be used to find:
	//   1. If a number is inside an interval
	//   2. If a number is outside an interval
	//   3. If two intervals overlap
	//   4. If an interval is a subset of another
	//   5. Find the overlap of two intervals
	//   6. Subtract one interval from another
	//   7. If an interval is empty
	//   8. Output an interval (to a std::ostream) in the form [a,b] or [empty]
	//   9. If something is > or < and interval (either an interval itself, or
	//      any other type that can be compared to a T by means of the < and <=
	//      operators).  I would have prefered to only use the < operator, but it
	//      was neccessary to use the <= to allow comparing of intervals when
	//      endpoints are equal (for overlap testing).
	//
	// Notes:
	// * Comparison of anything with an empty interval always returns false.
	//   This may change in the future, if I find that something external doesn't
	//   work properly because of this comparison.  Do not rely on this behavior!
	//
	template <class T>
	class GenericInterval
	{
	private:
		T m_begin;
		T m_end;

	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		//
		// Sets the Interval to be empty
		//
		GenericInterval() : m_begin(T(0)), m_end(T(0))
		{
		}

		//
		// Sets an Interval to a given range
		//
		GenericInterval(T a, T b)
			: m_begin(a), m_end(b)
		{
			if( m_end < m_begin )
			{
				std::swap(m_begin, m_end);
			}
		} // GenericInterval(T,T)

		//
		// Find out if a point is inside the Interval (generic)
		//
		template <class U>
		bool inside(U d) const
		{
			// This empty check looks redundant, but really serves a purpose when Intervals have infinite endpoints.
			if( empty() )
			{
				return false;
			}
			return((m_begin < d) && (d < m_end));
		} // inside(T) const

		//
		// Find out if a point is outside the Interval (generic)
		//
		template <class U>
		bool outside(U d) const
		{
			// Can't be inside or outside an empty Interval.
			if( empty() )
			{
				return false;
			}
			// The inverse of the 'inside' test above.  This is done this way instead
			// of the >= and <= operators to minimize the number of operators that
			// must be defined for types used within an Interval.
			return(!((m_begin < d) && (d < m_end)));
		} // outside(T) const


		//
		// Find out if two Intervals overlap
		//
		bool overlaps(const GenericInterval& i2) const
		{
			if( empty() || i2.empty() )
			{
				// If either one is empty, there can't be any overlap.
				return false;
			}
			// One interval must be completely to the left (endpoints excluded) of
			// the other to be non-overlapping.
			// i2.end() <= begin() || end() <= i2.begin()
			if( !(begin() < i2.end()) || !(i2.begin() < end()) )
			{
				return false;
			}
			return true;
		} // overlaps(const GenericInterval&) const

		//
		// Return if the Interval i2 is a subset of this Interval.
		//
		bool subset(const GenericInterval& i2) const
		{
			if( empty() || i2.empty() )
			{
				return false;
			}
			// return((begin() <= i2.begin()) && (i2.end() <= end()));
			return (!(i2.begin() < begin()) && !(end() < i2.end()));
		} // subset(const GenericInterval&) const

		bool empty() const
		{
			return !(this->m_begin < this->m_end);
		}
		T begin() const
		{
			return m_begin;
		}
		T end()   const
		{
			return m_end;
		}

		void set(T begin, T end)
		{
			m_begin = begin; m_end = end;
			if( m_end < m_begin )
			{
				std::swap(this->a, this->b);
			}
		} // set(T,T)

		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;
		blocxx::String name() const { return "Interval"; }
	}; // class GenericInterval<T>


	//
	// Return the overlap of Interval i1 and Interval i2 (as an Interval)
	//
	template <class T>
	GenericInterval<T> intersection(const GenericInterval<T>& i1, const GenericInterval<T>& i2)
	{
		GenericInterval<T> result; // result Interval (empty by default)

		// If either Interval is empty, return an empty Interval, as there can't be
		// any overlap.
		if( i1.empty() || i2.empty() )
		{
			return result;
		}

		// FIXME! Rewrite this to be simpler.

		if( (i1.begin() <= i2.begin()) && (i2.begin() <= i1.end()) )
		{
			result = GenericInterval<T>(i2.begin(), std::min(i1.end(), i2.end()));
		}
		else if( (i2.begin() <= i1.begin()) && (i1.begin() <= i2.end()) )
		{
			result = GenericInterval<T>(i1.begin(), std::min(i1.end(), i2.end()));
		}
		else if( (i1.begin() <= i2.end()) && (i2.end() <= i1.end()) )
		{
			result = GenericInterval<T>(std::max(i1.begin(), i2.begin()), i2.end());
		}
		else if( (i2.begin() <= i1.end()) && (i1.end() <= i2.end()) )
		{
			result = GenericInterval<T>(std::max(i1.begin(), i1.begin()), i1.end());
		}
		else
		{
			// Is a case here even valid?
			// FIXME: FOOBARBAZ
		}
		return result;
	} // intersection(const GenericInterval&) const

	//
	// Return the Interval created by subtracting the Interval i2 from
	// Interval i1.  If i1 is empty, returns empty.  If i2 is empty, returns i1.
	//
	template <class T>
	GenericInterval<T> operator-(const GenericInterval<T>& i1, const GenericInterval<T>& i2)
	{
		if( i1.empty() || i2.empty() )
		{
			// If the Interval is empty, it can't overlap, and thus cannot have
			// another Interval subtracted from it.
			//
			// If the second Interval is empty, then subtraction will do nothing.
			//
			// Both of these cases should work properly by just returning i1
			return i1;
		}

		// If the the minimum component of i1 is less than i2, the entire region
		// from i1.begin() to i2.begin() is all that remains after subtraction.
		// That is, IFF they overlap, if not, then the subtraction is the same as
		// the original i1.
		if( i1.begin() <= i2.begin() )
		{
			return(GenericInterval<T>(i1.begin(),std::min(i1.end(),i2.begin())));
 		}
		// If the the maximum component of i1 is greater than i2, the entire
		// region from i2.end() to i1.end() is all that remains after
		// subtraction.
		// That is, IFF they overlap, if not, then the subtraction is the same as
		// the original i1.
		else if( i2.end() <= i1.end() )
		{
			return(GenericInterval<T>(std::max(i1.begin(),i2.end()),i1.end()));
		}
		// Return an empty Interval...
		return GenericInterval<T>();
	} // operator-(const GenericInterval&, const GenericInterval&)


	//
	// Global comparison operators:
	// (GenericInterval, GenericInterval), (GenericInterval, template U), (template U, GenericInterval)
	//
	template <class T>
	inline bool operator <(const GenericInterval<T>& t1, const GenericInterval<T>& t2)
	{
		if( t1.empty() || t2.empty() )
		{
			return false;
		}

		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		// return (t1.begin() <= t2.begin()) && (t1.end() <= t2.begin());
		return !(t2.begin() < t1.begin()) && !(t2.begin() < t1.end());
	}

	template <class T>
	inline bool operator >(const GenericInterval<T>& t1, const GenericInterval<T>& t2)
	{
		if( t1.empty() || t2.empty() )
		{
			return false;
		}

		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		// return (t2.begin() <= t1.begin()) && (t2.end() <= t1.begin());
		return !(t1.begin() < t2.begin()) && !(t1.begin() < t2.end());
	}

	template <class T, class U>
	inline bool operator <(const GenericInterval<T>& t1, const U& u)
	{
		if( t1.empty() )
		{
			return false;
		}
		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		// return (t1.end() <= u);
		return !(u < t1.end());
	}

	template <class T, class U>
	inline bool operator >(const GenericInterval<T>& t1, const U& u)
	{
		if( t1.empty() )
		{
			return false;
		}
		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		//return (u <= t1.begin());
		return !(t1.begin() < u);
	}

	template <class T, class U>
	inline bool operator <(const U& u, const GenericInterval<T>& t1)
	{
		if( t1.empty() )
		{
			return false;
		}
		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		// return (u <= t1.begin());
		return !(t1.begin() < u);
	}

	template <class T, class U>
	inline bool operator >(const U& u, const GenericInterval<T>& t1)
	{
		if( t1.empty() )
		{
			return false;
		}
		// Note that the <= is actually correct here, as being ON the endpoint is
		// not being within the Interval.
		// return (t1.end() <= u);
		return !(u < t1.end());
	}

	template <class T>
	void GenericInterval<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("begin", begin());
		collector.addMember("end", end());
	}

	template <class T>
	blocxx::String GenericInterval<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericInterval<T>::name();
		}
		if( empty() )
		{
			return indent.initial() + tag + "[empty]";
		}
		return indent.initial() + tag +
			"[" +
			graphlib::toString(begin(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(end(), indent.indentInside(), prefix) +
			"]";
	}

} // namespace graphlib
#endif /* !defined(GRAPHLIB_INTERVAL_HPP) */
