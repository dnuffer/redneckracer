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

#ifndef BLOCXX_COW_INTRUSIVE_COUNTABLE_BASE_HPP_INCLUDE_GUARD_
#define BLOCXX_COW_INTRUSIVE_COUNTABLE_BASE_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/RefCount.hpp"

namespace BLOCXX_NAMESPACE
{

/**
 * COWIntrusiveCountableBase is the base class for all classes that are
 * managed by the COWIntrusiveReference. You MUST derive from this class
 * if you want to pass COWIntrusiveReference objects around for instances
 * of your class.
 * NOTE: If you are using multiple inheritance when you derive from this
 * class, derive virtually. Example:
 *		class Foo : public virtual COWIntrusiveCountableBase,
 *					public virtual SomeOtherBaseClass
 *		{
 *		...
 *		};
 *
 * Derived classes MUST implement: Derived* clone()
 */

class COWIntrusiveCountableBase;
void COWIntrusiveReferenceAddRef(COWIntrusiveCountableBase * p);
void COWIntrusiveReferenceRelease(COWIntrusiveCountableBase * p);
bool COWIntrusiveReferenceUnique(COWIntrusiveCountableBase* p);
template <typename T> T* COWIntrusiveReferenceClone(T* p);

class BLOCXX_COMMON_API COWIntrusiveCountableBase
{
private:
	RefCount m_usecount;

protected:
	/**
	 * Copy constructor.
	 */
	COWIntrusiveCountableBase(COWIntrusiveCountableBase const &)
		: m_usecount(0)
	{}

	/**
	 * Assignment operator. This method does NOTHING.
	 * @param x The COWIntrusiveCountableBase to assign to this one.
	 * @return A reference to this COWIntrusiveCountableBase object.
	 */
	COWIntrusiveCountableBase & operator=(COWIntrusiveCountableBase const &x)
	{
		(void) x;
		// don't assign or change the ref count, since it won't be different.
		return *this;
	}

	/**
	 * Default constructor.
	 */
	COWIntrusiveCountableBase(): m_usecount(0)
	{
	}

	/**
	 * Destroy this COWIntrusiveCountableBase object.
	 */
	virtual ~COWIntrusiveCountableBase();

	/**
	 * @return The RefCount object for this object.
	 */
	RefCount getRefCount() const
	{
		return m_usecount;
	}

public:
	/**
	 * Increment the reference count for a COWIntrusiveCountableBase object.
	 * @param p The COWIntrusiveCountableBase to increment the reference
	 *		count for.
	 */
	inline friend void COWIntrusiveReferenceAddRef(COWIntrusiveCountableBase * p)
	{
		p->m_usecount.inc();
	}

	/**
	 * Release a reference on a COWIntrusiveCountableBase. If the reference
	 * count is zero after the release, the COWIntrusiveCountableBase object
	 * will be deleted.
	 * @param p The COWIntrusiveCountableBase object to release the reference
	 *		for.
	 */
	inline friend void COWIntrusiveReferenceRelease(COWIntrusiveCountableBase * p)
	{
		if (p->m_usecount.decAndTest())
			delete p;
	}

	/**
	 * Determine of a COWIntrusiveCountableBase only has one reference.
	 * @param p The COWIntrusiveCountableBase object to check for a
	 *		single reference.
	 * @return true if is COWIntrusiveCountableBase has only one reference.
	 */
	inline friend bool COWIntrusiveReferenceUnique(COWIntrusiveCountableBase* p)
	{
		return p->m_usecount.get() == 1;
	}

	/**
	 * Have a COWIntrusiveCountableBase object create a clone of itself.
	 * @param p The COWIntrusiveCountableBase object to clone.
	 * @return A pointer to the new COWIntrusiveCountableBase object.
	 */
	template <typename T>
	friend T* COWIntrusiveReferenceClone(T* p);
};

template <typename T>
inline T* COWIntrusiveReferenceClone(T* p)
{
	// this needs to happen first to avoid a race condition between
	// another thread deleting the object and this one making a copy.
	T* tmp = p->clone();
	if (p->m_usecount.decAndTest())
	{
		// only copy--don't need to clone, also not a race condition.
		// undo the decAndTest.
		p->m_usecount.inc();
		delete tmp; // we won't need this anymore.
		return p;
	}
	else
	{
		// need to become unique
		if (tmp)
			COWIntrusiveReferenceAddRef(tmp);

		return tmp;
	}
}

} // end namespace BLOCXX_NAMESPACE

#endif


