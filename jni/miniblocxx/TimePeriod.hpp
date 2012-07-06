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

#ifndef BLOCXX_TIME_PERIOD_HPP_INCLUDE_GUARD_
#define BLOCXX_TIME_PERIOD_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/CommonFwd.hpp"
#include "miniblocxx/DateTime.hpp"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/stl/algorithm"

namespace BLOCXX_NAMESPACE
{
	namespace Time
	{
		/**
		 * A class representing a time period.
		 *
		 * The results of all operations on TimePeriods should follow from the
		 * following simple laws:
		 * - TimePeriod(a, b) represents the set of times t for which a <= t && t < b.
		 * - TimePeriod(dt, d), where d is a TimeDuration, is TimePeriod(dt, dt + d).
		 * - tp.contains(t) is true if and only if t is in the set of times tp.
		 * - tp.contains(tp') is true if and only if tp' is a subset of tp, that is,
		 *   every t in tp' is also in tp.
		 *
		 * Some consequences of these laws:
		 * - TimePeriod(a, b) is an empty interval if b <= a, and there is only
		 *   one such value: TimePeriod(a, b) == TimePeriod(a', b') whenever
		 *   b <= a and b' <= a'.
		 * - TimePeriod(a, b), where a or b is an invalid DateTime will produce a
		 *   TimePeriod with an invalid duration for all operations and fail to
		 *   contain anything except the invalid DateTime.
		 * - tp.contains(<the empty interval>) is true for all valid tp.
		*/
		class TimePeriod
		{
		public:
			TimePeriod();
			TimePeriod(const DateTime& start, const DateTime& finish);
			TimePeriod(const DateTime& start, const TimeDuration& length);
			TimePeriod(const TimePeriod& tp);
			~TimePeriod();
			TimePeriod& operator=(const TimePeriod& tp);

			DateTime begin() const { return m_start; }
			DateTime end() const { return m_finish; }

			TimeDuration length() const;
			bool empty() const;

			bool contains(const DateTime& dt) const;
			bool contains(const TimePeriod& tp) const;

			String toString() const;
			String toString(const String& format) const;

		private:
			DateTime m_start;
			DateTime m_finish;
		};

		bool operator==(const TimePeriod& tp1, const TimePeriod& tp2);
		bool operator!=(const TimePeriod& tp1, const TimePeriod& tp2);

		/**
		 * Return if the two time periods overlap.
		 */
		bool intersect(const TimePeriod& tp1, const TimePeriod& tp2);

		/**
		 * Return the overlap period between the two time periods.
		 */
		TimePeriod intersection(const TimePeriod& tp1, const TimePeriod& tp2);

		/**
		 * Return the union of the two time periods if they intersect (endpoints
		 * inclusive), otherwise an invalid time period is returned.
		 */
		TimePeriod merge(const TimePeriod& tp1, const TimePeriod& tp2);

		/**
		 * Return the union of everything between the start and finish
		 * of the two time periods.
		 */
		TimePeriod span(const TimePeriod& tp1, const TimePeriod& tp2);
	} // end namespace Time

} // end namespace BLOCXX_NAMESPACE

#endif
