#ifndef BLOCXX_TM_UTILS_HPP_INCLUDE_GUARD_
#define BLOCXX_TM_UTILS_HPP_INCLUDE_GUARD_
/*******************************************************************************
* Copyright (C) 2008, Quest Software, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Quest Software, Inc.,
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
 * @author Kevin S. Van Horn
 */

#include "miniblocxx/BLOCXX_config.h"

#include <time.h>

namespace BLOCXX_NAMESPACE
{
	// PURPOSE: Normalize a struct tm, for those platforms that lack the timegm
	// function or for which mktime doesn't normalize its argument.
	//
	// NOTE: Does not modify tm_wday, tm_yday, or tm_isdst.  Negative numbers
	// are OK.
	//
	// REQUIRE: the normalization will not cause integer overflow/underflow.
	//
	void normTm(struct tm & tm);

	// PURPOSE: Replacement for platforms that lack timegm.
	// RETURNS: the time_t value corresponding to tm interpreted as UTC time.
	// PROMISE: normalizes tm first (see normTm).
	// PROMISE: If normalized time is outside the valid range defined by
	// BLOCXX_DATETIME_MINIMUM_YEAR and BLOCXX_DATETIME_MAXIMUM_YEAR,
	// throws DateTimeException.
	// REQUIRE: any normalization will not cause integer flow.
	//
	time_t timeGm(struct tm & tm);
} // end namespace BLOCXX_NAMESPACE

#endif
