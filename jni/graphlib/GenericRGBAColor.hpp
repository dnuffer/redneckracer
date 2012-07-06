/*
 * $Id: GenericRGBAColor.hpp,v 1.1.2.2 2010/08/30 07:03:49 kpharris Exp $
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

#if       !defined(GRAPHLIB_GENERIC_RGBACOLOR_HPP)
#define            GRAPHLIB_GENERIC_RGBACOLOR_HPP

#include "StringDumpable.hpp"
#include "GenericRGBColor.hpp"
#include "miniblocxx/Larger.hpp"
#include "miniblocxx/stl/algorithm.hpp"
//#include "miniblocxx/stl/limits.hpp"
#include <limits>



namespace graphlib
{
	template <typename T>
	struct RGBAColorTraits
	{
		static T min() throw() { return 0; }
		static T max() throw() { return std::numeric_limits<T>::is_exact ? std::numeric_limits<T>::max() : T(1); }
		static T clamp(T t, T tmin, T tmax) { return blocxx::max(blocxx::min(t, tmax), tmin); }
		static T clamp(T t) { return clamp(t, min(), max()); }

		static const bool have_alpha = true;

		enum COLOR_VALUES
		{
			R = 0, G = 1, B = 2, A = 3
		};
	};

	template <typename T>
	struct BGRAColorTraits
	{
		static T min() throw() { return 0; }
		static T max() throw() { return std::numeric_limits<T>::is_exact ? std::numeric_limits<T>::max() : T(1); }
		static T clamp(T t, T tmin, T tmax) { return blocxx::max(blocxx::min(t, tmax), tmin); }
		static T clamp(T t) { return clamp(t, min(), max()); }

		static const bool have_alpha = true;

		enum COLOR_VALUES
		{
			B = 0, G = 1, R = 2, A = 3
		};
	};

	/**
	 *
	 * A generic class to represent RGBA colors.  Note that only built-in (or
	 * native) types (char, float, etc), are usable in this class.  The design of
	 * this class was chosen so that array accesses ([]) and direct accesses
	 * (.N()) would be almost the same speed.  Also, this class *may* be able to
	 * be used in an array and reinterpreted as an array of the base type.
	 *
	 * For example:
	 *<pre>
	 *@@ GenericRGBAColor<unsigned char> foo[100];
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
	 * @version $Revision: 1.1.2.2 $
	 *
	 */
	template <typename T, typename ColorTraits = RGBAColorTraits<T> >
	class GenericRGBAColor
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
			B = ColorTraits::B,
			A = ColorTraits::A,
		};

		static const bool HasAlpha = true;

	protected:
		T components[4]; ///< Components of RGBA in an 'array'

	public:
		/** Default constructor (does nothing) */
		GenericRGBAColor()
		{
		}

		GenericRGBAColor(T a)
		{
			components[0] = a;
			components[1] = a;
			components[2] = a;
			components[3] = a;
		}
		GenericRGBAColor(const T a[4])
		{
			components[0] = a[0];
			components[1] = a[1];
			components[2] = a[2];
			components[3] = a[3];
		}

		GenericRGBAColor(T r, T g, T b)
		{
			set(r,g,b);
		}

		GenericRGBAColor(T r, T g, T b, T a)
		{
			set(r,g,b,a);
		}

		/** Destructor -- NON VIRTUAL! (does nothing) */
		~GenericRGBAColor()
		{
		}

		/** Copy constructor */
		GenericRGBAColor(const GenericRGBAColor& old);
		template <typename U, typename ColorTraitsU>
		GenericRGBAColor(const GenericRGBColor<U, ColorTraitsU>& old);

		/** Assignment operator */
		GenericRGBAColor& operator= (const GenericRGBAColor& old);

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
		inline void set_a(T a)
		{
			components[ColorTraits::A] = a;
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
		T& a()
		{
			return components[ColorTraits::A];
		}
		T  a() const
		{
			return components[ColorTraits::A];
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
		void set(T r, T g, T b, T a);

		GenericRGBAColor& operator *=(T factor);          ///< Multiply by factor
		GenericRGBAColor& operator /=(T factor);          ///< Divide by factor
		template <class U>
		GenericRGBAColor& operator *=(U factor);          ///< Multiply by factor (general)
		template <class U>
		GenericRGBAColor& operator /=(U factor);          ///< Divide by factor (general)
		GenericRGBAColor& operator +=(const GenericRGBAColor& r); ///< Add the given to these

		// Functions required for the tag-based toString interface
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String toString(const Indentation& indent = Indentation(), StringDumpable::PrefixType prefix = StringDumpable::E_PREFIX_NONE ) const;

		blocxx::String name() const;
	}; // class GenericRGBAColor

	/** Copy constructor */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits>::GenericRGBAColor(const GenericRGBAColor& old)
	{
		components[0] = old.components[0];
		components[1] = old.components[1];
		components[2] = old.components[2];
		components[3] = old.components[3];
	} // GenericRGBAColor::GenericRGBAColor(GenericRGBAColor)


	template <typename T, typename ColorTraits>
	template <typename U, typename ColorTraitsU>
	GenericRGBAColor<T, ColorTraits>::GenericRGBAColor(const GenericRGBColor<U, ColorTraitsU>& old)
	{
		set(old.r(), old.g(), old.b(), 0);
	}

	/** Assignment operator */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator=(const GenericRGBAColor& old)
	{
		// The generic check for self-assignment has been removed, as it has
		// already been specified that the type given MUST be a native type.  Thus,
		// it should be really fast to do the assignment, and the branch could be
		// very slow.
		components[0] = old.components[0];
		components[1] = old.components[1];
		components[2] = old.components[2];
		components[3] = old.components[3];
		return *this;
	} // GenericRGBAColor::operator=(GenericRGBAColor)

	/** Sets the data... */
	template <typename T, typename ColorTraits>
	void GenericRGBAColor<T, ColorTraits>::set(T r, T g, T b)
	{
		components[ColorTraits::R] = r;
		components[ColorTraits::G] = g;
		components[ColorTraits::B] = b;
		components[ColorTraits::A] = 0;
	} // GenericRGBAColor::set(r,g,b)

	template <typename T, typename ColorTraits>
	void GenericRGBAColor<T, ColorTraits>::set(T r, T g, T b, T a)
	{
		components[ColorTraits::R] = r;
		components[ColorTraits::G] = g;
		components[ColorTraits::B] = b;
		components[ColorTraits::A] = a;
	} // GenericRGBAColor::set(r,g,b,a)

	/** Multiplication by a factor/assign (specific) */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator *=(T factor)
	{
		components[ColorTraits::R] *= factor;
		components[ColorTraits::G] *= factor;
		components[ColorTraits::B] *= factor;

		// multiply the OPACITY by the factor, not the transparency.
		T newalpha = ColorTraits::max() - (ColorTraits::max() - components[ColorTraits::A]) * factor;
		components[ColorTraits::A] = ColorTraits::clamp(newalpha);
		return *this;
	} // GenericRGBAColor::operator*=(T)

	/** Division by a factor/assign (specific) */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator /=(T factor)
	{
		components[0] /= factor;
		components[1] /= factor;
		components[2] /= factor;
		components[3] /= factor;
		return *this;
	} // GenericRGBAColor::operator/=(T)

	/** Multiplication by a factor/assign (general) */
	template <typename T, typename ColorTraits>
	template <class U>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator *=(U factor)
	{
		components[0] = T(components[0] * factor);
		components[1] = T(components[1] * factor);
		components[2] = T(components[2] * factor);
		components[3] = T(components[3] * factor);
		return *this;
	} // GenericRGBAColor::operator*=(U)

	/** Division by a factor/assign (general) */
	template <typename T, typename ColorTraits>
	template <class U>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator /=(U factor)
	{
		components[0] = T(components[0] / factor);
		components[1] = T(components[1] / factor);
		components[2] = T(components[2] / factor);
		components[3] = T(components[3] / factor);
		return *this;
	} // GenericRGBAColor::operator/=(U)

	/** addition/assign */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits>& GenericRGBAColor<T, ColorTraits>::operator +=(const GenericRGBAColor<T, ColorTraits>& r)
	{
		const T m = ColorTraits::max();
		const typename blocxx::Larger<T>::type alpha1 = m - r.a();
		const typename blocxx::Larger<T>::type alpha2 = r.a();
		if( std::numeric_limits<T>::is_exact )
		{
			set(
				(this->r() * alpha2 + r.r() * alpha1) / m,
				(this->g() * alpha2 + r.g() * alpha1) / m,
				(this->b() * alpha2 + r.b() * alpha1) / m,
				this->a() * r.a() );
		}
		else
		{
			// This saves some FP divs by assuming m == T(1)
			set(
				this->r() * alpha2 + r.r() * alpha1,
				this->g() * alpha2 + r.g() * alpha1,
				this->b() * alpha2 + r.b() * alpha1,
				this->a() * r.a() );
		}
		return *this;
	} // GenericRGBAColor::operator+=(GenericRGBAColor)

	/** Multiplication of a color by a factor (non-member) */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits> operator*(T factor, const GenericRGBAColor<T, ColorTraits>& r)
	{
		GenericRGBAColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(T,GenericRGBAColor)

	/** Multiplication of a color by a factor (non-member, general) */
	template <typename T, typename ColorTraits, class U>
	GenericRGBAColor<T, ColorTraits> operator*(U factor, const GenericRGBAColor<T, ColorTraits>& r)
	{
		GenericRGBAColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(U,GenericRGBAColor)

	/** Multiplication of a color by a factor (non-member, general) */
	template <typename T, typename ColorTraits, class U>
	GenericRGBAColor<T, ColorTraits> operator*(const GenericRGBAColor<T, ColorTraits>& r, U factor)
	{
		GenericRGBAColor<T, ColorTraits> retval(r);
		retval *= factor;
		return retval;
	} // operator*(U,GenericRGBAColor)

	/** Addition of two colors. */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits> operator+(const GenericRGBAColor<T, ColorTraits>& c1, const GenericRGBAColor<T, ColorTraits>& c2)
	{
		GenericRGBAColor<T, ColorTraits> retval(c1);
		retval += c2;
		return retval;
	} // operator+(GenericRGBAColor,GenericRGBAColor)

	/** Unary negation of a color. */
	template <typename T, typename ColorTraits>
	GenericRGBAColor<T, ColorTraits> operator-(const GenericRGBAColor<T, ColorTraits>& c)
	{
		return GenericRGBAColor<T, ColorTraits>(-c.r(),-c.g(),-c.b(),ColorTraits::max() - c.a());
	} // operator-(GenericRGBAColor,GenericRGBAColor)


	template <typename T, typename ColorTraits>
	inline T averageComponent(const GenericRGBAColor<T, ColorTraits>& c)
	{
		// FIXME! Average the alpha?
		return (c.r() + c.g() + c.b()) / T(3);
	}

	template <typename T, typename ColorTraits>
	inline T maxComponent(const GenericRGBAColor<T, ColorTraits>& c)
	{
		// FIXME! Max the alpha?
		return blocxx::max(blocxx::max(c.r(), c.g()), c.b());
	}

	template <typename T, typename ColorTraits>
	inline GenericRGBAColor<T, ColorTraits> abs(const GenericRGBAColor<T, ColorTraits>& c)
	{
		return GenericRGBAColor<T, ColorTraits>(
			blocxx::max(c.r(), -c.r()),
			blocxx::max(c.g(), -c.g()),
			blocxx::max(c.b(), -c.b()),
			blocxx::max(c.a(), -c.a()));
	}

	template <typename T, typename ColorTraits>
	void GenericRGBAColor<T, ColorTraits>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("r", r());
		collector.addMember("g", g());
		collector.addMember("b", b());
		collector.addMember("a", a());
	}

	template <typename T, typename ColorTraits>
	blocxx::String GenericRGBAColor<T, ColorTraits>::toString(const Indentation& indent, StringDumpable::PrefixType prefix) const
	{
		blocxx::String tag;
		if( prefix == StringDumpable::E_PREFIX_CLASSNAME )
		{
			tag = GenericRGBAColor<T, ColorTraits>::name();
		}

		return indent.initial() + tag +
			"[- " +
			graphlib::toString(r(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(g(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(b(), indent.indentInside(), prefix) +
			"," +
			graphlib::toString(a(), indent.indentInside(), prefix) +
			" -]";
	}

	template <typename T, typename ColorTraits>
	blocxx::String GenericRGBAColor<T, ColorTraits>::name() const
	{
		char rgba[5] = "RGBA";
		rgba[ColorTraits::R] = 'R';
		rgba[ColorTraits::G] = 'G';
		rgba[ColorTraits::B] = 'B';
		rgba[ColorTraits::A] = 'A';
		return rgba + blocxx::String("Color");
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB_GENERIC_RGBACOLOR_HPP) */
