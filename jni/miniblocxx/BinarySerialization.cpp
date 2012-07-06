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

#ifndef BLOCXX_NO_IOSTREAMS

#include "miniblocxx/BinarySerialization.hpp"
#include "miniblocxx/AutoPtr.hpp"
#include "miniblocxx/ByteSwap.hpp"
#include "miniblocxx/IOException.hpp"
#include "miniblocxx/Format.hpp"
#include <cerrno>
#include <iostream>

namespace BLOCXX_NAMESPACE
{

namespace BinarySerialization
{
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
writeLen(std::streambuf & ostrm, UInt32 len)
{
	// This is ASN.1 length encoding
	/*
	 * short len if it's less than 128 - one byte giving the len,
	 * with bit 8 0.
	 */
	if (len < 128)
	{
		UInt8 length_byte = static_cast<UInt8>(len);
		write(ostrm, &length_byte, 1);
		return;
	}
	/*
	 * long len otherwise - one byte with bit 8 set, giving the
	 * length of the length, followed by the length itself.
	 */
	/* find the first non-all-zero byte */
	UInt8 lenlen;
	if (len < (1 << 8))
	{
		lenlen = 1;
	}
	else if (len < (1 << 16))
	{
		lenlen = 2;
	}
	else if (len < (1 << 24))
	{
		lenlen = 3;
	}
	else
	{
		lenlen = 4;
	}
	UInt8 netlenlen = lenlen | 0x80UL;
	/* write the length of the length */
	write(ostrm, &netlenlen, 1);
	UInt8 netlen[sizeof(len)];
	for (int j = 0; j < lenlen; j++)
	{
		netlen[(sizeof(len)-1) - j] = static_cast<UInt8>(len & 0xffU);
		len >>= 8;
	}
	/* write the length itself */
	write(ostrm, static_cast<void *>(&netlen[sizeof(len)-lenlen]), lenlen);
}
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
readLen(std::streambuf & istrm, UInt32 & len)
{
	// This is ASN.1 length encoding
	UInt8 lc;
	read(istrm, lc);
	if (lc & 0x80U) 
	{
		UInt8 noctets = lc & 0x7fU;
		if ( noctets > sizeof(len) ) 
		{
			BLOCXX_THROW(IOException, Format("Failed reading data: length length (%1) is too large (> %2)", static_cast<int>(noctets), sizeof(len)).c_str());
		}
		UInt8 netlen[sizeof(len)];
		read(istrm, static_cast<void *>(netlen), noctets);
		len = 0;
		for (int i = 0; i < noctets; i++ ) 
		{
			len <<= 8;
			len |= netlen[i];
		}
	} 
	else 
	{
		len = lc;
	}
}
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
write(std::streambuf & ostrm, void const * dataOut, size_t dataOutLen)
{
	std::streamsize cnt = dataOutLen;
	if (ostrm.sputn(static_cast<char const *>(dataOut), cnt) != cnt)
	{
		BLOCXX_THROW_ERRNO_MSG(IOException, "Failed writing data");
	}
}
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
verifySignature(std::streambuf & istrm, UInt8 validSig)
{
	UInt8 val;
	read(istrm, val);
	if (val != validSig)
	{
		BLOCXX_THROW(BadSignatureException,
			Format("Received invalid signature. Got: %1 Expected: %2", Int32(val),
				Int32(validSig)).c_str());
	}
}
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
writeStringArray(std::streambuf & ostrm, const StringArray * propertyList)
{
	bool nullPropertyList = (propertyList == 0);
	writeBool(ostrm, nullPropertyList);
	if (!nullPropertyList)
	{
		writeStringArray(ostrm, *propertyList);
	}
}
//////////////////////////////////////////////////////////////////////////////
// STATIC
void
read(std::streambuf & istrm, void * dataIn, size_t dataInLen)
{
	std::streamsize cnt = dataInLen;
#ifdef BLOCXX_WIN32
// VC10 warns that sgetn is unsafe.
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	if (istrm.sgetn(static_cast<char *>(dataIn), cnt) != cnt)
#ifdef BLOCXX_WIN32
#pragma warning(pop)
#endif
	{
		BLOCXX_THROW(IOException, "Failed reading data");
	}
}

} // namespace BinarySerialization

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_IOSTREAMS
