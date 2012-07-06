/*******************************************************************************
* Copyright (C) 2005, 2009, Quest Software, Inc. All rights reserved.
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

#ifndef BLOCXX_MUTEX_INCLUDE_GUARD_HPP_
#define BLOCXX_MUTEX_INCLUDE_GUARD_HPP_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/ThreadTypes.hpp"

namespace BLOCXX_NAMESPACE
{

class BLOCXX_COMMON_API Mutex
{
public:
	/**
	 * Create a new recursive Mutex object.
	 */
	Mutex();
	/**
	 * Destroy this Mutex object.
	 */
	~Mutex();
	/**
	 * Acquire ownership of this Mutex object.
	 * This call will block if another thread has ownership of
	 * this Mutex. When it returns, the current thread will be
	 * the owner of this Mutex object.
	 */
	void acquire();
	/**
	 * Release ownership of this Mutex object. If another thread is waiting
	 * to acquire the ownership of this mutex it will stop blocking and acquire
	 * ownership when this call returns.
	 */
	bool release();
private:
	Mutex_t m_mutex;
	// noncopyable
	Mutex(const Mutex&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;
	Mutex& operator = (const Mutex&) BLOCXX_FUNCTION_NOT_IMPLEMENTED;
};

} // end namespace BLOCXX_NAMESPACE

#endif
