/*
 * $Id: GenericRGBColor.hpp,v 1.1.2.1 2010/08/30 06:00:50 kpharris Exp $
 *
 * Part of "Graphlib" a playground for graphics development
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

#if       !defined(GRAPHLIB_GENERIC_RGBCOLOR_HPP)
#define            GRAPHLIB_GENERIC_RGBCOLOR_HPP
#include "StringDumpable.hpp"
#include "miniblocxx/Larger.hpp"
#include "miniblocxx/stl/limits"

namespace graphlib
{
	template <typename T>
	struct RGBColorTraits
	{
		typedef T value_type;
		static T min() throw() { return 0; }
		static T max() throw() { return std::numeric_limits<T>::is_exact ? std::numeric_limits<T>::max() : T(1); }
		static T clamp(T t, T tmin, T tmax) { return std::max(std::min(t, tmax), tmin); }
		static T clamp(T t) { return clamp(t, min(), max()); }
		static const bool have_alpha = false;

		enum COLOR_VALUES
		{
			R = 0, G = 1, B = 2
		};
	};

	template <typename T>
	struct BGRColorTraits
	{
		typedef T value_type;
		static T min() throw() { return 0; }
		static T max() throw() { return std::numeric_limits<T>::is_exact ? std::numeric_limits<T>::max() : T(1); }
		static T clamp(T t, T tmin, T tmax) { return std::max(std::min(t, tmax), tmin); }
		static T clamp(T t) { return clamp(t, min(), max()); }

		static const bool have_alpha = false;

		enum COLOR_VALUES
		{
			B = 0, G = 1, R = 2
		};
	};

	/**
	 *
	 * A generic class to represent RGB colors.  Note that only built-in (or
	 * native) types (char, float, etc), are usable in this class.  The design of
	 * this class was chosen so that array accesses ([]) and direct accesses
	 * (.N()) would be almost the same speed.  Also, this class *may* be able to
	 * be used in an array and reinterpreted as an array of the base type.
	 *
	 * For example:
	 *<pre>
	 *@@ GenericRGBColor<unsigned char> foo[100];
	 *@@ unsigned char* foo_reinterpreted = reinterpret_cast<char*>(foo);
	 *@@ ...
	 *</pre>
	 *
	 * The opposite should also be possible.  In either case, BE CAREFUL to make
	 * sure that the correct type of destructor would be called (especially with
	 * memory allocated by 'new').
	 *
	 * Whatver changes or additions are made in the future, NO virtual functions
	 * should be added, as they would likely break the ability to reinterpret the
	 * class as an array of three ints.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1.2.1 $
	 *
	 */
	template <typename T, typename ColorTraits = RGBColorTraits<T> >
	class GenericRGBColor
	{
	public:
		typedef T number_type;
		typedef ColorTraits color_traits_type;

		// This tag allows use in the StringDumpable.hpp toString functions
		// without requiring the overridden virtual functions.
		typedef bool CanBeStringDumpedTag;

		enum COLOR_VALUES
		{
			R = ColorTraits::R,
			G = ColorTraits::G,
			B = ColorTraits::B
		};

		static const bool HasAlpha = false;

	protected:
		T components[3]; ///< Components of RGB in an 'array'

	public:
		/** Default constructor (does nothing) */
		GenericRGBColor()
		{
		}

		GenericRGBColor(T a)
		{
			components[0] = a;
			components[1] = a;
			components[2] = a;
		}
		GenericRGBColor(const T a[3])
		{
			components[0] = a[0];
			components[1] = a[1];
			components[2] = a[2];
		}

		GenericRGBColor(T r, T g, T b)
		{
			set(r,g,b);
		}

		/** Destructor -- NON VIRTUAL! (does nothing) */
		~GenericRGBColor()
		{
		}

		/** Copy constructor */
		GenericRGBColor(const GenericRGBColor& old);

		/** Assignment operator */
		GenericRGBColor& operator= (const GenericRGBColor& old);

		inline void set_r(T r)
		{
			components[ColorTraits::R] = r;
		} ///< Set the red component
		inline void set_g(T g)
		{
			components[ColorTraits::G] = g;
		} ///< Set the green component
		inline void set_b(T b)
		{
			components[ColorTraits::B] = b;
		} ///< Set the blue component

		T& r()
		{
			return components[ColorTraits::R];
		}
		T  r() const
		{
			return components[ColorTraits::R];
		}
		T& g()
		{
			return components[ColorTraits::G];
		}
		T  g() const
		{
			return components[ColorTraits::G];
		}
		T& b()
		{
			return components[ColorTraits::B];
		}
		T  b() const
		{
			return components[ColorTraits::B];
		}

		/** Get the element specified.  No bounds checking is performed */
		inline T& operator[](unsigned index)
		{
			return components[index];
		}
		/** Get the element specified.  No bounds checking is performed */
		inline T  operator[](unsigned index) const
		{
			return components[index];
		}

		/** Set the components to the given values */
		void set(T r, T g, T b);

		GenericRGBColor& operator *=(T factor);          ///< Multiply by factor
		GenericRGBColor& operator /=(T factor);          ///< Divide by factor
		template <class U>
		GenericRGBColor& operator *=(U factor);          ///< Multiply by factor (general)
		template <class U>
		GenericRGBColor& operator /=(U factor);          ///< Divide by factor (general)
		GenericRGBColor& operator +=(const GenericRGBColor& r); ///< Add the given to these
		GenericRGBColor& operator -=(const GenericRGBColor& r); ///< Sub the given from these
		template <typename U, typename ColorTraitsU>
		GenericRGBColor& operator +=(const GenericRGBColor<U, ColorTraitsU>& r); ///< Add the given to these
		template <typename U, typename ColorTraitsU>
		GenericRGBColor& operator -=(const GenericRGBColor<U, ColorTraitsU>& r); ///< Sub the given from these


		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;

		blocxx::String name() const;
	}; // class GenericRGBColor

	/** Copy constructor */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>::GenericRGBColor(const GenericRGBColor& old)
	{
		components[0] = old.components[0];
		components[1] = old.components[1];
		components[2] = old.components[2];
	} // GenericRGBColor::GenericRGBColor(GenericRGBColor)

	/** Assignment operator */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator=(const GenericRGBColor& old)
	{
		// The generic check for self-assignment has been removed, as it has
		// already been specified that the type given MUST be a native type.  Thus,
		// it should be really fast to do the assignment, and the branch could be
		// very slow.
		components[0] = old.components[0];
		components[1] = old.components[1];
		components[2] = old.components[2];
		return *this;
	} // GenericRGBColor::operator=(GenericRGBColor)

	/** Sets the data... */
	template <typename T, typename ColorTraits>
	void GenericRGBColor<T, ColorTraits>::set(T r, T g, T b)
	{
		components[ColorTraits::R] = r;
		components[ColorTraits::G] = g;
		components[ColorTraits::B] = b;
	} // GenericRGBColor::set(r,g,b)

	/** Multiplication by a factor/assign (specific) */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator *=(T factor)
	{
		components[0] *= factor;
		components[1] *= factor;
		components[2] *= factor;
		return *this;
	} // GenericRGBColor::operator*=(T)

	/** Division by a factor/assign (specific) */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator /=(T factor)
	{
		components[0] /= factor;
		components[1] /= factor;
		components[2] /= factor;
		return *this;
	} // GenericRGBColor::operator/=(T)

	/** Multiplication by a factor/assign (general) */
	template <typename T, typename ColorTraits>
	template <class U>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator *=(U factor)
	{
		components[0] = T(components[0] * factor);
		components[1] = T(components[1] * factor);
		components[2] = T(components[2] * factor);
		return *this;
	} // GenericRGBColor::operator*=(U)

	/** Division by a factor/assign (general) */
	template <typename T, typename ColorTraits>
	template <class U>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator /=(U factor)
	{
		components[0] = T(components[0] / factor);
		components[1] = T(components[1] / factor);
		components[2] = T(components[2] / factor);
		return *this;
	} // GenericRGBColor::operator/=(U)

	/** addition/assign */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator +=(const GenericRGBColor<T, ColorTraits>& r)
	{
		components[0] += r.components[0];
		components[1] += r.components[1];
		components[2] += r.components[2];
		return *this;
	} // GenericRGBColor::operator+=(GenericRGBColor)

	/** subtraction/assign */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator -=(const GenericRGBColor<T, ColorTraits>& r)
	{
		components[0] -= r.components[0];
		components[1] -= r.components[1];
		components[2] -= r.components[2];
		return *this;
	} // GenericRGBColor::operator-=(GenericRGBColor)

	template <typename T, typename ColorTraits>
	template <typename U, typename ColorTraitsU>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator +=(const GenericRGBColor<U, ColorTraitsU>& r)
	{
		components[ColorTraits::R] += T(r.r());
		components[ColorTraits::G] += T(r.g());
		components[ColorTraits::B] += T(r.b());
		return *this;
	} // GenericRGBColor::operator+=(GenericRGBColor)

	/** subtraction/assign */
	template <typename T, typename ColorTraits>
	template <typename U, typename ColorTraitsU>
	GenericRGBColor<T, ColorTraits>& GenericRGBColor<T, ColorTraits>::operator -=(const GenericRGBColor<U, ColorTraitsU>& r)
	{
		components[ColorTraits::R] -= T(r.r());
		components[ColorTraits::G] -= T(r.g());
		components[ColorTraits::B] -= T(r.b());
		return *this;
	} // GenericRGBColor::operator-=(GenericRGBColor)

	/** Multiplication of a color by a factor (non-member) */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits> operator*(T factor, const GenericRGBColor<T, ColorTraits>& r)
	{
		GenericRGBColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(T,GenericRGBColor)

	/** Multiplication of a color by a factor (non-member, general) */
	template <typename T, typename ColorTraits, class U>
	GenericRGBColor<T, ColorTraits> operator*(U factor, const GenericRGBColor<T, ColorTraits>& r)
	{
		GenericRGBColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(U,GenericRGBColor)

	/** Multiplication of a color by a factor (non-member, general) */
	template <typename T, typename ColorTraits, class U>
	GenericRGBColor<T, ColorTraits> operator*(const GenericRGBColor<T, ColorTraits>& r, U factor)
	{
		GenericRGBColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(U,GenericRGBColor)

	/** Addition of two colors. */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits> operator+(const GenericRGBColor<T, ColorTraits>& c1, const GenericRGBColor<T, ColorTraits>& c2)
	{
		GenericRGBColor<T, ColorTraits> retval(c1);
		retval += c2;
		return retval;
	} // operator+(GenericRGBColor,GenericRGBColor)

	template <typename T, typename ColorTraits, typename U, typename ColorTraitsU>
	GenericRGBColor<T, ColorTraits> operator+(const GenericRGBColor<T, ColorTraits>& c1, const GenericRGBColor<U, ColorTraitsU>& c2)
	{
		GenericRGBColor<T, ColorTraits> retval(c1);
		retval += c2;
		return retval;
	} // operator+(GenericRGBColor,GenericRGBColor)

	/** Subtraction of two colors. */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits> operator-(const GenericRGBColor<T, ColorTraits>& c1, const GenericRGBColor<T, ColorTraits>& c2)
	{
		GenericRGBColor<T, ColorTraits> retval(c1);
		retval -= c2;
		return retval;
	} // operator-(GenericRGBColor,GenericRGBColor)

	template <typename T, typename ColorTraits, typename U, typename ColorTraitsU>
	GenericRGBColor<T, ColorTraits> operator-(const GenericRGBColor<T, ColorTraits>& c1, const GenericRGBColor<U, ColorTraitsU>& c2)
	{
		GenericRGBColor<T, ColorTraits> retval(c1);
		retval -= c2;
		return retval;
	} // operator-(GenericRGBColor,GenericRGBColor)

	/** Unary negation of a color. */
	template <typename T, typename ColorTraits>
	GenericRGBColor<T, ColorTraits> operator-(const GenericRGBColor<T, ColorTraits>& c)
	{
		return GenericRGBColor<T, ColorTraits>(-c.r(),-c.g(),-c.b());
	} // operator-(GenericRGBColor,GenericRGBColor)


	template <typename T, typename ColorTraits>
	inline T averageComponent(const GenericRGBColor<T, ColorTraits>& c)
	{
		return (c.r() + c.g() + c.b()) / T(3);
	}

	template <typename T, typename ColorTraits>
	inline T maxComponent(const GenericRGBColor<T, ColorTraits>& c)
	{
		return std::max(std::max(c.r(), c.g()), c.b());
	}

	template <typename T, typename ColorTraits>
	inline GenericRGBColor<T, ColorTraits> abs(const GenericRGBColor<T, ColorTraits>& c)
	{
		return GenericRGBColor<T, ColorTraits>(
			std::max(c.r(), -c.r()),
			std::max(c.g(), -c.g()),
			std::max(c.b(), -c.b()));
	}

	template <typename T, typename ColorTraits>
	void GenericRGBColor<T, ColorTraits>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("r", r());
		collector.addMember("g", g());
		collector.addMember("b", b());
	}

	template <typename T, typename ColorTraits>
	blocxx::String GenericRGBColor<T, ColorTraits>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericRGBColor<T, ColorTraits>::name();
		}

		return indent.initial() + tag +
			"[- " +
			graphlib::toString(r(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(g(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(b(), indent.indentInside(), prefix) +
			" -]";
	}

	template <typename T, typename ColorTraits>
	blocxx::String GenericRGBColor<T, ColorTraits>::name() const
	{
		char rgb[4] = "RGB";
		rgb[ColorTraits::R] = 'R';
		rgb[ColorTraits::G] = 'G';
		rgb[ColorTraits::B] = 'B';
		return rgb + blocxx::String("Color");
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB_GENERIC_RGBCOLOR_HPP) */
