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
*     * Neither the name of
*       Quest Software, Inc.,
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

#ifndef BLOCXX_TIME_UTILS_HPP_INCLUDE_GUARD_
#define BLOCXX_TIME_UTILS_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/DateTime.hpp"
#include "miniblocxx/TimePeriod.hpp"
#include "miniblocxx/TimeDuration.hpp"

namespace BLOCXX_NAMESPACE
{
	namespace Time
	{
		/**
		 * Return if the given TimeDuration is invalid.
		 */
		inline bool isInvalid(const Time::TimeDuration& td)
		{
			return td.isInvalid();
		}

		/**
		 * Return if the given TimeDuration is infinite.
		 */
		inline bool isInfinite(const Time::TimeDuration& td)
		{
			return td.isInfinite();
		}


		/**
		 * Return if the given TimeDuration is a special TimeDuration.
		 */
		inline bool isSpecial(const Time::TimeDuration& td)
		{
			return td.isSpecial();
		}


		/**
		 * Return if the given TimePeriod is invalid.
		 */
		inline bool isInvalid(const TimePeriod& p)
		{
			if( isInvalid(p.begin()) )
			{
				return true;
			}
			else if( isInvalid(p.end()) )
			{
				return true;
			}
			else if( isPosInfinity(p.begin()) )
			{
				return true;
			}
			return false;
		}

		/**
		 * Return if the given TimePeriod is infinite in duration.
		 */
		inline bool isInfinite(const TimePeriod& p)
		{
			return timeBetween(p.begin(), p.end()).isInfinite();
		}

		inline DateTime infiniteTime()
		{
			return DateTime::getPosInfinity();
		}

		/**
		 * Return an infinite time period starting at the supplied
		 * DateTime.  The default will start at the epoch and end at the
		 * infinite time representation.
		 */
		inline TimePeriod infiniteTimePeriod(const DateTime& start)
		{
			return TimePeriod(start, infiniteTime());
		}


		inline DateTime invalidTime()
		{
			return DateTime(E_TIME_NADT);
		}

		/**
		 * Return an invalid time period.  This is not guaranteed to be
		 * the only invalid representation.
		 */
		inline TimePeriod invalidTimePeriod()
		{
			return TimePeriod(invalidTime(), invalidTime());
		}
	} // end namespace Time

} // end namespace BLOCXX_NAMESPACE

#endif

