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


//
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

/**
 * @author Peter Dimov
 * @author Dan Nuffer
 */

#ifndef BLOCXX_COW_INTRUSIVE_REFERENCE_HPP_INCLUDE_GUARD_
#define BLOCXX_COW_INTRUSIVE_REFERENCE_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"

#ifdef BLOCXX_CHECK_NULL_REFERENCES
#include "miniblocxx/ReferenceHelpers.hpp"
#endif

namespace BLOCXX_NAMESPACE
{

/**
 * COWIntrusiveReference
 * A smart pointer that uses intrusive reference counting. It supports
 * 'copy on write' functionality.
 * The 'intrusive' in the class names comes from the fact that
 * referenced objects are required to be sub-classes of
 * COWIntrusiveCountableBase. If your looking for a non-intrusive smart
 * smart pointer class that providers copy on write functionality,
 * check out the COWReference class.
 *
 * This class relies on calls to
 *
 *      void COWIntrusiveReferenceAddRef(T* p);
 *      void COWIntrusiveReferenceRelease(T* p);
 *      bool COWIntrusiveReferenceUnique(T* p);
 *      T* COWIntrusiveReferenceClone(T* p);
 *
 *          (p != 0)
 *
 *  The object is responsible for destroying itself.
 *
 * If you want your class to be managed by COWIntrusiveReference, you can
 * derive it from COWIntrusiveCountableBase, or write your own set of
 * functions.
 */

template<class T> class COWIntrusiveReference
{
private:
	typedef COWIntrusiveReference this_type;
public:
	typedef T element_type;

	/**
	 * Default constructor
	 * The underlying object pointer will be NULL.
	 */
	COWIntrusiveReference(): m_pObj(0)
	{
	}
	/**
	 * Construct a COWIntrusiveReference that will contain a pointer
	 * to a COWIntrusiveCountableBase object.
	 * @param p A pointer to a COWIntrusiveCountableBase derivative.
	 * @param addRef If true the reference count will be incremented before
	 *		the constructor returns. Otherwise thre reference count is left alone.
	 */
	COWIntrusiveReference(T * p, bool addRef = true): m_pObj(p)
	{
		if (m_pObj != 0 && addRef) COWIntrusiveReferenceAddRef(m_pObj);
	}
	/**
	 * Copy constructor. This takes a COWIntrusiveReference of a type
	 * derived from T.
	 * This constructor will cause this COWIntrusiveReference object to
	 * share the same underlying COWIntrusiveCountableBase pointer with
	 * another. This will cause the reference count to get incremented on
	 * the underlying object.
	 * @param rhs The object to copy the COWIntrusiveCountableBase
	 * 		pointer from.
	 */
	template<class U> COWIntrusiveReference(COWIntrusiveReference<U> const & rhs): m_pObj(rhs.m_pObj)
	{
		if (m_pObj != 0) COWIntrusiveReferenceAddRef(m_pObj);
	}
	/**
	 * Copy constructor. This constructor will cause this
	 * COWIntrusiveReference object to share the same underlying
	 * COWIntrusiveCountableBase pointer with another. This will cause the
	 * reference count to get incremented on the underlying object.
	 * @param rhs The object to copy the COWIntrusiveCountableBase
	 * 		pointer from.
	 */
	COWIntrusiveReference(COWIntrusiveReference const & rhs): m_pObj(rhs.m_pObj)
	{
		if (m_pObj != 0) COWIntrusiveReferenceAddRef(m_pObj);
	}
	/**
	 * Destroy this COWIntrusiveReference. If the reference count to the
	 * underlying COWIntrusiveCountableBase object is zero after it is
	 * decremented in this method it will be deleted.
	 */
	~COWIntrusiveReference()
	{
		if (m_pObj != 0) COWIntrusiveReferenceRelease(m_pObj);
	}

	/**
	 * Assignment operator that that takes a COWIntrusiveReference of a type
	 * derived from T.
	 * This method will cause this COWIntrusiveReference object to
	 * share the same underlying COWIntrusiveCountableBase pointer with
	 * another. This will cause the reference count to get incremented on
	 * the underlying object.
	 * @param rhs The object to copy the COWIntrusiveCountableBase
	 * 		pointer from.
	 * @return A reference to this COWIntrusiveReference object.
	 */
	template<class U> COWIntrusiveReference & operator=(COWIntrusiveReference<U> const & rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}
	/**
	 * Assignment operator.
	 * This method will cause this COWIntrusiveReference object to
	 * share the same underlying COWIntrusiveCountableBase pointer with
	 * another. This will cause the reference count to get incremented on
	 * the underlying object.
	 * @param rhs The object to copy the COWIntrusiveCountableBase
	 * 		pointer from.
	 * @return A reference to this COWIntrusiveReference object.
	 */
	COWIntrusiveReference & operator=(COWIntrusiveReference const & rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}
	/**
	 * Assignment operator.
	 * This changes the underlying COWIntrusiveCountableBase pointer
	 * to the one passed to this method.
	 * @param rhs A pointer to a COWIntrusiveCountableBase object. this
	 * 		will be the new underlying pointer for this object.
	 * @return A reference to this COWIntrusiveReference object.
	 */
	COWIntrusiveReference & operator=(T * rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}
	/**
	 * @return A read only pointer to the underlying
	 * COWIntrusiveCountableBase object.
	 */
	const T * getPtr() const
	{
		return m_pObj;
	}

	/**
	 * @return A read only reference to the underlying
	 * COWIntrusiveCountableBase object.
	 */
	const T & operator*() const
	{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
		ReferenceHelpers::checkNull(this);
		ReferenceHelpers::checkNull(m_pObj);
#endif
		return *m_pObj;
	}

	/**
	 * @return A read only pointer to the underlying
	 * COWIntrusiveCountableBase object.
	 */
	const T * operator->() const
	{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
		ReferenceHelpers::checkNull(this);
		ReferenceHelpers::checkNull(m_pObj);
#endif
		return m_pObj;
	}

	/**
	 * @return A read/write reference to the underlying
	 * COWIntrusiveCountableBase object.
	 */
	T & operator*()
	{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
		ReferenceHelpers::checkNull(this);
		ReferenceHelpers::checkNull(m_pObj);
#endif
		getWriteLock();
		return *m_pObj;
	}

	/**
	 * @return A read/write pointer to the underlying
	 * COWIntrusiveCountableBase object.
	 */
	T * operator->()
	{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
		ReferenceHelpers::checkNull(this);
		ReferenceHelpers::checkNull(m_pObj);
#endif
		getWriteLock();
		return m_pObj;
	}

	typedef T * this_type::*unspecified_bool_type;
	operator unspecified_bool_type () const
	{
		return m_pObj == 0? 0: &this_type::m_pObj;
	}

	/**
	 * Negation operator
	 * @return true if this objects COWIntrusiveCountableBase pointer
	 * 		is NULL.
	 */
	bool operator! () const
	{
		return m_pObj == 0;
	}

	void swap(COWIntrusiveReference & rhs)
	{
		T * tmp = m_pObj;
		m_pObj = rhs.m_pObj;
		rhs.m_pObj = tmp;
	}

#if !defined(__GNUC__) || __GNUC__ > 2 // causes gcc 2.95 to ICE
	/* This is so the templated constructor will work */
	template <class U> friend class COWIntrusiveReference;
private:
#endif

	/**
	 * Create a clone of the COWIntrusiveCountableBase object if
	 * there is more than one reference to it. This method is
	 * used to support the copy on write functionality.
	 */
	void getWriteLock()
	{
		if ((m_pObj != 0) && !COWIntrusiveReferenceUnique(m_pObj))
		{
			m_pObj = COWIntrusiveReferenceClone(m_pObj);
		}
	}


	T * m_pObj;
};
template<class T, class U> inline bool operator==(COWIntrusiveReference<T> const & a, COWIntrusiveReference<U> const & b)
{
	return a.getPtr() == b.getPtr();
}
template<class T, class U> inline bool operator!=(COWIntrusiveReference<T> const & a, COWIntrusiveReference<U> const & b)
{
	return a.getPtr() != b.getPtr();
}
template<class T> inline bool operator==(COWIntrusiveReference<T> const & a, const T * b)
{
	return a.getPtr() == b;
}
template<class T> inline bool operator!=(COWIntrusiveReference<T> const & a, const T * b)
{
	return a.getPtr() != b;
}
template<class T> inline bool operator==(const T * a, COWIntrusiveReference<T> const & b)
{
	return a == b.getPtr();
}
template<class T> inline bool operator!=(const T * a, COWIntrusiveReference<T> const & b)
{
	return a != b.getPtr();
}
#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
// Resolve the ambiguity between our op!= and the one in rel_ops
template<class T> inline bool operator!=(COWIntrusiveReference<T> const & a, COWIntrusiveReference<T> const & b)
{
	return a.getPtr() != b.getPtr();
}
#endif
template<class T> inline bool operator<(COWIntrusiveReference<T> const & a, COWIntrusiveReference<T> const & b)
{
	return a.getPtr() < b.getPtr();
}
template<class T> void swap(COWIntrusiveReference<T> & lhs, COWIntrusiveReference<T> & rhs)
{
	lhs.swap(rhs);
}

} // end namespace BLOCXX_NAMESPACE

#endif

