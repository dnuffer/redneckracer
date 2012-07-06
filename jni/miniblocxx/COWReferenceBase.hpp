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
 * @author Dan Nuffer
 */

#ifndef BLOCXX_COW_REFERENCE_BASE_HPP_INCLUDE_GUARD_
#define BLOCXX_COW_REFERENCE_BASE_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/RefCount.hpp"

// The classes and functions defined in this file are not meant for general
// use, they are internal implementation details.  They may change at any time.

namespace BLOCXX_NAMESPACE
{

//////////////////////////////////////////////////////////////////////////////
template<class T>
inline void COWRefSwap(T& x, T&y)
{
	T t = x;
	x = y;
	y = t;
}

//////////////////////////////////////////////////////////////////////////////
// This class contains the non-templated code for COWReference, to help
// minimize code bloat.
class BLOCXX_COMMON_API COWReferenceBase
{
protected:
	COWReferenceBase()
		: m_pRefCount(new RefCount) {}
	COWReferenceBase(const COWReferenceBase& arg)
		: m_pRefCount(arg.m_pRefCount)
	{
		m_pRefCount->inc();
	}
	void incRef()
	{
		m_pRefCount->inc();
	}

	bool decRef()
	{
		if (m_pRefCount->decAndTest())
		{
			delete m_pRefCount;
			return true;
		}
		return false;
	}

	bool refCountGreaterThanOne() const
	{
		return m_pRefCount->get() > 1;
	}

	// returns true if we're the only copy.
	bool getWriteLock()
	{
		if (m_pRefCount->decAndTest())
		{
			// only copy--don't need to clone, also not a race condition
			incRef();
			return true;
		}
		else
		{
			// need to become unique
			m_pRefCount = new RefCount;
			return false;
		}
	}

	void swap(COWReferenceBase& arg)
	{
		COWRefSwap(m_pRefCount, arg.m_pRefCount);
	}

	void useRefCountOf(const COWReferenceBase& arg)
	{
		decRef();
		m_pRefCount = arg.m_pRefCount;
		incRef();
	}

protected:
	RefCount* volatile m_pRefCount;
};

} // end namespace BLOCXX_NAMESPACE

#endif


