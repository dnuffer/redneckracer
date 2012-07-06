/*
 * $Id: GenericCoord3.hpp,v 1.1.2.4 2010/03/03 17:54:10 kpharris Exp $
 *
 * Part GraphLib
 * Copyright (C) 2009 Kevin Harris
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

#if    !defined(GRAPHLIB__GENERICCOORD3_HPP)
#define         GRAPHLIB__GENERICCOORD3_HPP

/*
  GenericCoord3.hpp

  A 3d coordinate class that has some basic functionality.  Also, some
  functions to act on that class (at the bottom).

*/

/*
  Revision history:
  17Jan1999 Wrote this file, did some basic testing added the dot product,
  cross product, length, and unit functions.
  20jan1999 Added a default constructor for the coord3 class and the
  coord3_union union.
  25jan1999 Added a unary - operator, and an invert function.
  05Mar1999 Added the * operator for two coord3s. It will take the
  componentwise product.
  02Apr1999 Added the constructor to take a single double, it is intended to
  init the coord to a given value.
  22Apr1999 Added the average function.
  Added the *= operator that takes two coord3s.
  18May2001 Started changing this for use in a newer tracer, as per "Realistic
  Raytracing" by Peter Shirley, "Templatized" the class...
  17Sep2003 Reordered the class, made it a part of namespace graphlib, changed
  extension to .hpp, added a GPL heading.
  28Feb2009 Renamed to GenericCoord3
*/

/**
 * @author Kevin Harris
 */

#include <cmath>
#include "graphlib/StringDumpable.hpp"

namespace graphlib
{
	template <class T>
	class GenericCoord3
	{
	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		enum COORD_VALUES
			{
				X = 0, Y = 1, Z = 2
			};
		typedef T base;

		GenericCoord3():coords(T(), T(), T())
		{
		}
		inline GenericCoord3(T u):coords(u,u,u)
		{
		}
		inline GenericCoord3(T x, T y, T z):coords(x,y,z)
		{
		}
		inline GenericCoord3(const GenericCoord3& old_coord):coords(old_coord.coords)
		{
		}

		inline void set(T x, T y, T z)
		{
			coords.direct.x = x;
			coords.direct.y = y;
			coords.direct.z = z;
		}

		/* Accessors */
		inline T& operator[](unsigned coord_index)
		{
			return(coords[coord_index]);
		}

		inline T  operator[](unsigned coord_index) const
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
		T& z()
		{
			return(coords.direct.z);
		}
		T  z() const
		{
			return(coords.direct.z);
		}

		GenericCoord3& operator= (const GenericCoord3& old_coord);
		GenericCoord3& operator+=(const GenericCoord3& p2);
		GenericCoord3& operator-=(const GenericCoord3& p2);
		GenericCoord3& operator*=(const GenericCoord3& p2); // Piecewise multiplication.
		GenericCoord3& operator*=(T factor);
		GenericCoord3& operator/=(T factor);
		template <class U>
		GenericCoord3& operator*=(U factor);
		template <class U>
		GenericCoord3& operator/=(U factor);

		inline T length() const
		{
			return T(sqrt(double((x() * x()) +
						(y() * y()) +
						(z() * z()))));
		}

		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;
		blocxx::String name() const { return "Coord3"; }

	private:
		/* class that has 3 Ts stored directly */
		struct coord3_direct
		{
			T x; T y; T z;
		};
		/* class that has 3 Ts stored in an array */
		struct coord3_array
		{
			T coords[3];
		};
		/* union to allow accesses to both indirectly through an array, and directly
			through a name, without adding any extra processing time or space
			requirements */
		union  coord3_union
		{
			coord3_union()
			{
			}
			coord3_union(T x, T y, T z)
			{
				direct.x = x; direct.y = y; direct.z = z;
			}
			inline T& operator[](unsigned index)
			{
				return(array.coords[index]);
			}
			inline T  operator[](unsigned index) const
			{
				return(array.coords[index]);
			}
			coord3_direct direct;
			coord3_array  array;
		};

		/* The internal coordinates of this class */
		coord3_union coords;
	};

	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator=(const GenericCoord3<T>& old_coord)
	{
		// No self-assignment comparison is done, because the compare/jump
		// should take longer than the assignment (for built-in types).
		coords.direct.x = old_coord.coords.direct.x;
		coords.direct.y = old_coord.coords.direct.y;
		coords.direct.z = old_coord.coords.direct.z;

		return *this;
	}


	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator+=(const GenericCoord3<T>& p2)
	{
		coords.direct.x += p2.coords.direct.x;
		coords.direct.y += p2.coords.direct.y;
		coords.direct.z += p2.coords.direct.z;
		return *this;
	}

	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator-=(const GenericCoord3<T>& p2)
	{
		coords.direct.x -= p2.coords.direct.x;
		coords.direct.y -= p2.coords.direct.y;
		coords.direct.z -= p2.coords.direct.z;
		return *this;
	}

	// Piecewise multiplication...
	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator*=(const GenericCoord3<T>& p2)
	{
		coords.direct.x *= p2.coords.direct.x;
		coords.direct.y *= p2.coords.direct.y;
		coords.direct.z *= p2.coords.direct.z;
		return *this;
	}

	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator*=(T factor)
	{
		coords.direct.x *= factor;
		coords.direct.y *= factor;
		coords.direct.z *= factor;
		return *this;
	}

