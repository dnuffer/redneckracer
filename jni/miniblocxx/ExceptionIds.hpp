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

#ifndef BLOCXX_EXCEPTION_IDS_HPP_INCLUDE_GUARD
#define BLOCXX_EXCEPTION_IDS_HPP_INCLUDE_GUARD
#include "miniblocxx/BLOCXX_config.h"

namespace BLOCXX_NAMESPACE
{
	namespace ExceptionIds
	{
		// The purpose of this file/namespace is to centralize all the
		// exception subclass id numbers,
		// in order to to prevent id conflicts.  Numbering starts from -1,
		// and all numbers in BloCxx must be negative.

		// common library
		static const int HTTPExceptionId = -3;
		static const int XMLParseExceptionId = -4;
		static const int AssertionExceptionId = -5;
		static const int ValueCastExceptionId = -6;
		static const int AuthenticationExceptionId = -7;
		static const int UnknownHostExceptionId = -9;
		static const int SocketAddressExceptionId = -10;
		static const int SocketExceptionId = -11;
		static const int SocketTimeoutExceptionId = -12;
		static const int ConditionLockExceptionId = -13;
		static const int ConditionResourceExceptionId = -14;
		static const int ConfigExceptionId = -15;
		static const int NULLCOWReferenceExceptionId = -16;
		static const int DateTimeExceptionId = -17;
		static const int EnumerationExceptionId = -18;
		static const int ExecTimeoutExceptionId = -19;
		static const int ExecBufferFullExceptionId = -20;
		static const int ExecErrorExceptionId = -21;
		static const int IOExceptionId = -22;
		static const int BadSignatureExceptionId = -23;
		static const int LoggerExceptionId = -24;
		static const int DeadlockExceptionId = -25;
		static const int NoSuchPropertyExceptionId = -26;
		static const int NoSuchProviderExceptionId = -27;
		static const int NoSuchQualifierExceptionId = -28;
		static const int NULLValueExceptionId = -29;
		static const int ContextDataNotFoundExceptionId = -30;
		static const int OutOfBoundsExceptionId = -31;
		static const int FileSystemExceptionId = -32;
		static const int RWLockerExceptionId = -33;
		static const int NULLReferenceExceptionId = -34;
		static const int SelectExceptionId = -35;
		static const int SharedLibraryExceptionId = -36;
		static const int StringConversionExceptionId = -37;
		static const int TimeoutExceptionId = -38;
		static const int ThreadExceptionId = -39;
		static const int CancellationDeniedExceptionId = -40;
		static const int ThreadBarrierExceptionId = -41;
		static const int MalformedURLExceptionId = -42;
		static const int UUIDExceptionId = -43;
		static const int UnnamedPipeExceptionId = -44;
		static const int InvalidUTF8ExceptionId = -45;
		static const int CmdLineParserExceptionId = -46;
		static const int NULLCOWIntrusiveReferenceExceptionId = -47;
		static const int LogMessagePatternFormatterExceptionId = -48;
		static const int CryptographicRandomNumberExceptionId = -50;
		static const int SSLExceptionId = -51;
		static const int GenericRWLockImplExceptionId = -52;
		static const int FormatExceptionId = -53;

		static const int MD5ExceptionId = -1003;

		// new in BloCxx
		static const int IPCMutexExceptionId = -5001;
		static const int RegExCompileExceptionId = -5002;
		static const int RegExExecuteExceptionId = -5003;
	}
}

#endif

