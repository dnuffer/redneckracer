/*
 * $Id: GenericCoord2.hpp,v 1.1.2.4 2010/03/03 17:54:09 kpharris Exp $
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

#if    !defined(GRAPHLIB__GENERIC_COORD2_HPP)
#define         GRAPHLIB__GENERIC_COORD2_HPP

/*
  GenericCoord2.hpp

  The 2d equiv of everything that I placed initially in coord3.h, except for
  the cross product, as there is no 2d equiv.
*/

/**
 * @author Kevin Harris
 */

/*
  Revision history:
  17Jan1999 Wrote this file, did some basic testing.
  20jan1999 Added a default constructor for the GenericCoord2 class.
  Added a default constructor for the union in the GenericCoord2 class.
  25jan1999 Added a unary - operator, and an invert function.
  18May2001 "Templatized" the class...
  17Sep2003 Reordered the class, made it a part of namespace graphlib, changed
  extension to .hpp, added a GPL heading.
*/

#include <cmath>
#include "graphlib/StringDumpable.hpp"

namespace graphlib
{
	// No inheritence, no virtual functions.  Adding either will break the
	// ability to reinterpret this structure as a pair of doubles.
	template <class T>
	class GenericCoord2
	{
	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		enum COORD_VALUES
			{
				X = 0, Y = 1
			};
		typedef T base;

		GenericCoord2():coords(T(), T())
		{
		}
		inline GenericCoord2(T x, T y):coords(x,y)
		{
		}
		inline GenericCoord2(const GenericCoord2& old_coord)
			: coords(old_coord.coords)
		{
		}

		inline void set(T x, T y)
		{
			coords.direct.x = x; coords.direct.y = y;
		}

		/* Accessors */
		inline T& operator[](int coord_index)
		{
			return(coords[coord_index]);
		}

		inline T  operator[](int coord_index) const
		{
			return(coords[coord_index]);
		}

		T& x()
		{
			return(coords.direct.x);
		}
		T  x() const
		{
			return(coords.direct.x);
		}
		T& y()
		{
			return(coords.direct.y);
		}
		T  y() const
		{
			return(coords.direct.y);
		}


		GenericCoord2& operator=(const GenericCoord2& old_coord);
		GenericCoord2& operator+=(const GenericCoord2& p2);
		GenericCoord2& operator-=(const GenericCoord2& p2);
		GenericCoord2& operator*=(const GenericCoord2& p2); // Piecewise multiplication.
		GenericCoord2& operator*=(T factor);
		GenericCoord2& operator/=(T factor);
		template <class U>
		GenericCoord2& operator*=(U factor);
		template <class U>
		GenericCoord2& operator/=(U factor);

		/* Functions that more relate to vectors, but are needed */
		inline T length() const
		{
			return(T(sqrt(double(x() * x() + y() * y()))));
		}

		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;
		blocxx::String name() const { return "Coord2"; }

	private:
		/* class that has 2 Ts stored directly */
		struct coord2_direct
		{
			T x; T y;
		};
		/* class that has 2 Ts stored in an array */
		struct coord2_array
		{
			T coords[2];
		};
		/* union to allow accesses to both indirectly through an array, and directly
			through a name, without adding any extra processing time or space
			requirements */
		union  coord2_union
		{
			coord2_union()
			{
			}
			coord2_union(T x, T y)
			{
				direct.x = x; direct.y = y;
			}
			T& operator[](int index)
			{
				return(array.coords[index]);
			}
			T  operator[](int index) const
			{
				return(array.coords[index]);
			}
			coord2_direct direct;
			coord2_array  array;
		};

		/* The internal coordinates of this class */
		coord2_union coords;

	}; // class coord2

	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator=(const GenericCoord2<T>& old_coord)
	{
		// No self-assignment comparison is done, because the compare/jump
		// should take longer than the assignment (for built-in types).
		coords.direct.x = old_coord.coords.direct.x;
		coords.direct.y = old_coord.coords.direct.y;

		return *this;
	}


	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator+=(const GenericCoord2<T>& p2)
	{
		coords.direct.x += p2.coords.direct.x;
		coords.direct.y += p2.coords.direct.y;

		return *this;
	}

	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator-=(const GenericCoord2<T>& p2)
	{
		coords.direct.x -= p2.coords.direct.x;
		coords.direct.y -= p2.coords.direct.y;

		return *this;
	}

	// Piecewise multiplication...
	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator*=(const GenericCoord2<T>& p2)
	{
		coords.direct.x *= p2.coords.direct.x;
		coords.direct.y *= p2.coords.direct.y;

		return *this;
	}

	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator*=(T factor)
	{
		coords.direct.x *= factor;
		coords.direct.y *= factor;

		return *this;
	}

