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
#include "miniblocxx/Mutex.hpp"
#include "miniblocxx/Assertion.hpp"
#include "miniblocxx/MutexImpl.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/Format.hpp"

namespace BLOCXX_NAMESPACE
{

BLOCXX_DECLARE_EXCEPTION(Mutex);
BLOCXX_DEFINE_EXCEPTION(Mutex);

Mutex::Mutex()
{
	int rv = MutexImpl::createMutex(m_mutex);
	if (rv != 0)
	{
		BLOCXX_THROW_ERRNO_MSG1(MutexException,
			Format("MutexImpl::createMutex failed with error %1", rv).c_str(),
			rv);
	}
}
Mutex::~Mutex()
{
	int rv = MutexImpl::destroyMutex(m_mutex);
	if (rv == -1)
	{
		assert(0 == "MutexImpl::destroyMutex failed because it is currently locked.");
	}
	else if (rv != 0)
	{
		assert(0 == "MutexImpl::destroyMutex failed.");
	}
}
void
Mutex::acquire()
{
	int rv = MutexImpl::acquireMutex(m_mutex);
	if (rv != 0)
	{
		BLOCXX_THROW_ERRNO_MSG1(MutexException,
			Format("MutexImpl::acquireMutex returned with error %1", rv).c_str(),
			rv);
	}
}
bool
Mutex::release()
{
	int rv = MutexImpl::releaseMutex(m_mutex);
	if ( rv != 0)
	{
		BLOCXX_THROW_ERRNO_MSG1(MutexException,
			Format("MutexImpl::releaseMutex returned with error %1", rv).c_str(),
			rv);
	}
	return true;
}

} // end namespace BLOCXX_NAMESPACE

