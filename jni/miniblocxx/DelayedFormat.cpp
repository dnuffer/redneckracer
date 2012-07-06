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
 * @author Kevin Harris
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/DelayedFormat.hpp"
#include "miniblocxx/Assertion.hpp"

namespace BLOCXX_NAMESPACE
{
	namespace DelayedFormatInternals
	{
		DelayedFormatReferenceBase::DelayedFormatReferenceBase()
		{
		}
		DelayedFormatReferenceBase::~DelayedFormatReferenceBase()
		{
		}
		std::ostream& DelayedFormatReferenceBase::dumpToStream(std::ostream& o) const
		{
			return doDumpToStream(o);
		}
		std::ostream& operator<<(std::ostream& o, const DelayedFormatReferenceBase& s)
		{
			return s.dumpToStream(o);
		}
	}

	DelayedFormat::DelayedFormat(const String& format)
		: formatString(format), formatParameters()
	{
	}

	String DelayedFormat::toString() const
	{
		return format().toString();
	}

	DelayedFormat::operator String() const
	{
		return toString();
	}

	Format DelayedFormat::format() const
	{
		return formatWithString(formatString.c_str());
	}

	Format DelayedFormat::formatWithString(const String& fs) const
	{
		return formatWithString(fs.c_str());
	}

	Format DelayedFormat::formatWithString(const char* fsp) const
	{
		// The constructors allow up to 9 (inclusive) parameters.
		BLOCXX_ASSERT( formatParameters.size() <= 9 );
		const Array<Reference<DelayedFormatInternals::DelayedFormatReferenceBase> >& fp = formatParameters;
		switch( formatParameters.size() )
		{
		case 0:
			return Format(fsp, "");
		case 1:
			return Format(fsp, *fp[0]);
		case 2:
			return Format(fsp, *fp[0], *fp[1]);
		case 3:
			return Format(fsp, *fp[0], *fp[1], *fp[2]);
		case 4:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3]);
		case 5:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3], *fp[4]);
		case 6:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3], *fp[4], *fp[5]);
		case 7:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3], *fp[4], *fp[5], *fp[6]);
		case 8:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3], *fp[4], *fp[5], *fp[6], *fp[7]);
		case 9:
			return Format(fsp, *fp[0], *fp[1], *fp[2], *fp[3], *fp[4], *fp[5], *fp[6], *fp[7], *fp[8]);
		}
		// Never reached.
		return Format("","");
	}


} // end namespace BLOCXX_NAMESPACE