	template <class T>
	inline GenericCoord2<T>& GenericCoord2<T>::operator/=(T factor)
	{
		coords.direct.x /= factor;
		coords.direct.y /= factor;

		return *this;
	}

	template <class T>
	template <class U>
	inline GenericCoord2<T>& GenericCoord2<T>::operator*=(U factor)
	{
		coords.direct.x = T(coords.direct.x * factor);
		coords.direct.y = T(coords.direct.y * factor);

		return *this;
	}

	template <class T>
	template <class U>
	inline GenericCoord2<T>& GenericCoord2<T>::operator/=(U factor)
	{
		coords.direct.x = T(coords.direct.x / factor);
		coords.direct.y = T(coords.direct.y / factor);

		return *this;
	}


	/* non-member math functions (also inlined for an attempt at some speed) */
	template <class T>
	inline GenericCoord2<T> operator+(const GenericCoord2<T>& p1, const GenericCoord2<T>& p2)
	{
		GenericCoord2<T> p(p1); p += p2; return(p);
	}
	template <class T>
	inline GenericCoord2<T> operator-(const GenericCoord2<T>& p1, const GenericCoord2<T>& p2)
	{
		GenericCoord2<T> p(p1); p -= p2; return(p);
	}
	template <class T>
	inline GenericCoord2<T> operator*(const GenericCoord2<T>& p1, T d)
	{
		GenericCoord2<T> p(p1); p *= d;  return(p);
	}
	template <class T>
	inline GenericCoord2<T> operator*(T d, const GenericCoord2<T>& p1)
	{
		GenericCoord2<T> p(p1); p *= d;  return(p);
	}
	template <class T>
	inline GenericCoord2<T> operator/(const GenericCoord2<T>& p1, T d)
	{
		GenericCoord2<T> p(p1); p /= d;  return(p);
	}

	template <class T, class U>
	inline GenericCoord2<T> operator*(const GenericCoord2<T>& p1, U d)
	{
		GenericCoord2<T> p(p1); p *= d;  return(p);
	}

	template <class T, class U>
	inline GenericCoord2<T> operator*(U d, const GenericCoord2<T>& p1)
	{
		GenericCoord2<T> p(p1); p *= d;  return(p);
	}

	template <class T>
	inline GenericCoord2<T> operator*(const GenericCoord2<T>& p1, const GenericCoord2<T>& p2)
	{
		return(GenericCoord2<T>(p1.x() * p2.x(),
				p1.y() * p2.y()));
	}

	template <class T, class U>
	inline GenericCoord2<T> operator/(const GenericCoord2<T>& p1, U d)
	{
		GenericCoord2<T> p(p1); p /= d;  return(p);
	}

	/* Unary minus */
	template <class T>
	inline GenericCoord2<T> operator-(const GenericCoord2<T>& p1)
	{
		return(T(-1)*p1);
	}


	/* exports from this header file */
	template <class T>
	inline T dotprod(const GenericCoord2<T>& c1, const GenericCoord2<T>& c2)
	{
		return( (c1.x()*c2.x()) + (c1.y()*c2.y()));
	}

	template <class T>
	inline T length(const GenericCoord2<T>& c)
	{
		return(c.length());
	}

	template <class T>
	inline GenericCoord2<T> unit(const GenericCoord2<T>& c)
	{
		return(c/c.length());
	}

	template <class T>
	inline GenericCoord2<T> invert(const GenericCoord2<T>& c)
	{
		return(GenericCoord2<T>(1/c.x(), 1/c.y()));
	}

	template <class T>
	inline T averageComponent(const GenericCoord2<T>& c)
	{
		return((c.x() + c.y()) / 2);
	}

	template <class T>
	inline T maxComponent(const GenericCoord2<T>& c)
	{
		return(std::max(c.x(), c.y()));
	}

	template <class T>
	inline GenericCoord2<T> abs(const GenericCoord2<T>& v)
	{
		return GenericCoord2<T>(std::max(v.x(), -v.x()), std::max(v.y(), -v.y()));
	}

	template <class T>
	void GenericCoord2<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("x", x());
		collector.addMember("y", y());
	}

	template <class T>
	blocxx::String GenericCoord2<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericCoord2<T>::name();
		}
		return indent.initial() + tag +
			"{" +
			graphlib::toString(x(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(y(), indent.indentInside(), prefix) +
			"}";
	}
	
	template <class T>
	bool operator==(const GenericCoord2<T>& lhs, const GenericCoord2<T>& rhs)
	{
		return (lhs.x() == rhs.x()) && (lhs.y() == rhs.y());
	}

	template <class T>
	bool operator!=(const GenericCoord2<T>& lhs, const GenericCoord2<T>& rhs)
	{
		return !(lhs == rhs);
	}
	
} // namespace graphlib

#endif /* !defined(GRAPHLIB__GENERIC_COORD2_HPP) */
