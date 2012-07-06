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

#ifndef BLOCXX_COMMON_FWD_HPP_INCLUDE_GUARD_
#define BLOCXX_COMMON_FWD_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/ArrayFwd.hpp"
#include "miniblocxx/IntrusiveReference.hpp"

// Yeah I know this is forbidden by the standard, but what am I gonna do?  #include <algorithm> ? I think not.
// If it causes a problem on some compiler, just #ifdef a fix in.
#ifdef BLOCXX_WIN32
namespace std
{
	template <typename T> struct less;
}
#else
namespace std
{
	template <typename T> class less;
}
#endif

/**
 * A versioned blocxx namespace.  This exists only to separate
 * binary-incompatible releases.  User code should use the blocxx namespace alias
 * from "miniblocxx/BLOCXX_config.h" instead.
 */
namespace BLOCXX_NAMESPACE
{

struct LogMessage;

class BLOCXX_COMMON_API Logger;
typedef IntrusiveReference<Logger> LoggerRef;

class BLOCXX_COMMON_API LogAppender;
typedef IntrusiveReference<LogAppender> LogAppenderRef;

class BLOCXX_COMMON_API MultiAppender;
typedef IntrusiveReference<MultiAppender> MultiAppenderRef;

class String;
typedef Array<String> StringArray;

class Char16;
typedef Array<Char16> Char16Array;

template <class Key, class T, class Compare>
class SortedVectorMapDataCompare;

template<class Key, class T, class Compare = SortedVectorMapDataCompare<Key, T, std::less<Key> > >
class SortedVectorMap;

class StringBuffer;

class DateTime;

namespace Time
{
	class TimeDuration;
}

template <class T> class Enumeration;
typedef Enumeration<String> StringEnumeration;

class Thread;
typedef IntrusiveReference<Thread> ThreadRef;

class ThreadPool;
typedef IntrusiveReference<ThreadPool> ThreadPoolRef;

class Bool;
typedef Array<Bool>       			BoolArray;

class NonRecursiveMutexLock;

class NonRecursiveMutex;
class Mutex;

class UnnamedPipe;
typedef IntrusiveReference<UnnamedPipe> UnnamedPipeRef;

class File;
namespace FileSystem
{
	struct FileInformation;
}

class MD5;

class SharedLibraryLoader;
typedef IntrusiveReference<SharedLibraryLoader> SharedLibraryLoaderRef;

class SelectableIFC;
typedef IntrusiveReference<SelectableIFC> SelectableIFCRef;

class SelectableCallbackIFC;
typedef IntrusiveReference<SelectableCallbackIFC> SelectableCallbackIFCRef;

class Socket;

class SocketBaseImpl;
typedef IntrusiveReference<SocketBaseImpl> SocketBaseImplRef;

class ServerSocket;

class TempFileStream;

class Runnable;
typedef IntrusiveReference<Runnable> RunnableRef;

class ThreadCounter;
typedef IntrusiveReference<ThreadCounter> ThreadCounterRef;

class ThreadDoneCallback;
typedef IntrusiveReference<ThreadDoneCallback> ThreadDoneCallbackRef;

class CmdLineParser;
class CmdLineParserException;

//class PopenStreams;

class Timeout;
class TimeoutTimer;

class SSLServerCtx;
typedef IntrusiveReference<SSLServerCtx> SSLServerCtxRef;

class SSLClientCtx;
typedef IntrusiveReference<SSLClientCtx> SSLClientCtxRef;

class SSLTrustStore;
typedef IntrusiveReference<SSLTrustStore> SSLTrustStoreRef;

class SharedLibrary;
typedef IntrusiveReference<SharedLibrary> SharedLibraryRef;

class Process;
typedef IntrusiveReference<Process> ProcessRef;

class ThreadSafeProcess;
typedef IntrusiveReference<ThreadSafeProcess> ThreadSafeProcessRef;


namespace Exec
{
	class PreExec;
}

#ifdef BLOCXX_ENABLE_TEST_HOOKS
class FileSystemMockObject;
class ExecMockObject;
#endif

#ifdef BLOCXX_WIN32
template class BLOCXX_COMMON_API Array<String>;
#endif

class SelectEngine;

class UUID;

template <typename T>
class Enumerator;

template <typename T>
class RepeatEnumerator;

template <typename T>
class ArrayEnumerator;

template <typename T>
class TakeEnumerator;

template <typename T, typename U>
class MapEnumerator;

template <typename T>
class FilterEnumerator;



} // end namespace BLOCXX_NAMESPACE

#endif

