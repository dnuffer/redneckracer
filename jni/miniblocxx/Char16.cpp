/*******************************************************************************
* Copyright (C) 2005, 2010, Quest Software, Inc. All rights reserved.
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
#include "miniblocxx/Char16.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/UTF8Utils.hpp"

#ifndef BLOCXX_NO_IOSTREAMS
#include "miniblocxx/ByteSwap.hpp"
#include "miniblocxx/BinarySerialization.hpp"

#include <cstdio>
#include <cstring>
#ifdef BLOCXX_HAVE_OSTREAM
#include <ostream>
#else
#include <iostream>
#endif
#endif // BLOCXX_NO_IOSTREAMS

namespace BLOCXX_NAMESPACE
{
//////////////////////////////////////////////////////////////////////////////
Char16::Char16(const String& x) :
	m_value(0)
{
	m_value = UTF8Utils::UTF8toUCS2(x.c_str());
}
//////////////////////////////////////////////////////////////////////////////
String
Char16::toString() const
{
	return UTF8Utils::UCS2toUTF8(m_value);
}

#ifndef BLOCXX_NO_IOSTREAMS
using std::ostream;
using std::streambuf;

//////////////////////////////////////////////////////////////////////////////
void
Char16::writeObject(streambuf & ostrm) const
{
	BinarySerialization::write(ostrm, m_value);
}
//////////////////////////////////////////////////////////////////////////////
void
Char16::readObject(streambuf & istrm)
{
	BinarySerialization::read(istrm, m_value);
}
//////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<< (std::ostream& ostrm, const Char16& arg)
{
	UInt16 val = arg.getValue();
	if (val > 0 && val <= 127)
	{
		ostrm << static_cast<char>(val);
	}
	else
	{
		// Print in hex format:
		char bfr[8];
		sprintf(bfr, "\\x%04X", val);
		ostrm << bfr;
	}
	return ostrm;
}
#endif // BLOCXX_NO_IOSTREAMS

} // end namespace BLOCXX_NAMESPACE

