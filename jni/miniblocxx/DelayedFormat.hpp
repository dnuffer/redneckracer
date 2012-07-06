/*******************************************************************************
* Copyright (C) 2005, Quest Software, Inc. All rights reserved.
* Copyright (C) 2006, Novell, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of
*       Quest Software, Inc.,
*       nor Novell, Inc.,
*       nor the names of its contributors or employees may be used to
*       endorse or promote products derived from this software without
*       specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/


/**
 * @author Kevin Harris
 */

#ifndef BLOCXX_DELAYED_FORMAT_HPP_INCLUDE_GUARD_
#define BLOCXX_DELAYED_FORMAT_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include <iosfwd>
#include "miniblocxx/Format.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Reference.hpp"

namespace BLOCXX_NAMESPACE
{
	// These "internals" define a base class interface for the templated derived
	// classes to provide.  By doing this, we can create an array of these
	// objects for simplicity of storage.
	namespace DelayedFormatInternals
	{
		// Provide an interface for a virtual << operator.
		class BLOCXX_COMMON_API DelayedFormatReferenceBase
		{
		protected:
			DelayedFormatReferenceBase();
		public:
			virtual ~DelayedFormatReferenceBase();
			std::ostream& dumpToStream(std::ostream& o) const;
			virtual String toString() const = 0;
		private:
			virtual std::ostream& doDumpToStream(std::ostream& o) const = 0;
		};
		std::ostream& operator<<(std::ostream&, const DelayedFormatReferenceBase& s);


		// The templated version which any non-const reference (that defines a <<
		// operator for std::ostream) can be used.
		template<class T>
		class DelayedFormatReference : public DelayedFormatReferenceBase
		{
		public:
			DelayedFormatReference(T& t): ref_(t)
			{
			}
			~DelayedFormatReference()
			{
			}
			virtual String toString() const
			{
				return toString(ref_);
			}
		private:
			DelayedFormatReference(const DelayedFormatReference&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;
			DelayedFormatReference& operator=(const DelayedFormatReference&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;

			// Dump it to the stream.
			virtual std::ostream& doDumpToStream(std::ostream& o) const
			{
				return o << ref_;
			}
			T& ref_;
		};
	}

	/**
	 * This is a class similar to the blocxx Format, except that all arguments
	 * are bound by reference and the formatting is performed at a later point
	 * in time.  Because the references are stored, no temporary values can be
	 * passed to the delayed formatter; store them in a variable first.
	 *
	 * For clarity and emphasis it is important to restate that NOTHING (except
	 * the format string) can be passed to this formatter whose lifetime does
	 * not exceed the instance of this formatter.
	 */
	class BLOCXX_COMMON_API DelayedFormat
	{
	public:
		/**
		 * This constructor allows for simple text output without non-format
		 * arguments.  The blocxx::Format class does not allow such behavior, as
		 * such, an empty string will be used as a parameter for creation of the
		 * output Format object.  That means that %1 will be a valid format
		 * conversion, but it will always be empty.
		 */
		DelayedFormat(const String& format);
		template <typename A>
		DelayedFormat(const String& format, A& a);
		template <typename A, typename B>
		DelayedFormat(const String& format, A& a, B& b);
		template <typename A, typename B, typename C>
		DelayedFormat(const String& format, A& a, B& b, C& c);
		template <typename A, typename B, typename C, typename D>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d);
		template <typename A, typename B, typename C, typename D, typename E>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e);
		template <typename A, typename B, typename C, typename D, typename E, typename F>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f);
		template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g);
		template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g, H& h);
		template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
		DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g, H& h, I& i);

		/** Format the output using the stored references and initial format string. */
		Format format() const;

		/** Simple conversion operator. */
		operator String() const;
		String toString() const;

		/** Format the stored references using a different format string than used for initialization. */
		Format formatWithString(const String& fs) const;
		Format formatWithString(const char* fs) const;
	private:
		/** This is a helper function to keep the various constructors simple. */
		template <class T>
		void append(T& t);

		/** The format string which will be passed to Format() when formatting this object. */
		String formatString;
		typedef Reference<DelayedFormatInternals::DelayedFormatReferenceBase> paramEntry;
		/** The parameters that will be passed to format. */
		Array<paramEntry> formatParameters;
	};

	template <typename T>
	void DelayedFormat::append(T& t)
	{
		formatParameters.append(paramEntry(new DelayedFormatInternals::DelayedFormatReference<T>(t)));
	}

	// Everything below here is just for the mess of constructors.
	template <typename A>
	DelayedFormat::DelayedFormat(const String& format, A& a)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(1);
		append(a);
	}

	template <typename A, typename B>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(2);
		append(a); append(b);
	}

	template <typename A, typename B, typename C>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(3);
		append(a); append(b); append(c);
	}

	template <typename A, typename B, typename C, typename D>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(4);
		append(a); append(b); append(c); append(d);
	}

	template <typename A, typename B, typename C, typename D, typename E>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(5);
		append(a); append(b); append(c); append(d); append(e);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(6);
		append(a); append(b); append(c); append(d); append(e); append(f);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(7);
		append(a); append(b); append(c); append(d); append(e); append(f); append(g);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g, H& h)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(8);
		append(a); append(b); append(c); append(d); append(e); append(f); append(g); append(h);
	}

	template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
	DelayedFormat::DelayedFormat(const String& format, A& a, B& b, C& c, D& d, E& e, F& f, G& g, H& h, I& i)
		: formatString(format), formatParameters()
	{
		formatParameters.reserve(9);
		append(a); append(b); append(c); append(d); append(e); append(f); append(g); append(h); append(i);
	}
} // end namespace BLOCXX_NAMESPACE

#endif
