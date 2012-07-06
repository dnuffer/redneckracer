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

#ifndef BLOCXX_BINARY_SERIALIZATION_HPP_INCLUDE_GUARD_
#define BLOCXX_BINARY_SERIALIZATION_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

#ifndef BLOCXX_NO_IOSTREAMS

#include "miniblocxx/Types.hpp"
#include "miniblocxx/Bool.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/ByteSwap.hpp"
#include "miniblocxx/IOException.hpp"

#include <iosfwd>

// The classes and functions defined in this file are not meant for general
// use, they are internal implementation details.  They may change at any time.

namespace BLOCXX_NAMESPACE
{

namespace BinarySerialization
{

// This should generally be kept in sync with the repository version in BLOCXX_HDBCommon.hpp
// The general idea is to have it be a concatenation of release numbers with
// a revision on the end (to prevent problems during development)
// So 3000003 originated from version 3.0.0 rev 4
//
// 10/25/2003 - 3000005. Changed enumClassNames to send over an enum of strings
//   instead of object paths.
// 7/22/2004 - 3000008. Changed signatures and added versioning. Now all
//   readObject() calls will be able to read older versions as well as the
//   current.  Introduced MinBinaryProtocolVersion which is the oldest version
//   we can sucessfully read.
// 9/01/2005 - only HDB version to 4000000. Changed key format to use : instead of / to fix a bug.
// 10/12/2005 - only HDB version to 4000001. Fixed association and instance key format wrt associations.
const UInt32 BinaryProtocolVersion = 4000002;

// This is the oldest version the code can handle.
const UInt32 MinBinaryProtocolVersion = 3000007;

// These values are all used by the binary protocol
const UInt8 BIN_OK =				0;		// Success returned from server
const UInt8 BIN_ERROR =			1;		// Error returned from server
const UInt8 BIN_EXCEPTION =		2;		// CIM Exception returned from server
const UInt8 BIN_END =			3;	// Final sentinel

const UInt8 BIN_LOG_MESSAGE =		45;		// log message


const UInt8 BINSIG_BOOL =			104;
const UInt8 BINSIG_STR =			106;
const UInt8 BINSIG_STRARRAY =		107;

const UInt8 BINSIG_STRINGENUM =	115;

const UInt8 END_STRINGENUM =		154;


	BLOCXX_COMMON_API void verifySignature(std::streambuf & istrm, UInt8 validSig);

//////////////////////////////////////////////////////////////////////////////
	template <typename Handler, typename ReaderFunc>
	inline void readEnum(
			std::streambuf & istrm, Handler & result,
			const ReaderFunc & read, const UInt8 beginsig, const UInt8 endsig)
	{
		verifySignature(istrm, beginsig);
		bool done = false;
		while (!done)
		{
			try
			{
				result.handle(read(istrm));
			}
			catch (const BadSignatureException& e)
			{
				// read threw because we've read all the objects
				verifySignature(istrm, endsig);
				done = true;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////////
	BLOCXX_COMMON_API void write(
		std::streambuf & ostrm, const void * dataOut, size_t dataOutLen
	);

	inline void write(std::streambuf & ostrm, Int32 val)
	{
		val = hton32(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, UInt32 val)
	{
		val = hton32(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	BLOCXX_COMMON_API void writeLen(std::streambuf & ostrm, UInt32 len);

	inline void write(std::streambuf & ostrm, UInt8 val)
	{
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, UInt16 val)
	{
		val = hton16(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, Int16 val)
	{
		val = hton16(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, UInt64 val)
	{
		val = hton64(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, Int64 val)
	{
		val = hton64(val);
		BinarySerialization::write(ostrm, &val, sizeof(val));
	}

	inline void write(std::streambuf & ostrm, const String & str)
	{
		str.writeObject(ostrm);
	}

	inline void writeBool(std::streambuf & ostrm, Bool arg)
	{
		BinarySerialization::write(ostrm, BINSIG_BOOL);
		arg.writeObject(ostrm);
	}

	inline void writeString(std::streambuf & ostrm, const String & str)
	{
		BinarySerialization::write(ostrm, BINSIG_STR);
		str.writeObject(ostrm);
	}

	BLOCXX_COMMON_API void readLen(std::streambuf & istrm, UInt32 & len);

	/////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline void
	readArray(std::streambuf & istr, T & a)
	{
		a.clear();
		UInt32 len;
		BinarySerialization::readLen(istr, len);

		a.reserve(len);
		for (UInt32 i = 0; i < len; i++)
		{
			typename T::value_type x;
			x.readObject(istr);
			a.push_back(x);
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	template <typename T>
	inline void
	writeArray(std::streambuf & ostrm, const T & a)
	{
		UInt32 len = static_cast<UInt32>(a.size());
		BinarySerialization::writeLen(ostrm, len);
		for (UInt32 i = 0; i < len; i++)
		{
			a.operator[](i).writeObject(ostrm);
		}
	}

	inline void writeStringArray(
		std::streambuf & ostrm, const StringArray & stra
	)
	{
		BinarySerialization::write(ostrm, BINSIG_STRARRAY);
		writeArray(ostrm, stra);
	}

	BLOCXX_COMMON_API void writeStringArray(
		std::streambuf & ostrm, const StringArray * propertyList
	);


	BLOCXX_COMMON_API void read(
		std::streambuf & istrm, void * dataIn, size_t dataInLen
	);

	inline void read(std::streambuf & istrm, String & arg)
	{
		arg.readObject(istrm);
	}

	inline void read(std::streambuf & istrm, UInt64 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh64(val);
	}

	inline void read(std::streambuf & istrm, Int64 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh64(val);
	}

	inline void read(std::streambuf & istrm, Int32 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh32(val);
	}

	inline void read(std::streambuf & istrm, UInt32 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh32(val);
	}

	inline void read(std::streambuf & istrm, UInt16 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh16(val);
	}

	inline void read(std::streambuf & istrm, Int16 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
		val = ntoh16(val);
	}

	inline void read(std::streambuf & istrm, UInt8 & val)
	{
		BinarySerialization::read(istrm, &val, sizeof(val));
	}

	inline Bool readBool(std::streambuf & istrm)
	{
		BinarySerialization::verifySignature(istrm, BINSIG_BOOL);
		Bool b;
		b.readObject(istrm);
		return b;
	}

	inline String readString(std::streambuf & istrm)
	{
		BinarySerialization::verifySignature(istrm, BINSIG_STR);
		String rv;
		rv.readObject(istrm);
		return rv;
	}

	inline StringArray readStringArray(std::streambuf & istrm)
	{
		BinarySerialization::verifySignature(istrm, BINSIG_STRARRAY);
		StringArray stra;
		readArray(istrm, stra);
		return stra;
	}

} // end namespace BinarySerialization

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_IOSTREAMS
#endif	// BLOCXX_BinarySerialization_HPP_
