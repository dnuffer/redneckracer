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

#ifndef BLOCXX_BYTE_SWAP_HPP_INCLUDE_GUARD_
#define BLOCXX_BYTE_SWAP_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"

#if !defined(BLOCXX_WORDS_BIGENDIAN) && defined(BLOCXX_HAVE_BYTESWAP_H) && !defined(BLOCXX_DEBUG_MEMORY)
#include <byteswap.h>
#endif

namespace BLOCXX_NAMESPACE
{

// this will be defined by the configure script.
#ifndef BLOCXX_WORDS_BIGENDIAN
#if defined(BLOCXX_HAVE_BYTESWAP_H) && !defined(BLOCXX_DEBUG_MEMORY)

/**
 * Convert a UInt16 value to network byte order.
 * @param v The UInt16 value to convert
 * @return The value of v in network byte order.
 */
inline UInt16 hton16(UInt16 v) { return __bswap_16(v); }
/**
 * Convert a UInt32 value to network byte order.
 * @param v The UInt32 value to convert
 * @return The value of v in network byte order.
 */
inline UInt32 hton32(UInt32 v) { return __bswap_32(v); }
/**
 * Convert a UInt64 value to network byte order.
 * @param v The UInt64 value to convert
 * @return The value of v in network byte order.
 */
inline UInt64 hton64(UInt64 v) { return __bswap_64(v); }
/**
 * Convert a UInt16 value from network byte order to host byte order.
 * @param v The UInt16 value to convert
 * @return The value of v in host byte order.
 */
inline UInt16 ntoh16(UInt16 v) { return __bswap_16(v); }
/**
 * Convert a UInt32 value from network byte order to host byte order.
 * @param v The UInt32 value to convert
 * @return The value of v in host byte order.
 */
inline UInt32 ntoh32(UInt32 v) { return __bswap_32(v); }
/**
 * Convert a UInt64 value from network byte order to host byte order.
 * @param v The UInt64 value to convert
 * @return The value of v in host byte order.
 */
inline UInt64 ntoh64(UInt64 v) { return __bswap_64(v); }
#else
inline UInt16 hton16(UInt16 v)
{
	UInt16 rval;
	(reinterpret_cast<unsigned char*>(&rval))[1] = (reinterpret_cast<unsigned char*>(&v))[0];
	(reinterpret_cast<unsigned char*>(&rval))[0] = (reinterpret_cast<unsigned char*>(&v))[1];
	return rval;
}
inline UInt32 hton32(UInt32 v)
{
	UInt32 rval;
	(reinterpret_cast<unsigned char*>(&rval))[3] = (reinterpret_cast<unsigned char*>(&v))[0];
	(reinterpret_cast<unsigned char*>(&rval))[2] = (reinterpret_cast<unsigned char*>(&v))[1];
	(reinterpret_cast<unsigned char*>(&rval))[1] = (reinterpret_cast<unsigned char*>(&v))[2];
	(reinterpret_cast<unsigned char*>(&rval))[0] = (reinterpret_cast<unsigned char*>(&v))[3];
	return rval;
}
inline UInt64 hton64(UInt64 v)
{
	UInt64 rval;
	(reinterpret_cast<unsigned char*>(&rval))[7] = (reinterpret_cast<unsigned char*>(&v))[0];
	(reinterpret_cast<unsigned char*>(&rval))[6] = (reinterpret_cast<unsigned char*>(&v))[1];
	(reinterpret_cast<unsigned char*>(&rval))[5] = (reinterpret_cast<unsigned char*>(&v))[2];
	(reinterpret_cast<unsigned char*>(&rval))[4] = (reinterpret_cast<unsigned char*>(&v))[3];
	(reinterpret_cast<unsigned char*>(&rval))[3] = (reinterpret_cast<unsigned char*>(&v))[4];
	(reinterpret_cast<unsigned char*>(&rval))[2] = (reinterpret_cast<unsigned char*>(&v))[5];
	(reinterpret_cast<unsigned char*>(&rval))[1] = (reinterpret_cast<unsigned char*>(&v))[6];
	(reinterpret_cast<unsigned char*>(&rval))[0] = (reinterpret_cast<unsigned char*>(&v))[7];
	return rval;
}
inline UInt16 ntoh16(UInt16 v)
{
	return hton16(v);
}
inline UInt32 ntoh32(UInt32 v)
{
	return hton32(v);
}
inline UInt64 ntoh64(UInt64 v)
{
	return hton64(v);
}
#endif
#else // we're big-endian, just pass-thru
inline UInt16 hton16(UInt16 v) { return v; }
inline UInt32 hton32(UInt32 v) { return v; }
inline UInt64 hton64(UInt64 v) { return v; }
inline UInt16 ntoh16(UInt16 v) { return v; }
inline UInt32 ntoh32(UInt32 v) { return v; }
inline UInt64 ntoh64(UInt64 v) { return v; }
#endif

} // end namespace BLOCXX_NAMESPACE

#endif
