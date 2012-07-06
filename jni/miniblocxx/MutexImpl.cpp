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
 * @author Jon Carey
 * @author Dan Nuffer
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/MutexImpl.hpp"
#include <errno.h>
#include <assert.h>

namespace BLOCXX_NAMESPACE
{

namespace MutexImpl
{

#if defined (BLOCXX_USE_PTHREAD)

#if !defined (BLOCXX_NCR)

/**
 * Create a platform specific mutext handle.
 * @param handle	The mutex handle that should be initialized by this method
 * @return 0 on success. Otherwise -1.
 */
int
createMutex(Mutex_t& handle)
{
	pthread_mutexattr_t attr;
	int res = pthread_mutexattr_init(&attr);
	assert(res == 0);
	if (res != 0)
	{
		return -1;
	}

#if defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	res = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	assert(res == 0);
	if (res != 0)
	{
		pthread_mutexattr_destroy(&attr);
		return -1;
	}
#endif

	res = pthread_mutex_init(&handle.mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	if (res != 0)
	{
		return -1;
	}

#if !defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	res = pthread_cond_init(&handle.unlocked, PTHREAD_COND_ATTR_DEFAULT);
	if (res != 0)
	{
		pthread_mutex_destroy(&handle.mutex);
		return -1;
	}

	handle.valid_id = false;
	handle.count = 0;
#endif
	return 0;
}

#else //#if !defined (BLOCXX_NCR)

int
createMutex(Mutex_t& handle)
{
	pthread_mutexattr_t attr;
	int res = pthread_mutexattr_create(&attr);
	assert(res == 0);
	if (res != 0)
	{
		return -1;
	}

#if defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	res = pthread_mutexattr_setkind_np(&attr, MUTEX_RECURSIVE_NP);
	assert(res == 0);
	if (res != 0)
	{
		pthread_mutexattr_delete(&attr);
		return -1;
	}
#endif

	res = pthread_mutex_init(&handle.mutex, attr);

	pthread_mutexattr_delete(&attr);
	if (res != 0)
	{
		return -1;
	}

#if !defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	res = pthread_cond_init(&handle.unlocked, PTHREAD_COND_ATTR_DEFAULT);
	if (res != 0)
	{
		pthread_mutex_destroy(&handle.mutex);
		return -1;
	}

	handle.valid_id = false;
	handle.count = 0;
#endif
	return 0;
}

#endif //#ifndef BLOCXX_NCR

/**
 * Destroy a mutex previously created with createMutex.
 * @param handle The handle to the mutex that will be destroyed.
 * @return The following error codes:
 *		 0:	success
 *		-1:	Could not be acquired for destruction because it is currently
 *				locked.
 *		-2:	All other error conditions
 */
int
destroyMutex(Mutex_t& handle)
{
	switch (pthread_mutex_destroy(&handle.mutex))
	{
		case 0:
			break;
		case EBUSY:
			return -1;
			break;
		default:
			return -2;
	}
	int res = 0;
#if !defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	res = pthread_cond_destroy(&handle.unlocked);
	assert(res == 0);
#endif
	return res;
}

/**
 * Acquire the mutex specified by a given mutex handle. This method should
 * block until the desired mutex can be acquired. The error return value is
 * used to indicate critical errors.
 *
 * @param handle The mutex to acquire.
 * @return 0 on success. -1 indicates a critical error.
 */
int
acquireMutex(Mutex_t& handle)
{
	int res = pthread_mutex_lock(&handle.mutex);
	assert(res == 0);

#if !defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	pthread_t tid = pthread_self();
	if (handle.valid_id && pthread_equal(handle.thread_id, tid))
	{
		++handle.count;
	}
	else
	{
		while (handle.valid_id)
		{
			res = pthread_cond_wait(&handle.unlocked, &handle.mutex);
			assert(res == 0 || res == EINTR);
		}

		handle.thread_id = tid;
		handle.valid_id = true;
		handle.count = 1;
	}

	res = pthread_mutex_unlock(&handle.mutex);
	assert(res == 0);
#endif
	return res;
}

/**
 * Release a mutex that was previously acquired with the acquireMutex
 * method.
 * @param handle The handle to the mutex that is being released.
 * @return 0 on success. -1 indicates a critical error.
 */
int
releaseMutex(Mutex_t& handle)
{
#if defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
	int res = pthread_mutex_unlock(&handle.mutex);
	assert(res == 0);
	return res;
#else
	int res = 0;
	res = pthread_mutex_lock(&handle.mutex);
	assert(res == 0);

	pthread_t tid = pthread_self();
	if (handle.valid_id && !pthread_equal(handle.thread_id, tid))
	{
		res = pthread_mutex_unlock(&handle.mutex);
		assert(res == 0);
		return -1;
	}

	if (--handle.count == 0)
	{
		assert(handle.valid_id);
		handle.valid_id = false;

		res = pthread_cond_signal(&handle.unlocked);
		assert(res == 0);
	}

	res = pthread_mutex_unlock(&handle.mutex);
	assert(res == 0);
	return res;
#endif //#if defined(BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE)
}

#endif //#if defined (BLOCXX_USE_PTHREAD)

#if defined(BLOCXX_WIN32)

int
createMutex(Mutex_t& handle)
{
	handle = new CRITICAL_SECTION;
	assert(handle);
	InitializeCriticalSection(handle);
	return 0;
}

int
destroyMutex(Mutex_t& handle)
{
	if(handle)
	{
		DeleteCriticalSection(handle);
		delete handle;
		handle = 0;
	}
	return 0;
}

int
acquireMutex(Mutex_t& handle)
{
	EnterCriticalSection(handle);
	return 0;
}

int
releaseMutex(Mutex_t& handle)
{
	LeaveCriticalSection(handle);
	return 0;
}

#endif //#if defined(BLOCXX_WIN32)

} // end namespace MutexImpl
} // end namespace BLOCXX_NAMESPACE

