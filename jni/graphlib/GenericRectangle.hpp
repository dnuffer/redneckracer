/*
 * $Id: Rectangle.hpp,v 1.1 2003/11/18 11:46:59 kpharris Exp $
 *
 * Part of "Graphlib"
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

#if       !defined(GRAPHLIB_GENERIC_RECTANGLE_HPP)
#define            GRAPHLIB_GENERIC_RECTANGLE_HPP

#include "GenericSize2.hpp"
#include "GenericPoint2.hpp"
#include "Fuzzy.hpp"
#include "GenericVector2.hpp"

namespace graphlib
{

	/**
	 *
	 * A simple Rectangle class.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1 $
	 *
	 */
	template<typename base>
	struct GenericRectangle
	{

		enum Components
		{
			E_Left = 0,
			E_Right,
			E_Top,
			E_Bottom
		};

		union
		{
			struct
			{
				base left;
				base right;
				base top;
				base bottom;
			};
			base components[4];
		};

		GenericRectangle(base l, base r, base t, base b);
		virtual ~GenericRectangle();
		GenericRectangle(const GenericRectangle& old);
		GenericRectangle& operator= (const GenericRectangle& old);

		base width() const { return std::abs(right - left); }
		base height() const { return std::abs(bottom - top); }

		static GenericRectangle makeCenteredOn(const GenericPoint2<base>& center, const GenericSize2<base>& size);

		base& operator[](Components c) { return components[c]; }
		base operator[](Components c) const { return components[c]; }

	}; // class GenericRectangle

	template<typename base>
	GenericRectangle<base>::GenericRectangle(base l, base r, base t, base b):
		left(l), right(r),
		top(t), bottom(b)
	{

	} // GenericRectangle()

	template<typename base>
	GenericRectangle<base>::~GenericRectangle()
	{

	} // ~GenericRectangle()

	template<typename base>
	GenericRectangle<base>::GenericRectangle(const GenericRectangle<base>& old):
		left(old.left), right(old.right),
		top(old.top), bottom(old.bottom)
	{

	} // GenericRectangle(GenericRectangle)

	template<typename base>
	GenericRectangle<base>& GenericRectangle<base>::operator= (const GenericRectangle<base>& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{
			left = old.left;
			right = old.right;
			top = old.top;
			bottom = old.bottom;
		}
		return (*this);
	} // GenericRectangle::operator=(GenericRectangle)

	template<typename base>
	GenericRectangle<base> GenericRectangle<base>::makeCenteredOn(const GenericPoint2<base>& center, const GenericSize2<base>& size)
	{
		return GenericRectangle<base>(
			center.x() - size.width()/base(2),
			center.x() + size.width()/base(2),
			center.y() + size.height()/base(2),
			center.y() - size.height()/base(2));
	}

	template<typename base>
	bool operator==(const GenericRectangle<base>& lhs, const GenericRectangle<base>& rhs)
	{
		return fuzzyEquals(lhs.left, rhs.left)
			&& fuzzyEquals(lhs.right, rhs.right)
			&& fuzzyEquals(lhs.top, rhs.top)
			&& fuzzyEquals(lhs.bottom, rhs.bottom);
	}

	template<typename base>
	inline GenericRectangle<base> operator+(const GenericRectangle<base>& rect, const GenericVector2<base>& vec)
	{
		return GenericRectangle<base>(
			rect.left + vec.x(),
			rect.right + vec.x(),
			rect.top + vec.y(),
			rect.bottom + vec.y());
	}

	template<typename base>
	bool operator!=(const GenericRectangle<base>& lhs, const GenericRectangle<base>& rhs)
	{
		return !(lhs == rhs);
	}

	template <typename base>
	bool intersecting(const GenericRectangle<base>& r1, const GenericRectangle<base>& r2)
	{
		if( fuzzyGreater(r2.left, r1.right) )
		{
			return false;
		}
		if( fuzzyGreater(r1.left, r2.right) )
		{
			return false;
		}
		if( fuzzyGreater(r1.bottom, r2.top) )
		{
			return false;
		}
		if( fuzzyGreater(r2.bottom, r1.top) )
		{
			return false;
		}
		return true;
	}

	template <typename base>
	GenericPoint2<base> center(const GenericRectangle<base>& r)
	{
		return GenericPoint2<base>(
			(r.left + r.right) / 2,
			(r.top + r.bottom) / 2);
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB_GENERIC_RECTANGLE_HPP) */

