/*
 * $Id: GenericONB.hpp,v 1.1.2.3 2010/03/07 18:43:35 kpharris Exp $
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

#if    !defined(GRAPHLIB__GENERIC_ONB_HPP)
#define         GRAPHLIB__GENERIC_ONB_HPP

/*
  GenericONB.hpp -- A class to represent an ortho-normal basis.

  An ortho-normal basis is a set of 3 mutually perpendicular (ortho) vectors
  which are all unit-length (normal).  As such, it can be used for simple
  transformations on vectors, points, and coordinates involving only a single
  rotation.
*/

/*
  Revision History:
  18May2001 Created this file.  Wrote constructors, accessors, and functions to
  convert to/from the ONB.
  18Sep2003 Set a namespace, renamed the file (hpp instead of h), added another
  constructor, changed the internal types to be GenericVector3 instead of
  GenericCoord3.
  ...
  02Mar2010 Converted for use in graphlib.  Added string dumping functions.
*/

/**
 * @author Kevin Harris
 */

#include "graphlib/StringDumpable.hpp"
#include "graphlib/GenericVector3.hpp"

namespace graphlib
{
	template <class T>
	class GenericONB
	{
	public:
		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		GenericONB():
			m_u(1,0,0),
			m_v(0,1,0),
			m_w(0,0,1)
		{
		}

		// An ONB where only the direction of w matters...
		GenericONB(const GenericVector3<T>& a):
			m_w(unit(a))
		{
			v = perp_vector(w);
			u = crossprod(v,w);
		}

		// An ONB where the direction of u,v, and w matter...
		// a and b define a plane, where a is in the direction of w, b is in the
		// direction of v, and u is the normal to this plane.
		GenericONB(const GenericVector3<T>& a, const GenericVector3<T>& b):
			m_v(unit(b)),
			m_w(unit(a))
		{
			u = crossprod(v,w);
		}

		// An ONB where the direction of u,v, and w are fully specified.
		GenericONB(const GenericVector3<T>& a, const GenericVector3<T>& b, const GenericVector3<T>& c):
			m_u(unit(a)),
			m_v(unit(b)),
			m_w(unit(c))
		{
		}

		GenericONB(const GenericONB<T>& o): m_u(o.m_u), m_v(o.m_v), m_w(o.m_w)
		{
		}

		GenericONB<T>& operator=(const GenericONB<T>& o)
		{
			if( &o != this )
			{
				m_u = o.u;
				m_v = o.v;
				m_w = o.w;
			}
			return(*this);
		}

		// Take an external coord, and convert to one for this ONB
		GenericCoord3<T> globalToLocal(const GenericCoord3<T>& c) const
		{
			GenericVector3<T> cv(c);
			return GenericCoord3<T>(dotprod(cv,m_u),dotprod(cv,m_v),dotprod(cv,m_w));
		}

		// Take an external point, and convert to one for this ONB
		GenericPoint3<T> globalToLocal(const GenericPoint3<T>& c) const
		{
			GenericVector3<T> cv(c.x(), c.y(), c.z());
			return GenericPoint3<T>(dotprod(cv,m_u),dotprod(cv,m_v),dotprod(cv,m_w));
		}

		// Take an external vector, and convert to one for this ONB
		GenericVector3<T> globalToLocal(const GenericVector3<T>& c) const
		{
			return GenericVector3<T>(dotprod(c,m_u),dotprod(c,m_v),dotprod(c,m_w));
		}

		// Take an internal coord, and convert to one for a 'global' ONB.
		GenericCoord3<T> localToGlobal(const GenericCoord3<T>& c) const
		{
			GenericVector3<T> cv(m_u * c.x() + m_v * c.y() + m_w * c.z());
			return( GenericCoord3<T>(cv.x(), cv.y(), cv.z()) );
		}

		// Take an internal point, and convert to one for a 'global' ONB.
		GenericPoint3<T> localToGlobal(const GenericPoint3<T>& c) const
		{
			GenericVector3<T> cv(m_u * c.x() + m_v * c.y() + m_w * c.z());
			return( GenericPoint3<T>(cv.x(), cv.y(), cv.z()) );
		}

		// Take an internal vector, and convert to one for a 'global' ONB.
		GenericVector3<T> localToGlobal(const GenericVector3<T>& c) const
		{
			return(m_u * c.x() + m_v * c.y() + m_w * c.z());
		}

		const GenericVector3<T>& u() const
		{
			return m_u;
		}
		const GenericVector3<T>& v() const
		{
			return m_v;
		}
		const GenericVector3<T>& w() const
		{
			return m_w;
		}

		GenericVector3<T>& u()
		{
			return m_u;
		}
		GenericVector3<T>& v()
		{
			return m_v;
		}
		GenericVector3<T>& w()
		{
			return m_w;
		}

		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;
		blocxx::String name() const { return "ONB"; }

	private:
		GenericVector3<T> m_u;
		GenericVector3<T> m_v;
		GenericVector3<T> m_w;
	}; // class ONB

	template <class T>
	void GenericONB<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("u", u());
		collector.addMember("v", v());
		collector.addMember("w", w());
	}

	template <class T>
	blocxx::String GenericONB<T>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericONB<T>::name();
		}
		return indent.initial() + tag +
			"{" +
			graphlib::toString(u(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(v(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(w(), indent.indentInside(), prefix) +
			"}";
	}

} // namespace graphlib

#endif /* !defined(GRAPHLIB__GENERIC_ONB_HPP) */
