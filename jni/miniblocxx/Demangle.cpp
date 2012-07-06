/*******************************************************************************
* Copyright (C) 2010, Quest Software, Inc. All rights reserved.
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

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Demangle.hpp"

#include <cstdlib>
#if defined(BLOCXX_HAVE_CXXABI_H)
#include <cxxabi.h>
#endif

/**
 * @author Kevin Harris
 */

namespace BLOCXX_NAMESPACE
{

	String demangleTypeName(const char* const name)
	{
		String retval(name);

#if defined(BLOCXX_HAVE_CXXABI_H)
		int status = 0;
		char* demangledText = abi::__cxa_demangle(name, 0, 0, &status);

		if( demangledText )
		{
			if( status == 0 )
			{
				retval = String(demangledText);
			}
			free(demangledText);
		}
#elif defined(BLOCXX_WIN32)
		// TODO: Use UnDecorateSymbolName or _unDName to demangle this.
#endif
		return retval;
	}
} // end namespace BLOCXX_NAMESPACE
