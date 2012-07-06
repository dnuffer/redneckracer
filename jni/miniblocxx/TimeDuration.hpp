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

#ifndef BLOCXX_TIME_DURATION_HPP_INCLUDE_GUARD_
#define BLOCXX_TIME_DURATION_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/CommonFwd.hpp"
#include "miniblocxx/TimeConstants.hpp"

namespace BLOCXX_NAMESPACE
{
	namespace Time
	{
		/**
		 * A class representing a time duration.
		 *
		 * Any invalid times as set by the E_TIME_NADT special flag cannot be
		 * compared to any valid time durations.  These comparisons will always
		 * return false.
		 *
		 * Any infinite TimeDurations will remain infinite through any
		 * mathematical operations.
		 *
		 * Any mathematical operations where the number of microseconds would
		 * overflow an Int64 have undefined behavior and should not be attempted.
		 *
		 * All operations retrieving values (such as hourInDay(), microseconds(),
		 * etc) have undefined results on special TimeDurations.
		 */
		class TimeDuration
		{
		public:
			/**
			 * The default constructor creates a duration of 0.
			 */
			TimeDuration();
			explicit TimeDuration(ESpecialTime special);
			explicit TimeDuration(Int64 microseconds);
			explicit TimeDuration(double seconds);
			TimeDuration(const TimeDuration& td);
			~TimeDuration();
			TimeDuration& operator=(const TimeDuration& td);

			/**
			 * Return the relative number of days.  For positive TimeDurations,
			 * this value will be the same as completeDays().  If the TimeDuration
			 * is negative, this value is less than completeDays to ensure that
			 * the sum of the relative times is equal to the real number of
			 * microseconds.
			 *
			 * It is guaranteed that for all non-special time durations:
			 * microseconds() == (((((relativeDays()*24 + hourInDay())*60
			 *      + minuteInHour())*60 + secondInMinute())*1000000)
			 *    + microsecondInSecond() )
			 */
			Int32 relativeDays() const;
			/**
			 * Return the relative number of hours between 0 and 23 (inclusive).
			 * @see relativeDays() for a description of negative durations
			 */
			Int32 hourInDay() const;
			/**
			 * Return the relative number of minutes between 0 and 59 (inclusive).
			 * @see relativeDays() for a description of negative durations
			 */
			Int32 minuteInHour() const;
			/**
			 * Return the relative number of seconds between 0 and 59 (inclusive).
			 * @see relativeDays() for a description of negative durations
			 */
			Int32 secondInMinute() const;

			/**
			 * Return the relative number of seconds between 0 and 59 (inclusive).
			 * @see relativeDays() for a description of negative durations
			 */
			Int32 microsecondInSecond() const;

			Int64 completeDays() const { return completeHours() / 24; }
			Int64 completeHours() const { return completeMinutes() / 60; }
			Int64 completeMinutes() const { return completeSeconds() / 60; }
			Int64 completeSeconds() const { return microseconds() / 1000000; }
			Int64 microseconds() const { return m_microseconds; }

			bool isSpecial() const { return m_special != E_TIME_NOT_SPECIAL; }
			bool isInfinite() const { return (m_special == E_TIME_POS_INFINITY) || (m_special == E_TIME_NEG_INFINITY); }
			bool isInvalid() const { return m_special == E_TIME_NADT; }

			ESpecialTime getSpecialFlag() const { return m_special; }

			/**
			 * Get a floating-point number of seconds for this duration.
			 */
			double realSeconds() const { return microseconds() / double(1000000); }

			String toString() const;

			/**
			 * @param formatStr is a Format() specifier where:
			 *   %1=effective days
			 *   %2=hour in day
			 *   %3=minute in hour
			 *   %4=second in minute
			 *   %5=second in minute + fractional seconds (microsecond in second)
			 *   %6=complete hours (may be larger than %2)
			 *   %7=complete minutes (may be larger than %3)
			 *   %8=complete seconds (may be larger than %4)
			 *   %9=complete microseconds (may be larger than %5)
			 */
			String toString(const char* formatStr) const;

		private:
			Int64 m_microseconds;
			ESpecialTime m_special;
		};

		/**
		 * Compare two durations.
		 *
		 * @returns false if either td1 or td2 are invalid
		 * @returns false if both td1 and td2 are infinite
		 * @returns true if td2 is infinite and td1 is non-special
		 * @returns true if both are non-special and the microsecond comparison is true
		 * @returns false if none of the above apply.
		 */
		bool operator<(const TimeDuration& td1, const TimeDuration& td2);
		bool operator>(const TimeDuration& td1, const TimeDuration& td2);
		bool operator>=(const TimeDuration& td1, const TimeDuration& td2);
		bool operator<=(const TimeDuration& td1, const TimeDuration& td2);
		bool operator==(const TimeDuration& td1, const TimeDuration& td2);
		bool operator!=(const TimeDuration& td1, const TimeDuration& td2);

		TimeDuration operator*(const TimeDuration& td, int factor);
		TimeDuration& operator*=(TimeDuration& td, int factor);
		TimeDuration operator*(int factor, const TimeDuration& td);
		TimeDuration operator+(const TimeDuration& td1, const TimeDuration& td2);
		TimeDuration& operator+=(TimeDuration& td1, const TimeDuration& td2);
		TimeDuration operator-(const TimeDuration& td1, const TimeDuration& td2);
		TimeDuration& operator-=(TimeDuration& td1, const TimeDuration& td2);
		TimeDuration operator-(const TimeDuration& td1);

		/**
		 * Return a TimeDuration for the specified number of days.
		 */
		TimeDuration days(Int64 count);
		/**
		 * Return a TimeDuration for the specified number of hours.
		 */
		TimeDuration hours(Int64 count);
		/**
		 * Return a TimeDuration for the specified number of minutes.
		 */
		TimeDuration minutes(Int64 count);
		/**
		 * Return a TimeDuration for the specified number of seconds.
		 */
		TimeDuration seconds(Int64 count);

		/**
		 * Return a TimeDuration for the specified number of microseconds.
		 */
		TimeDuration microseconds(Int64 count);

	} // end namespace Time

} // end namespace BLOCXX_NAMESPACE

#endif
