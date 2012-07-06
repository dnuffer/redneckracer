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

#ifndef BLOCXX_GLOBAL_PTR_HPP_INCLUDE_GUARD_
#define BLOCXX_GLOBAL_PTR_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/ThreadOnce.hpp"
#include "miniblocxx/SafeBool.hpp"

namespace BLOCXX_NAMESPACE
{
	/**
	 * This class is the default factory for GlobalPtr. create() returns a new instance of T constructed via the default
	 * constructor.
	 */
	template <typename T>
	struct DefaultConstructorFactory
	{
		static T* create()
		{
			return new T;
		}
	};

	/**
	 * This class can be used to store a global pointer. The pointer is lazily constructed. The pointer is
	 * never deleted. The intended use is for objects which must always exist for the lifetime of a program, but
	 * which may need to be polymorphic and replaced to enable plugging in a mock object for testing.
	 *
	 * GlobalPtr is a POD type, and thus must be initialized using POD initialization syntax:
	 *   GlobalPtr<myType> myGlobalPtr = BLOCXX_GLOBAL_PTR_INIT;
	 *
	 * The pointer will be lazily constructed. The first time get() is called, a new T (or derived) will be
	 * instantiated by a call to FactoryT::create(). The default type for FactoryT allocates a new instance of T
	 * using the default constructor. The initialization is done in a thread safe manner. The double checked
	 * locking pattern is used, which doesn't have a large overhead.
	 *
	 * The object will never be deleted, so if the destructor needs to do meaningful work, this class shouldn't be
	 * used.
	 *
	 * All calls to get() are thread safe.
	 * Calls to set() are not thread safe.
	 *
	 * @param T The type of the pointer.
	 * @param FactoryT To create the T*, FactoryT::create() will be called with no parameters. The return type
	 *                 must be convertible to a T*. The default will return a new T using the default constructor.
	 */
	template <typename T, typename FactoryT = DefaultConstructorFactory<T> >
	class GlobalPtr
	{
	private:
		struct InitPtr
		{
			InitPtr(T*& p)
			: m_p(p)
			{}
			T*& m_p;

			void operator()()
			{
				m_p = static_cast<T*>(FactoryT::create());
			}
		};

	public:
		/**
		 * Gets the current value of the pointer.
		 *
		 * @return T*
		 */
		T* get() const
		{
			callOnce(m_onceFlag, InitPtr(m_p));

			return m_p;
		}

		/**
		 * Sets the pointer.
		 * @param newP the new pointer.
		 *
		 * @return T* the previous value.
		 */
		T* set(T* newP)
		{
			T* oldP = get();
			m_p = newP;
			return oldP;
		}

		BLOCXX_SAFE_BOOL_IMPL(GlobalPtr, T*, GlobalPtr::m_p, get() != 0);

		T & operator*() const
		{
			return *get();
		}
		T * operator->() const
		{
			return get();
		}


		// These members should be treated as private. They aren't marked private because if they are, POD initializaer syntax can't be used.
		mutable T* m_p;
		mutable OnceFlag m_onceFlag;
	};

	/**
	 * This macro is provided to abstract the details of GlobalPtr. It should be used to initialize instances of
	 * GlobalPtr.
	 */
#define BLOCXX_GLOBAL_PTR_INIT { 0, BLOCXX_ONCE_INIT }


} // end namespace BLOCXX_NAMESPACE


#endif


