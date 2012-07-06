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

#ifndef BLOCXX_THREAD_ONCE_HPP_INCLUDE_GUARD_
#define BLOCXX_THREAD_ONCE_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Assertion.hpp"

#if defined(BLOCXX_USE_PTHREAD)
#include <pthread.h>
#include <signal.h> // for sig_atomic_t
#include <assert.h>
#include "miniblocxx/MemoryBarrier.hpp"
#elif defined(BLOCXX_WIN32)
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <wtypes.h>
#endif

namespace BLOCXX_NAMESPACE
{

#ifdef BLOCXX_NCR
#ifndef PTHREAD_MUTEX_INITIALIZER
#define PTHREAD_MUTEX_INITIALIZER	{NULL, 0, 0}
#endif
#endif

#if defined(BLOCXX_USE_PTHREAD)

struct OnceFlag
{
	volatile ::sig_atomic_t flag;
	::pthread_mutex_t mtx;
};

#define BLOCXX_ONCE_INIT {0, BLOCXX_MUTEX_INITIALIZER}

#elif defined(BLOCXX_WIN32)

typedef long OnceFlag;
#define BLOCXX_ONCE_INIT 0

#else
#error "Port me!"
#endif

/**
 * The first time callOnce is called with a given onceFlag argument, it calls func with no argument and changes the value of flag to indicate
 * that func has been run.  Subsequent calls with the same onceFlag do nothing.
 */
template <typename FuncT>
void BLOCXX_COMMON_API callOnce(OnceFlag& flag, FuncT F);



#if defined(BLOCXX_USE_PTHREAD)

class CallOnce_pthread_MutexLock
{
public:
	CallOnce_pthread_MutexLock(::pthread_mutex_t* mtx)
		: m_mtx(mtx)
	{
#ifdef BLOCXX_NCR //we get coredump without initialization
		if (m_mtx->field1 == NULL)
		{
			pthread_mutexattr_t attr;
			int ret = pthread_mutexattr_create(&attr);
			assert(ret == 0);
			ret = pthread_mutex_init(m_mtx, attr);
			assert(ret == 0);
			pthread_mutexattr_delete(&attr);
		}
#endif

#ifndef NDEBUG
		int res =
#endif  // Avoid unused variable warnings
			pthread_mutex_lock(m_mtx);
		assert(res == 0);
	}
	~CallOnce_pthread_MutexLock()
	{
#ifndef NDEBUG
		int res =
#endif  // Avoid unused variable warnings
			pthread_mutex_unlock(m_mtx);
		assert(res == 0);

#ifdef BLOCXX_NCR
		pthread_mutex_destroy(m_mtx);
#endif
	}
private:
	::pthread_mutex_t* m_mtx;
};

template <typename FuncT>
inline void callOnce(OnceFlag& flag, FuncT f)
{
	readWriteMemoryBarrier();
	if (flag.flag == 0)
	{
		CallOnce_pthread_MutexLock lock(&flag.mtx);
		if (flag.flag == 0)
		{
			f();
			flag.flag = 1;
		}
	}
}

#endif

#if defined(BLOCXX_WIN32)

template <typename FuncT>
inline void callOnce(OnceFlag& flag, FuncT f)
{
	// this is the double-checked locking pattern, but with a bit more strength than normally implemented :-)
	if (InterlockedCompareExchange(&flag, 1, 1) == 0)
	{
		wchar_t mutexName[MAX_PATH];
		_snwprintf(mutexName, MAX_PATH, L"%X-%p-587ccea9-c95a-4e81-ac51-ab0ddc6cef63", GetCurrentProcessId(), &flag);
		mutexName[MAX_PATH - 1] = 0;

		HANDLE mutex = CreateMutexW(NULL, FALSE, mutexName);
		BLOCXX_ASSERT(mutex != NULL);

		int res = 0;
		res = WaitForSingleObject(mutex, INFINITE);
		BLOCXX_ASSERT(res == WAIT_OBJECT_0);

		if (InterlockedCompareExchange(&flag, 1, 1) == 0)
		{
			try
			{
				f();
			}
			catch (...)
			{
				res = ReleaseMutex(mutex);
				BLOCXX_ASSERT(res);
				res = CloseHandle(mutex);
				BLOCXX_ASSERT(res);
				throw;
			}
			InterlockedExchange(&flag, 1);
		}

		res = ReleaseMutex(mutex);
		BLOCXX_ASSERT(res);
		res = CloseHandle(mutex);
		BLOCXX_ASSERT(res);
	}
}

#endif

} // end namespace BLOCXX_NAMESPACE

#endif

