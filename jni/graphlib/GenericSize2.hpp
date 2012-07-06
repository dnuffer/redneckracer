/*
 * $Id: GenericSize2.hpp,v 1.1.2.2 2010/03/03 17:54:10 kpharris Exp $
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

#if    !defined(GRAPHLIB__GENERICSIZE2_HPP)
#define         GRAPHLIB__GENERICSIZE2_HPP

/*
  GenericSize2.hpp

  A 2d equiv of the stuff in point3.hpp
*/

/*
  revision history:
  17Jan1999 Wrote this file, did some basic testing.
  20jan1999 Added a default constructor.
  02feb1999 Fixed the broken constructor.
  18May2001 "Templatized" the class...
  17Sep2003 Changed the inheritence from public to private.  Added accessors,
  fixed implementation to use those accessors.
  14Jun2009 Renamed from point2 to GenericSize2
*/

#include "graphlib/GenericCoord2.hpp"
#include "graphlib/GenericVector2.hpp"

namespace graphlib
{

	template <class T>
	class GenericSize2: private GenericCoord2<T>
	{
	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		using GenericCoord2<T>::COORD_VALUES;
		using GenericCoord2<T>::X;
		using GenericCoord2<T>::Y;

		typedef T base;

		inline GenericSize2():GenericCoord2<T>()
		{
		}
		inline GenericSize2(T width, T height): GenericCoord2<T>(width,height)
		{
		}
		inline explicit GenericSize2(const GenericCoord2<T>& c): GenericCoord2<T>(c)
		{
		}
		inline GenericSize2(const GenericSize2<T>& p): GenericCoord2<T>(p)
		{
		}

		inline void set(T width, T height)
		{
			GenericCoord2<T>::set(width,height);
		}

		GenericCoord2<T> getcoord() const
		{
			return GenericCoord2<T>(width(), height());
		}

		/* Accessors */
		inline T& operator[](int coord_index)
		{
			return(GenericCoord2<T>::operator[](coord_index));
		}

		inline T  operator[](int coord_index) const
		{
			return(GenericCoord2<T>::operator[](coord_index));
		}

		T& width()
		{
			return(GenericCoord2<T>::x());
		}
		T  width() const
		{
			return(GenericCoord2<T>::x());
		}
		T& height()
		{
			return(GenericCoord2<T>::y());
		}
		T  height() const
		{
			return(GenericCoord2<T>::y());
		}

		GenericSize2<T>& operator=(const GenericSize2<T>& p);
		GenericSize2<T>& operator+=(const GenericVector2<T>& v1);
		GenericSize2<T>& operator-=(const GenericVector2<T>& v1);

		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;

		blocxx::String name() const { return "Size2"; }
	}; // class GenericSize2

	template <class T>
	inline GenericSize2<T>& GenericSize2<T>::operator=(const GenericSize2<T>& p)
	{
		set( p.width(), p.height() );

		return *this;
	}

	template <class T>
	inline GenericSize2<T>& GenericSize2<T>::operator+=(const GenericVector2<T>& v1)
	{
		set( width() + v1.x(),
			height() + v1.y() );
		return *this;
	}

	template <class T>
	inline GenericSize2<T>& GenericSize2<T>::operator-=(const GenericVector2<T>& v1)
	{
		set( width() - v1.x(),
			height() - v1.y() );
		return *this;
	}

	template <class T>
	inline GenericVector2<T> operator-(const GenericSize2<T>& p1, const GenericSize2<T>& p2)
	{
		return GenericVector2<T>(p1.width() - p2.width(), p1.height() - p2.height());
	}

	template <class T>
	inline GenericSize2<T>  operator+(const GenericSize2<T>& p1, const GenericVector2<T>& v1)
	{
		return GenericSize2<T>(p1.width() + v1.x(), p1.height() + v1.y());
	}

	template <class T>
	inline GenericSize2<T>  operator-(const GenericSize2<T>& p1, const GenericVector2<T>& v1)
	{
		return GenericSize2<T>(p1.width() - v1.x(), p1.height() - v1.y());
	}

	template <class T>
	inline T averageComponent(const GenericSize2<T>& v)
	{
		return((v.width() + v.height()) / 2);
	}

	template <class T>
	inline T maxComponent(const GenericSize2<T>& v)
	{
		return(std::max(v.width(), v.height()));
	}

	template <class T>
	void GenericSize2<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		GenericCoord2<T>::collectInternalMembers(collector);
	}

	template <class T>
	blocxx::String GenericSize2<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericSize2<T>::name();
		}

		return indent.initial() + tag +
			"(" +
			graphlib::toString(width(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(height(), indent.indentInside(), prefix) +
			")";
	}

} // namespace graphlib

#endif /* !defined(GRAPHLIB__GenericSize2_HPP) */
