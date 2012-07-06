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

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/DateTime.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/TimeUtils.hpp"

#include <math.h>

namespace BLOCXX_NAMESPACE
{
	namespace
	{
		// Macros for these functions are suggested in the autoconf documentation
		// on "Portability of C functions", and were added in the C99 standard.

		// The volatile keyword is here to prevent optimization on the variable.
		inline bool isNAN(volatile double d)
		{
			// NAN isn't comparable to anything, including itself.
			return d != d;
		}

		inline bool isINF(volatile double d)
		{
			// INF - INF == NAN
			return !isNAN(d) && isNAN(d - d);
		}
	}

	namespace Time
	{
		namespace
		{
			const Int64 MICROSECONDS_PER_SECOND = 1000000;
			const Int64 MICROSECONDS_PER_MINUTE = 60 * MICROSECONDS_PER_SECOND;
			const Int64 MICROSECONDS_PER_HOUR = 60 * MICROSECONDS_PER_MINUTE;
			const Int64 MICROSECONDS_PER_DAY = 24 * MICROSECONDS_PER_HOUR;
		}

		TimeDuration::TimeDuration()
			: m_microseconds(0)
			, m_special(E_TIME_NOT_SPECIAL)
		{
		}

		TimeDuration::TimeDuration(ESpecialTime special)
			: m_microseconds(0)
			, m_special(special)
		{
		}

		TimeDuration::TimeDuration(Int64 microseconds)
			: m_microseconds(microseconds)
			, m_special(E_TIME_NOT_SPECIAL)
		{
		}

		TimeDuration::TimeDuration(double seconds)
			: m_microseconds(0)
		{
			if( isNAN(seconds) )
			{
				m_special = E_TIME_NADT;
			}
			else if( isINF(seconds) && seconds > 0 )
			{
				m_special = E_TIME_POS_INFINITY;
			}
			else if( isINF(seconds) && seconds < 0 )
			{
				m_special = E_TIME_NEG_INFINITY;
			}
			else
			{
				m_special = E_TIME_NOT_SPECIAL;
				// Round to the nearest microsecond.
				m_microseconds = static_cast<Int64>(MICROSECONDS_PER_SECOND * seconds + 0.5);
			}
		}

		TimeDuration::TimeDuration(const TimeDuration& td)
			: m_microseconds(td.m_microseconds)
			, m_special(td.m_special)
		{
		}

		TimeDuration::~TimeDuration()
		{
		}

		TimeDuration& TimeDuration::operator=(const TimeDuration& td)
		{
			m_microseconds = td.m_microseconds;
			m_special = td.m_special;

			return *this;
		}

		Int32 TimeDuration::relativeDays() const
		{
			if( m_microseconds < 0 )
			{
				// Days can be negative, but everything else is positive.  Borrow one.
				return completeDays() - 1;
			}
			return completeDays();
		}

		Int32 TimeDuration::hourInDay() const
		{
			if( m_microseconds < 0 )
			{
				return 24 - ((-completeHours()) % 24 + 1);
			}
			return completeHours() % 24;
		}

		Int32 TimeDuration::minuteInHour() const
		{
			if( m_microseconds < 0 )
			{
				return 60 - ((-completeMinutes()) % 60 + 1);
			}
			return completeMinutes() % 60;
		}

		Int32 TimeDuration::secondInMinute() const
		{
			if( m_microseconds < 0 )
			{
				return 60 - ((-completeSeconds()) % 60 + 1);
			}
			return completeSeconds() % 60;
		}

		Int32 TimeDuration::microsecondInSecond() const
		{
			if ( m_microseconds < 0 )
			{
				return MICROSECONDS_PER_SECOND - ((-m_microseconds) % MICROSECONDS_PER_SECOND);
			}
			return m_microseconds % 1000000;
		}

		String TimeDuration::toString() const
		{
			return toString("%6:%3:%5");
		}

		namespace
		{
			String padOnLeft(Int64 val, size_t width)
			{

				if( val < 0 )
				{
					return "-" + padOnLeft(Format("%1", -val), width - 1, '0');
				}
				return padOnLeft(Format("%1", val), width, '0');
			}
		}

		String TimeDuration::toString(const char* formatStr) const
		{
			if( m_special == E_TIME_NOT_SPECIAL )
			{
				/// @todo Make this more efficient when Format() accepts some form of width specifiers.
				return Format(formatStr,
					relativeDays(), // 1
					padOnLeft(hourInDay(), 2), // 2
					padOnLeft(minuteInHour(), 2), // 3
					padOnLeft(secondInMinute(), 2), // 4
					padOnLeft(secondInMinute(), 2) + "." + padOnLeft(microsecondInSecond(), 6), // 5
					completeHours() + ((m_microseconds < 0)?(-1):(0)), // 6
					completeMinutes(), // 7
					completeSeconds(), // 8
					microseconds()); // 9
			}
			else
			{
				return getDescriptionForFlag(m_special);
			}
		}