	template <class T>
	inline GenericCoord3<T>& GenericCoord3<T>::operator/=(T factor)
	{
		coords.direct.x /= factor;
		coords.direct.y /= factor;
		coords.direct.z /= factor;
		return *this;
	}

	template <class T>
	template <class U>
	inline GenericCoord3<T>& GenericCoord3<T>::operator*=(U factor)
	{
		coords.direct.x = T(coords.direct.x * factor);
		coords.direct.y = T(coords.direct.y * factor);
		coords.direct.z = T(coords.direct.z * factor);
		return *this;
	}

	template <class T>
	template <class U>
	inline GenericCoord3<T>& GenericCoord3<T>::operator/=(U factor)
	{
		coords.direct.x = T(coords.direct.x / factor);
		coords.direct.y = T(coords.direct.y / factor);
		coords.direct.z = T(coords.direct.z / factor);
		return *this;
	}

	/* non-member math functions (also inlined for an attempt at some speed) */
	template <class T>
	inline GenericCoord3<T> operator+(const GenericCoord3<T>& p1, const GenericCoord3<T>& p2)
	{
		GenericCoord3<T> p(p1); p += p2; return(p);
	}

	template <class T>
	inline GenericCoord3<T> operator-(const GenericCoord3<T>& p1, const GenericCoord3<T>& p2)
	{
		GenericCoord3<T> p(p1); p -= p2; return(p);
	}

	template <class T>
	inline GenericCoord3<T> operator*(const GenericCoord3<T>& p1, T factor)
	{
		GenericCoord3<T> p(p1); p *= factor;  return(p);
	}

	template <class T>
	inline GenericCoord3<T> operator*(T factor, const GenericCoord3<T>& p1)
	{
		GenericCoord3<T> p(p1); p *= factor;  return(p);
	}

	template <class T, class U>
	inline GenericCoord3<T> operator*(const GenericCoord3<T>& p1, U factor)
	{
		GenericCoord3<T> p(p1); p *= factor;  return(p);
	}

	template <class T, class U>
	inline GenericCoord3<T> operator*(U factor, const GenericCoord3<T>& p1)
	{
		GenericCoord3<T> p(p1); p *= factor;  return(p);
	}

	template <class T>
	inline GenericCoord3<T> operator*(const GenericCoord3<T>& p1, const GenericCoord3<T>& p2)
	{
		return(GenericCoord3<T>(p1.x() * p2.x(),
				p1.y() * p2.y(),
				p1.z() * p2.z()));
	}
	template <class T>
	inline GenericCoord3<T> operator/(const GenericCoord3<T>& p1, T factor)
	{
		GenericCoord3<T> p(p1); p /= factor;  return(p);
	}

	template <class T, class U>
	inline GenericCoord3<T> operator/(const GenericCoord3<T>& p1, U factor)
	{
		GenericCoord3<T> p(p1); p /= factor;  return(p);
	}

	// Unary minus
	template <class T>
	inline GenericCoord3<T> operator-(const GenericCoord3<T>& p1)
	{
		return(T(-1)*p1);
	}


	/* exports from this header file */
	template <class T>
	inline T dotprod(const GenericCoord3<T>& c1, const GenericCoord3<T>& c2)
	{
		return( (c1.x()*c2.x()) + (c1.y()*c2.y()) + (c1.z()*c2.z()));
	}

	template <class T>
	inline GenericCoord3<T> crossprod(const GenericCoord3<T>& c1, const GenericCoord3<T>& c2)
	{
		return( GenericCoord3<T>((c1.y()*c2.z()) - (c2.y()*c1.z()),
				(c2.x()*c1.z()) - (c1.x()*c2.z()),
				(c1.x()*c2.y()) - (c2.x()*c1.y())));
	}

	template <class T>
	inline T length(const GenericCoord3<T>& c)
	{
		return(c.length());
	}

	template <class T>
	inline T averageComponent(const GenericCoord3<T>& c)
	{
		return (c.x() + c.y() + c.z()) / T(3);
	}

	template <class T>
	inline GenericCoord3<T> invert(const GenericCoord3<T>& c)
	{
		return(GenericCoord3<T>(1/c.x(), 1/c.y(), 1/c.z()));
	}

	template <class T>
	inline T maxComponent(const GenericCoord3<T>& c)
	{
		return std::max(std::max(c.x(), c.y()), c.z());
	}

	template <class T>
	inline GenericCoord3<T> abs(const GenericCoord3<T>& v)
	{
		return GenericCoord3<T>(std::max(v.x(), -v.x()), std::max(v.y(), -v.y()), std::max(v.z(), -v.z()));
	}

	template <class T> inline T squared_length(const GenericCoord3<T>& c)
	{
		return(dotprod(c,c));
	}

	template <class T> inline GenericCoord3<T> unit(const GenericCoord3<T>& c)
	{
		return(c/c.length());
	}

	template <class T>
	void GenericCoord3<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("x", x());
		collector.addMember("y", y());
		collector.addMember("z", z());
	}

	template <class T>
	blocxx::String GenericCoord3<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericCoord3<T>::name();
		}
		return indent.initial() + tag +
			"{" +
			graphlib::toString(x(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(y(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(z(), indent.indentInside(), prefix) +
			"}";
	}

} // namespace graphlib

#endif /* !defined(GRAPHLIB__COORD3_HPP) */
