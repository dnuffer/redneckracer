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

//
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef BLOCXX_INTRUSIVE_COUNTABLE_BASE_HPP_INCLUDE_GUARD_
#define BLOCXX_INTRUSIVE_COUNTABLE_BASE_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/RefCount.hpp"

namespace BLOCXX_NAMESPACE
{

/**
 * If you want your class to be managed by IntrusiveReference, then derive
 * from this class.  Note that if multiple inheritance is used, you must derive
 * "virtual"ly.
 */

class IntrusiveCountableBase;
void IntrusiveReferenceAddRef(IntrusiveCountableBase * p);
void IntrusiveReferenceRelease(IntrusiveCountableBase * p);
void intrusive_ptr_add_ref(IntrusiveCountableBase* p);
void intrusive_ptr_release(IntrusiveCountableBase* p);

class BLOCXX_COMMON_API IntrusiveCountableBase
{
private:
	RefCount m_useCount;

protected:
	IntrusiveCountableBase()
		: m_useCount(0)
	{
	}

	IntrusiveCountableBase(const IntrusiveCountableBase&)
		: m_useCount(0)
	{
	}

	IntrusiveCountableBase& operator=(const IntrusiveCountableBase&)
	{
		// don't change m_useCount
		return *this;
	}

	virtual ~IntrusiveCountableBase();

public:
	inline friend void IntrusiveReferenceAddRef(IntrusiveCountableBase * p)
	{
		p->m_useCount.inc();
	}

	inline friend void intrusive_ptr_add_ref(IntrusiveCountableBase * p)
	{
		p->m_useCount.inc();
	}

	inline friend void IntrusiveReferenceRelease(IntrusiveCountableBase * p)
	{
		if (p->m_useCount.decAndTest())
			delete p;
	}

	inline friend void intrusive_ptr_release(IntrusiveCountableBase * p)
	{
		if (p->m_useCount.decAndTest())
			delete p;
	}
};

} // end namespace BLOCXX_NAMESPACE

#endif

