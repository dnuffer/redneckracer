/*******************************************************************************
* Copyright (C) 2005, 2010, Quest Software, Inc. All rights reserved.
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

#ifndef BLOCXX_COW_REFERENCE_HPP_INCLUDE_GUARD_
#define BLOCXX_COW_REFERENCE_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/COWReferenceBase.hpp"
#include "miniblocxx/SafeBool.hpp"

#ifdef BLOCXX_CHECK_NULL_REFERENCES
#include "miniblocxx/ReferenceHelpers.hpp"
#endif

namespace BLOCXX_NAMESPACE
{

/**
 * COWReference
 * A smart pointer that uses non-intrusive reference counting. It supports
 * 'copy on write' functionality.
 */
template<class T>
class COWReference : private COWReferenceBase
{
public:
	typedef T element_type;

	/**
	 * Default constructor
	 * The underlying object pointer will be NULL.
	 */
	COWReference();

	/**
	 * Construct a COWReference that will take ownership of a given pointer.
	 * @param ptr The pointer that will be owned by this COWReference.
	 */
	explicit COWReference(T* ptr);

	/**
	 * Copy constructor. This constructor will cause this COWReference object
	 * to share the same underlying object pointer with another. This will
	 * cause the reference count to get incremented the underlying object.
	 * @param arg The object to copy the object pointer from.
	 */
	COWReference(const COWReference<T>& arg);

	/**
	 * Copy constructor. This takes a COWReference of a type derived from T
	 * This constructor will cause this COWReference object to share the same
	 * underlying object pointer with another. This will cause the reference
	 * count to get incremented on the underlying object.
	 * @param arg The object to copy the object pointer from.
	 */
	template <class U>
	COWReference(const COWReference<U>& arg);

	/**
	 * Destroy this COWReference. If the reference count to the
	 * underlying object is zero after it is decremented in this method it
	 * will be deleted.
	 */
	~COWReference();

	/**
	 * Assignment operator.
	 * This changes the underlying object pointer to the one contained
	 * by the COWReference object passed as the argument.
	 * @param arg The COWReference object that contains the object pointer
	 * 		that will be assigned to this on.
	 * @return A reference to this COWReference object.
	 */
	COWReference<T>& operator= (const COWReference<T>& arg);

	/**
	 * Assignment operator.
	 * This will cause thise COWReference to release ownership of it's
	 * existing object pointer and assume ownership of another.
	 * @param newObj The new object pointer this COWReference will assume
	 * 		ownership of.
	 * @return A reference to thei COWReference object after the assignment
	 * 			as taken place.
	 */
	COWReference<T>& operator= (T* newObj);

	void swap(COWReference<T>& arg);

	/**
	 * @return A read/write pointer to the underlying object.
	 */
	T* operator->();

	/**
	 * @return A read only pointer to the underlying object.
	 */
	const T* operator->() const;

	/**
	 * @return A read/write reference to the underlying object.
	 */
	T& operator*();

	/**
	 * @return A read only reference to the underlying object.
	 */
	const T& operator*() const;

	/**
	 * @return A read only pointer to the underlying object.
	 */
	const T* getPtr() const;

	BLOCXX_SAFE_BOOL_IMPL(COWReference, T* volatile, COWReference::m_pObj, m_pObj)

	template <class U>
	COWReference<U> cast_to() const;

	template <class U>
	void useRefCountOf(const COWReference<U>&);

#if !defined(__GNUC__) || __GNUC__ > 2 // causes gcc 2.95 to ICE
	/* This is so the templated constructor will work */
	template <class U> friend class COWReference;
private:
#endif
	T* volatile m_pObj;
	void decRef();
	void getWriteLock();
};
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>::COWReference()
	: COWReferenceBase(), m_pObj(0)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>::COWReference(T* ptr)
	: COWReferenceBase(), m_pObj(ptr)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>::COWReference(const COWReference<T>& arg)
	: COWReferenceBase(arg), m_pObj(arg.m_pObj)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
template<class U>
inline COWReference<T>::COWReference(const COWReference<U>& arg)
	: COWReferenceBase(arg), m_pObj(arg.m_pObj)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>::~COWReference()
{
#ifndef BLOCXX_NO_EXCEPTIONS
	try
	{
		decRef();
	}
	catch (...)
	{
		// don't let exceptions escape
	}
#else
	decRef();
#endif
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline void COWReference<T>::decRef()
{
	typedef char type_must_be_complete[sizeof(T)];
	if (COWReferenceBase::decRef())
	{
		delete m_pObj;
		m_pObj = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////
template<class T>
inline void COWReference<T>::getWriteLock()
{
	if (COWReferenceBase::refCountGreaterThanOne())
	{
		// this needs to happen first to avoid a race condition between
		// another thread deleting the object and this one making a copy.
		T* tmp = COWReferenceClone(m_pObj);
		// this will decrement the count and then make a new one if we're making a copy.
		if (COWReferenceBase::getWriteLock())
		{
			delete tmp;
		}
		else
		{
			m_pObj = tmp;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>& COWReference<T>::operator= (const COWReference<T>& arg)
{
	COWReference<T>(arg).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline COWReference<T>& COWReference<T>::operator= (T* newObj)
{
	COWReference<T>(newObj).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
inline void COWReference<T>::swap(COWReference<T>& arg)
{
	COWReferenceBase::swap(arg);
	COWRefSwap(m_pObj, arg.m_pObj);
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* COWReference<T>::operator->()
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif
	getWriteLock();

	return m_pObj;
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline T& COWReference<T>::operator*()
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif
	getWriteLock();

	return *(m_pObj);
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline const T* COWReference<T>::operator->() const
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif

	return m_pObj;
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline const T& COWReference<T>::operator*() const
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif

	return *(m_pObj);
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline const T* COWReference<T>::getPtr() const
{
	return m_pObj;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
template <class U>
inline COWReference<U>
COWReference<T>::cast_to() const
{
	COWReference<U> rval;
	rval.m_pObj = dynamic_cast<U*>(m_pObj);
	if (rval.m_pObj)
	{
		rval.useRefCountOf(*this);
	}
	return rval;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
template <class U>
inline void
COWReference<T>::useRefCountOf(const COWReference<U>& arg)
{
	COWReferenceBase::useRefCountOf(arg);
}
//////////////////////////////////////////////////////////////////////////////
// Comparisons
template <class T, class U>
inline bool operator==(const COWReference<T>& a, const COWReference<U>& b)
{
	return a.getPtr() == b.getPtr();
}
//////////////////////////////////////////////////////////////////////////////
template <class T, class U>
inline bool operator!=(const COWReference<T>& a, const COWReference<U>& b)
{
	return a.getPtr() != b.getPtr();
}
//////////////////////////////////////////////////////////////////////////////
template <class T, class U>
inline bool operator<(const COWReference<T>& a, const COWReference<U>& b)
{
	return a.getPtr() < b.getPtr();
}

//////////////////////////////////////////////////////////////////////////////
template <class T>
inline T* COWReferenceClone(T* obj)
{
	// default implementation.  If a certain class doesn't have clone()
	// (like std::vector), then they can overload this function
	return obj->clone();
}

} // end namespace BLOCXX_NAMESPACE

#endif	// BLOCXX_COW_REFERENCE_HPP_INCLUDE_GUARD_
