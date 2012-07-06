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

#ifndef BLOCXX_TYPES_HPP_INCLUDE_GUARD_
#define BLOCXX_TYPES_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

#ifdef BLOCXX_WIN32

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <wtypes.h>

#ifdef max
#undef max
#endif

#endif

#ifndef __cplusplus
#error "BLOCXX_Types.hpp can only be included by c++ files"
#endif

extern "C"
{
#include <sys/types.h>
}

namespace BLOCXX_NAMESPACE
{

typedef unsigned char      			UInt8;
typedef signed char        			Int8;
#if BLOCXX_SIZEOF_SHORT_INT == 2
typedef unsigned short     			UInt16;
typedef short       				Int16;
#define BLOCXX_INT16_IS_SHORT 1
#elif BLOCXX_SIZEOF_INT == 2
typedef unsigned int     			UInt16;
typedef int       					Int16;
#define BLOCXX_INT16_IS_INT 1
#endif
#if BLOCXX_SIZEOF_INT == 4
typedef unsigned int       			UInt32;
typedef int         				Int32;
#define BLOCXX_INT32_IS_INT 1
#elif BLOCXX_SIZEOF_LONG_INT == 4
typedef unsigned long       		UInt32;
typedef long         				Int32;
#define BLOCXX_INT32_IS_LONG 1
#endif
#if BLOCXX_SIZEOF_LONG_INT == 8
typedef unsigned long   UInt64;
typedef long    Int64;
#define BLOCXX_INT64_IS_LONG 1
#elif BLOCXX_SIZEOF_LONG_LONG_INT == 8
typedef unsigned long long 			UInt64;
typedef long long   				Int64;
#define BLOCXX_INT64_IS_LONG_LONG 1
#else
#error "Compiler must support 64 bit long"
#endif
#if BLOCXX_SIZEOF_DOUBLE == 8
typedef double						Real64;
#define BLOCXX_REAL64_IS_DOUBLE 1
#elif BLOCXX_SIZEOF_LONG_DOUBLE == 8
typedef long double					Real64;
#define BLOCXX_REAL64_IS_LONG_DOUBLE 1
#endif
#if BLOCXX_SIZEOF_FLOAT == 4
typedef float						Real32;
#define BLOCXX_REAL32_IS_FLOAT 1
#elif BLOCXX_SIZEOF_DOUBLE == 4
typedef double						Real32;
#define BLOCXX_REAL32_IS_DOUBLE 1
#endif

#ifdef BLOCXX_WIN32

typedef HANDLE FileHandle;
typedef HANDLE Descriptor;
#define BLOCXX_INVALID_HANDLE INVALID_HANDLE_VALUE
#define BLOCXX_INVALID_FILEHANDLE INVALID_HANDLE_VALUE
typedef PSID uid_t;
typedef PSID UserId;
typedef PSID gid_t;
typedef PSID GroupId;
typedef HANDLE pid_t;
typedef HANDLE ProcId;
typedef int mode_t;
typedef long ssize_t;
typedef struct {} siginfo_t;

#define BLOCXX_SHAREDLIB_EXTENSION ".dll"
#define BLOCXX_FILENAME_SEPARATOR "\\"
#define BLOCXX_FILENAME_SEPARATOR_C '\\'
#define BLOCXX_PATHNAME_SEPARATOR ";"

#else //ifdef BLOCXX_WIN32

typedef int FileHandle;
typedef int Descriptor;
#define BLOCXX_INVALID_HANDLE -1
#define BLOCXX_INVALID_FILEHANDLE -1
typedef uid_t UserId;
typedef pid_t ProcId;
typedef gid_t GroupId;

#if defined BLOCXX_DARWIN
#define BLOCXX_SHAREDLIB_EXTENSION ".dylib.bundle"
#elif defined BLOCXX_HPUX && !defined(BLOCXX_ARCH_IA64)
#define BLOCXX_SHAREDLIB_EXTENSION ".sl"
#elif defined BLOCXX_HPUX
#define BLOCXX_SHAREDLIB_EXTENSION ".so"
#elif defined BLOCXX_NETWARE
#define BLOCXX_SHAREDLIB_EXTENSION ".nlm"
#else
#define BLOCXX_SHAREDLIB_EXTENSION ".so"
#endif

#define BLOCXX_FILENAME_SEPARATOR "/"
#define BLOCXX_FILENAME_SEPARATOR_C '/'
#define BLOCXX_PATHNAME_SEPARATOR ":"
#endif //ifdef BLOCXX_WIN32

} // end namespace BLOCXX_NAMESPACE

#endif
