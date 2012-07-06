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
 * @author Jon Carey
 * @author Dan Nuffer
 */

#ifndef BLOCXX_REFERENCE_HPP_INCLUDE_GUARD_
#define BLOCXX_REFERENCE_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/ReferenceBase.hpp"
#include "miniblocxx/SafeBool.hpp"

#include "miniblocxx/Format.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/Demangle.hpp"

#ifdef BLOCXX_CHECK_NULL_REFERENCES
#include "miniblocxx/ReferenceHelpers.hpp"
#endif

namespace BLOCXX_NAMESPACE
{

//////////////////////////////////////////////////////////////////////////////
template<class T>
class Reference :
#if !defined(__GNUC__) || __GNUC__ > 2 // because of a gcc 2.95 ICE
	private ReferenceBase
#else
	public ReferenceBase
#endif
{
	public:
		typedef T element_type;

		Reference();
		explicit Reference(T* ptr);
		Reference(const Reference<T>& arg);

		/* construct out of a reference to a derived type.  U should be
		derived from T */
		template <class U>
		Reference(const Reference<U>& arg);
		~Reference();
		Reference<T>& operator= (const Reference<T>& arg);
		Reference<T>& operator= (T* newObj);
		void swap(Reference<T>& arg);
		T* operator->() const;
		T& operator*() const;
		T* getPtr() const;

		BLOCXX_SAFE_BOOL_IMPL(Reference, T* volatile, Reference::m_pObj, m_pObj)

		template <class U>
		Reference<U> cast_to() const;
		template <class U>
		void useRefCountOf(const Reference<U>&);
#if !defined(__GNUC__) || __GNUC__ > 2 // causes gcc 2.95 to ICE
		/* This is so the templated constructor will work */
		template <class U> friend class Reference;
	private:
#endif
		void decRef();
		T* volatile m_pObj;
};
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>::Reference()
	: ReferenceBase(), m_pObj(0)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>::Reference(T* ptr)
	: ReferenceBase(), m_pObj(ptr)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>::Reference(const Reference<T>& arg)
	: ReferenceBase(arg), m_pObj(arg.m_pObj)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
template<class U>
inline Reference<T>::Reference(const Reference<U>& arg)
  : ReferenceBase(arg),
  m_pObj(arg.m_pObj)
{
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>::~Reference()
{
	decRef();
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline void Reference<T>::decRef()
{
	typedef char type_must_be_complete[sizeof(T)];
	if (ReferenceBase::decRef())
	{
#ifdef BLOCXX_NO_EXCEPTIONS
		delete m_pObj;
#else
		const char* typeName = "UNKNOWN";
		if( m_pObj )
		{
			try
			{
				typeName = typeid(*m_pObj).name();
			}
			catch(...)
			{
			}
		}

		try
		{
			delete m_pObj;
		}
		catch(const Exception& e)
		{
			logErroneousException(Format("Exception caught in decRef() for %1 with runtime type Reference<%2>:", demangleTypeName(*this), demangleTypeName(typeName)).c_str(), e);
		}
		catch(const std::exception& e)
		{
			logErroneousException(Format("Exception caught in decRef() for %1 with runtime type Reference<%2>:", demangleTypeName(*this), demangleTypeName(typeName)).c_str(), e);
		}
		catch(...)
		{
			logErroneousException(Format("Exception caught in decRef() for %1 with runtime type Reference<%2>", demangleTypeName(*this), demangleTypeName(typeName)).c_str());
		}
#endif // BLOCXX_NO_EXCEPTIONS
	}
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>& Reference<T>::operator= (const Reference<T>& arg)
{
	Reference<T>(arg).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline Reference<T>& Reference<T>::operator= (T* newObj)
{
	Reference<T>(newObj).swap(*this);
	return *this;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
inline void Reference<T>::swap(Reference<T>& arg)
{
	ReferenceBase::swap(arg);
	RefSwap(m_pObj, arg.m_pObj);
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* Reference<T>::operator->() const
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif

	return m_pObj;
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline T& Reference<T>::operator*() const
{
#ifdef BLOCXX_CHECK_NULL_REFERENCES
	ReferenceHelpers::checkNull(this);
	ReferenceHelpers::checkNull(m_pObj);
#endif

	return *(m_pObj);
}
//////////////////////////////////////////////////////////////////////////////
template<class T>
inline T* Reference<T>::getPtr() const
{
	return m_pObj;
}
//////////////////////////////////////////////////////////////////////////////
template <class T>
template <class U>
inline Reference<U>
Reference<T>::cast_to() const
{
	Reference<U> rval;
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
Reference<T>::useRefCountOf(const Reference<U>& arg)
{
	ReferenceBase::useRefCountOf(arg);
}
//////////////////////////////////////////////////////////////////////////////
// Comparisons
template <class T, class U>
inline bool operator==(const Reference<T>& a, const Reference<U>& b)
{
	return a.getPtr() == b.getPtr();
}
//////////////////////////////////////////////////////////////////////////////
template <class T, class U>
inline bool operator!=(const Reference<T>& a, const Reference<U>& b)
{
	return a.getPtr() != b.getPtr();
}
//////////////////////////////////////////////////////////////////////////////
#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96
// Resolve the ambiguity between our op!= and the one in rel_ops
template <class T>
inline bool operator!=(const Reference<T>& a, const Reference<T>& b)
{
	return a.getPtr() != b.getPtr();
}
#endif
//////////////////////////////////////////////////////////////////////////////
template <class T, class U>
inline bool operator<(const Reference<T>& a, const Reference<U>& b)
{
	return a.getPtr() < b.getPtr();
}
//////////////////////////////////////////////////////////////////////////////
/**
 * Free function for compatibility with other libraries such as boost::mem_fn
 *
 * see http://www.boost.org/doc/libs/1_42_0/libs/bind/mem_fn.html
 */
template<class T> T * get_pointer(const Reference<T> &p)
{
   return p.getPtr();
}

} // end namespace BLOCXX_NAMESPACE

#endif	// BLOCXX_REFERENCE_HPP_INCLUDE_GUARD_
