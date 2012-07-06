/*
 * $Id: GenericPoint3.hpp,v 1.1.2.2 2010/03/03 17:54:10 kpharris Exp $
 *
 * Part of GraphLib
 * Copyright (C) 2003 Kevin Harris
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

#if    !defined(GRAPHLIB__GENERICPOINT3_HPP)
#define         GRAPHLIB__GENERICPOINT3_HPP

/*
  GenericPoint3.hpp

  An expansion of the GenericCoord3 class to allow the creation of distinct
  types.

  It allows for creation of a vector by subtracting two points, and creation of
  another point by addition or subtraction of a vector to a point.
*/


/*
  Revision history:
  17jan1999 Wrote this file, did some basic testing.  Added the extraction
  operator to this, and the other '3' classes (GenericVector3, GenericCoord3).
  This will make different output when dumped on a stream.  It could
  come in handy when debugging something.
  20jan1999 Added a default constructor.
  18May2001 "Templatized" the class...
  17Sep2003 Changed the inheritence from public to private.  Added accessors,
  fixed implementation to use those accessors.
  14Jun2009 Renamed from point3 to GenericPoint3
*/

#include "graphlib/GenericCoord3.hpp"
#include "graphlib/GenericVector3.hpp"

namespace graphlib
{

	template <class T>
	class GenericPoint3: private GenericCoord3<T>
	{
	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		using GenericCoord3<T>::COORD_VALUES;
		using GenericCoord3<T>::X;
		using GenericCoord3<T>::Y;
		using GenericCoord3<T>::Z;

		typedef T base;

		inline GenericPoint3(): GenericCoord3<T>()
		{
		}
		inline GenericPoint3(T x, T y, T z): GenericCoord3<T>(x,y,z)
		{
		}
		inline explicit GenericPoint3(const GenericCoord3<T>& c): GenericCoord3<T>(c)
		{
		}
		inline GenericPoint3(const GenericPoint3<T>& c): GenericCoord3<T>(c)
		{
		}

		GenericCoord3<T> getcoord() const
		{
			return GenericCoord3<T>(x(), y(), z());
		}

		inline void set(T x, T y, T z)
		{
			GenericCoord3<T>::set(x,y,z);
		}

		/* Accessors */
		inline T& operator[](int coord_index)
		{
			return(GenericCoord3<T>::operator[](coord_index));
		}

		inline T operator[](int coord_index) const
		{
			return(GenericCoord3<T>::operator[](coord_index));
		}

		T& x()
		{
			return(GenericCoord3<T>::x());
		}
		T x() const
		{
			return(GenericCoord3<T>::x());
		}
		T& y()
		{
			return(GenericCoord3<T>::y());
		}
		T y() const
		{
			return(GenericCoord3<T>::y());
		}
		T& z()
		{
			return(GenericCoord3<T>::z());
		}
		T z() const
		{
			return(GenericCoord3<T>::z());
		}

		GenericPoint3<T>& operator=(const GenericPoint3<T>& p);
		GenericPoint3<T>& operator+=(const GenericVector3<T>& v1);
		GenericPoint3<T>& operator-=(const GenericVector3<T>& v1);

		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;

		blocxx::String name() const { return "Point3"; }
	}; // class GenericPoint3

	template <class T>
	inline GenericPoint3<T>& GenericPoint3<T>::operator=(const GenericPoint3<T>& p)
	{
		set(p.x(), p.y(), p.z());
		return(*this);
	}

	template <class T>
	inline GenericPoint3<T>& GenericPoint3<T>::operator+=(const GenericVector3<T>& v1)
	{
		set(x() + v1.x(),
			y() + v1.y(),
			z() + v1.z());
		return(*this);
	}

	template <class T>
	inline GenericPoint3<T>& GenericPoint3<T>::operator-=(const GenericVector3<T>& v1)
	{
		set(x() - v1.x(),
			y() - v1.y(),
			z() - v1.z());
		return(*this);
	}

	template <class T>
	inline GenericVector3<T> operator-(const GenericPoint3<T>& p1, const GenericPoint3<T>& p2)
	{
		return GenericVector3<T>(p1.x() - p2.x(),
			p1.y() - p2.y(),
			p1.z() - p2.z());
	}
	template <class T>
	inline GenericPoint3<T> operator+(const GenericPoint3<T>& p1, const GenericVector3<T>& v1)
	{
		return GenericPoint3<T>(p1.x() + v1.x(),
			p1.y() + v1.y(),
			p1.z() + v1.z());
	}
	template <class T>
	inline GenericPoint3<T> operator-(const GenericPoint3<T>& p1, const GenericVector3<T>& v1)
	{
		return GenericPoint3<T>(p1.x() - v1.x(),
			p1.y() - v1.y(),
			p1.z() - v1.z());
	}
	template <class T>
	inline GenericPoint3<T> operator+(const GenericVector3<T>& v1, const GenericPoint3<T>& p1)
	{
		return GenericPoint3<T>(p1.x() + v1.x(),
			p1.y() + v1.y(),
			p1.z() + v1.z());
	}
	template <class T>
	inline GenericPoint3<T> operator-(const GenericVector3<T>& v1, const GenericPoint3<T>& p1)
	{
		return GenericPoint3<T>(v1.x() - p1.x(),
			v1.y() - p1.y(),
			v1.z() - p1.z());
	}

	template <class T>
	inline T averageComponent(const GenericPoint3<T>& v)
	{
		return (v.x() + v.y() + v.z()) / T(3);
	}

	template <class T>
	inline T maxComponent(const GenericPoint3<T>& v)
	{
		return std::max(std::max(v.x(), v.y()), v.z());
	}


	template <class T>
	void GenericPoint3<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		GenericCoord3<T>::collectInternalMembers(collector);
	}

	template <class T>
	blocxx::String GenericPoint3<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericPoint3<T>::name();
		}

		return indent.initial() + tag +
			"(" +
			graphlib::toString(x(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(y(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(z(), indent.indentInside(), prefix) +
			")";
	}

} // namespace graphlib

#endif /* !defined(GRAPHLIB__GENERICPOINT3_HPP) */