		bool operator<(const TimeDuration& td1, const TimeDuration& td2)
		{
			ESpecialTime flag1 = td1.getSpecialFlag();
			ESpecialTime flag2 = td2.getSpecialFlag();

			if( flag1 == E_TIME_NADT || flag2 == E_TIME_NADT )
			{
				return false;
			}

			if( flag1 == flag2 )
			{
				return (td1.microseconds() < td2.microseconds());
			}

			return (flag1 < flag2);
		}

		bool operator>(const TimeDuration& td1, const TimeDuration& td2)
		{
			ESpecialTime flag1 = td1.getSpecialFlag();
			ESpecialTime flag2 = td2.getSpecialFlag();

			if( flag1 == E_TIME_NADT || flag2 == E_TIME_NADT )
			{
				return false;
			}

			if( flag1 == flag2 )
			{
				return (td1.microseconds() > td2.microseconds());
			}

			return (flag1 > flag2);
		}

		bool operator==(const TimeDuration& td1, const TimeDuration& td2)
		{
			return (td1.getSpecialFlag() == td2.getSpecialFlag()) &&
				(td1.microseconds() == td2.microseconds());
		}

		bool operator!=(const TimeDuration& td1, const TimeDuration& td2)
		{
			return !(td1 == td2);
		}

		bool operator>=(const TimeDuration& td1, const TimeDuration& td2)
		{
			return (td1 == td2) || (td1 > td2);
		}

		bool operator<=(const TimeDuration& td1, const TimeDuration& td2)
		{
			return (td1 == td2) || (td1 < td2);
		}


		TimeDuration operator*(const TimeDuration& td, int factor)
		{
			if( td.isSpecial() )
			{
				return td;
			}
			return TimeDuration(td.microseconds() * factor);
		}

		TimeDuration& operator*=(TimeDuration& td, int factor)
		{
			td = td * factor;
			return td;
		}

		TimeDuration operator*(int factor, const TimeDuration& td)
		{
			if( td.isSpecial() )
			{
				return td;
			}
			return TimeDuration(td.microseconds() * factor);
		}

		TimeDuration operator+(const TimeDuration& td1, const TimeDuration& td2)
		{
			ESpecialTime flag1 = td1.getSpecialFlag();
			ESpecialTime flag2 = td2.getSpecialFlag();

			if( flag1 == Time::E_TIME_NADT || flag2 == Time::E_TIME_NADT )
			{
				// nadt + (non-special|-inf|+inf|nadt) --> nadt
				// (non-special|-inf|+inf) + nadt --> nadt
				return TimeDuration(E_TIME_NADT);
			}
			else if( ( (flag1 == Time::E_TIME_POS_INFINITY) &&
				(flag2 == Time::E_TIME_NEG_INFINITY) ) ||
				( (flag1 == Time::E_TIME_NEG_INFINITY) &&
					(flag2 == Time::E_TIME_POS_INFINITY) ) )
			{
				// +inf + -inf --> 0
				// -inf + +inf --> 0
				return TimeDuration(Int64(0));
			}
			else if( flag1 != Time::E_TIME_NOT_SPECIAL )
			{
				// +inf + (non-special|+inf) --> +inf
				// -inf + (non-special|-inf) --> -inf
				return td1;
			}
			else if( flag2 != Time::E_TIME_NOT_SPECIAL )
			{
				// non-special + +inf --> +inf
				// non-special + -inf --> -inf
				return td2;
			}

			return TimeDuration(td1.microseconds() + td2.microseconds());
		}

		TimeDuration& operator+=(TimeDuration& td1, const TimeDuration& td2)
		{
			td1 = td1 + td2;
			return td1;
		}

		TimeDuration operator-(const TimeDuration& td1, const TimeDuration& td2)
		{
			return td1 + (-td2);
		}

		TimeDuration& operator-=(TimeDuration& td1, const TimeDuration& td2)
		{
			td1 = td1 - td2;
			return td1;
		}

		TimeDuration operator-(const TimeDuration& td1)
		{
			ESpecialTime flag = td1.getSpecialFlag();
			switch(flag)
			{
			case E_TIME_NOT_SPECIAL:
				return TimeDuration(-td1.microseconds());
			case E_TIME_NEG_INFINITY:
				return TimeDuration(E_TIME_POS_INFINITY);
			case E_TIME_POS_INFINITY:
				return TimeDuration(E_TIME_NEG_INFINITY);
			case E_TIME_NADT:
				return td1;
			}
			// Never reached.
			return TimeDuration(E_TIME_NADT);
		}

		TimeDuration days(Int64 count)
		{
			return TimeDuration(count * MICROSECONDS_PER_DAY);
		}

		TimeDuration hours(Int64 count)
		{
			return TimeDuration(count * MICROSECONDS_PER_HOUR);
		}

		TimeDuration minutes(Int64 count)
		{
			return TimeDuration(count * MICROSECONDS_PER_MINUTE);
		}

		TimeDuration seconds(Int64 count)
		{
			return TimeDuration(count * MICROSECONDS_PER_SECOND);
		}

		TimeDuration microseconds(Int64 count)
		{
			return TimeDuration(count);
		}

	} // end namespace Time

} // end namespace BLOCXX_NAMESPACE
