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
*       nor Network Associates,
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


#ifndef BLOCXX_AUTO_RESOURCE_HPP_INCLUDE_GUARD_
#define BLOCXX_AUTO_RESOURCE_HPP_INCLUDE_GUARD_

/**
* @author Kevin S. Van Horn
*/

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/SafeBool.hpp"

namespace BLOCXX_NAMESPACE
{

	/**
	* Utility class used in implementing AutoResource operations.
	*/
	template <typename Policy>
	struct AutoResourceRef
	{
		typedef typename Policy::handle_type ref_type;
		ref_type hdl;

		explicit AutoResourceRef(ref_type h)
		: hdl(h)
		{
		}
	};

	/**
	* PURPOSE: The AutoResource class template is an analog of std::auto_ptr
	* for managing arbitrary resource handles.  The Policy template argument
	* specifies what type of resource to manage and how to release the
	* resource.
	*
	* PROMISE: None of the operations of the class throw exceptions.
	*
	* REQUIRE: the Policy argument used below must be a class with these
	* properties:
	*
	* - Policy::handle_type is the type of resource handle to be managed.
	*
	* - Policy::null() is an expression convertible to Policy::handle_type,
	*   denoting an invalid resource handle.  It always returns the same
	*   value, and throws no exception.
	*
	* - Policy::free(h) is a legal expression whenever h is convertible to
	*   Policy::handle_type.  It frees the resource indicated by h, unless
	*   h is Policy::null(), in which case it does nothing.  It throws no
	*   exception.
	*
	* - Policy::equal(h1, h2) is a legal expression convertible to bool
	*   whenever h1 and h2 are convertible to Policy::handle_type.  Either
	*
	*   - (1) the expression is always false, or
	*
	*   - (2) the expression is true if and only if h1 and h2 are the same
	*     resource handle.
	*
	*   Note the additional requirements for reset(handle_type) and
	*   operator=(autorc_ref) in case (1).  The equal function never throws.
	*/
	template <typename Policy>
	class AutoResource
	{
		typedef typename Policy::handle_type handle_type;
		handle_type hdl;

	public:

		/**
		* Default (no argument) ctor initializes with value indicating no
		* resource currently owned.
		*/
		AutoResource() // throw()
		: hdl(Policy::null())
		{
		}

		/**
		* Take over ownership of h.
		*/
		explicit AutoResource(handle_type h) // throw()
		: hdl(h)
		{
		}

#if defined __HP_aCC
		/// Take over ownership of resource owned by x.
		//
		AutoResource(const AutoResource & x) // throw()
		: hdl(const_cast<AutoResource &>(x).release())
		{
		}
#else
		/// Take over ownership of resource owned by x.
		//
		AutoResource(AutoResource & x) // throw()
		: hdl(x.release())
		{
		}
#endif

#if defined __HP_aCC
		/// Assignment takes over ownership of resource owned by x.
		//
		AutoResource & operator=(const AutoResource & x) // throw()
		{
			reset(const_cast<AutoResource &>(x).release());
			return *this;
		}
#else
		/// Assignment takes over ownership of resource owned by x.
		//
		AutoResource & operator=(AutoResource & x) // throw()
		{
			reset(x.release());
			return *this;
		}
#endif
		/// Free resource when AutoResource object reaches end of lifetime.
		//
		~AutoResource() // throw()
		{
			Policy::free(hdl);
		}

		BLOCXX_SAFE_BOOL_IMPL(AutoResource<Policy>, handle_type, AutoResource<Policy>::hdl, (hdl != Policy::null()));

		/// Return handle of resource, retaining ownership.
		//
		handle_type get() const // throw()
		{
			return hdl;
		}

		/// Relinquish ownership of resource and return its handle.
		//
		handle_type release() // throw()
		{
			handle_type h = hdl;
			hdl = Policy::null();
			return h;
		}

		/**
		* Free resource and take over ownership of another.
		*
		* REQUIRE: if Policy::equal() always returns false, then
		* h must must not be the resource handle currently owned by *this.
		*/
		void reset(handle_type h) // throw()
		{
			if (!Policy::equal(h, hdl)) {
				Policy::free(hdl);
				hdl = h;
			}
		}

		void reset() // throw()
		{
			reset(Policy::null());
		}

		/**
		* Conversion to facilitate passing and returning AutoResource by value
		* (which transfers ownership).
		*/
		AutoResource(AutoResourceRef<Policy> href) // throw()
		: hdl(href.hdl)
		{
		}

		/**
		* Conversion to facilitate passing and returning AutoResource by value
		* (which transfers ownership).
		*/
		operator AutoResourceRef<Policy>() // throw()
		{
			return AutoResourceRef<Policy>(this->release());
		}

		/**
		* Converting assignment to facilitate returning autorc by value
		* (which transfers ownership).
		*
		* REQUIRE: if Policy::equal() always returns false, then href must not
		* contain the resource handle currently owned by *this;
		*/
		AutoResource & operator=(AutoResourceRef<Policy> href) // throw()
		{
			if (!Policy::equal(href.hdl, this->get()))
			{
				Policy::free(hdl);
				hdl = href.hdl;
			}
			return *this;
		}

	};

} // namespace BLOCXX_NAMESPACE

#endif
