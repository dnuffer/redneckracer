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

#ifndef BLOCXX_ASSERTION_HPP_INCLUDE_GUARD_
#define BLOCXX_ASSERTION_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include <assert.h>

#ifdef BLOCXX_NO_EXCEPTIONS
#define BLOCXX_ASSERT(CON) assert(CON)
#define BLOCXX_ASSERTMSG(CON,MSG) assert(CON)
#else // Have exception support
#include "miniblocxx/Exception.hpp"

namespace BLOCXX_NAMESPACE
{

BLOCXX_DECLARE_APIEXCEPTION(Assertion, BLOCXX_COMMON_API);

/**
 * BLOCXX_ASSERT works similar to the assert() macro, but instead of calling
 * abort(), it throws an AssertionException.
 * @param CON The condition which will be evaluated.
 */
#ifdef BLOCXX_DEBUG
#define BLOCXX_ASSERT(CON) if (!(CON)) throw ::BLOCXX_NAMESPACE::AssertionException(__FILE__, __LINE__, \
		#CON)
#else
#define BLOCXX_ASSERT(CON)
#endif

/**
 * BLOCXX_ASSERTMSG works the same as BLOCXX_ASSERT, but with a second string
 * parameter that will be added to the exception message on failure.
 * @param CON The condition which will be evaluated.
 * @param MSG The extra message.  A C string literal.
 */
#ifdef BLOCXX_DEBUG
// MSG should be a string
#define BLOCXX_ASSERTMSG(CON, MSG) if (!(CON)) throw ::BLOCXX_NAMESPACE::AssertionException(__FILE__, __LINE__, \
		#CON ":" MSG)
#else
#define BLOCXX_ASSERTMSG(CON, MSG)
#endif

} // end namespace BLOCXX_NAMESPACE
#endif // exception support

#endif
