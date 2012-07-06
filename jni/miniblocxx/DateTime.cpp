/*******************************************************************************
* Copyright (C) 2008, Quest Software, Inc. All rights reserved.
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
 * @author Kevin Harris
 * @author Kevin S. Van Horn
 */

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/DateTime.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/Mutex.hpp"
#include "miniblocxx/MutexLock.hpp"
#include "miniblocxx/DivRem.hpp"
#include "miniblocxx/TmUtils.hpp"

#include <time.h>
#ifdef BLOCXX_HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

//#include <cctype>

#include "miniblocxx/stl/limits"
//#include <numeric>
#include <errno.h>

using namespace blocxx;

#undef min

namespace
{
	Int32 const MICROSECONDS_PER_SECOND = 1000000;

	// Representation of special DateTime values.  Note that a special
	// DateTime can be identified by looking at only m_microseconds (usec),
	// which will be a value outside of the valid range [0, 1000000)
	// for a normal DateTime.

	// +infinity is the largest value under the (sec, usec) ordering
	time_t const POS_INF_SEC = std::numeric_limits<time_t>::max();
	Int32 const POS_INF_USEC = std::numeric_limits<Int32>::max();

	// -infinity is the smallest value under the (sec, usec) ordering
	time_t const NEG_INF_SEC = std::numeric_limits<time_t>::min();
	Int32 const NEG_INF_USEC = std::numeric_limits<Int32>::min();

	// NADT is outside of the ordering.  Any fixed value for sec will do,
	// and any fixed unused usec value >= 1000000 will do.
	time_t const NADT_SEC = 0;
	Int32 const NADT_USEC = MICROSECONDS_PER_SECOND;

	// INVARIANT: For any DateTime value, either
	// - m_seconds == POS_INF_SEC && m_microseconds == POS_INF_USEC
	//   (+infinity),
	// - m_seconds == NEG_INF_SEC && m_microseconds == NEG_INF_USEC
	//   (-infinity),
	// - m_seconds == NADT_SEC && m_microseconds == NADT_USEC
	//   (not-a-datetime), or
	// - BLOCXX_DATETIME_MINIMUM_TIME <= m_seconds &&
	//   m_seconds <= BLOCXX_DATETIME_MAXIMUM_TIME &&
	//   0 <= m_microseconds && m_microseconds < MICROSECONDS_PER_SECOND
	//   (normal DateTime).
}

#ifndef BLOCXX_HAVE_LOCALTIME_R
namespace
{
	BLOCXX_NAMESPACE::Mutex localtimeMutex;
}
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
	BLOCXX_NAMESPACE::MutexLock lock(localtimeMutex);
	struct tm *p = localtime(timep);

	if (p)
	{
		*(result) = *p;
	}

	return p;
}
#endif

#ifndef BLOCXX_HAVE_GMTIME_R
namespace
{
	BLOCXX_NAMESPACE::Mutex gmtimeMutex;
}
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
	BLOCXX_NAMESPACE::MutexLock lock(gmtimeMutex);
	struct tm *p = gmtime(timep);

	if (p)
	{
		*(result) = *p;
	}

	return p;
}
#endif

#ifndef BLOCXX_HAVE_ASCTIME_R
namespace
{
	BLOCXX_NAMESPACE::Mutex asctimeMutex;
}
char *asctime_r(const struct tm *tm, char *result)
{
	BLOCXX_NAMESPACE::MutexLock lock(asctimeMutex);
	char *p = asctime(tm);

	if (p)
	{
		//asctime_r requires a buffer to be at least 26 chars in size
		::strncpy(result,p,25);
		result[25] = 0;
	}

	return result;
}
#endif

namespace BLOCXX_NAMESPACE
{
	namespace
	{
		inline bool isNADT(Int32 microsecond)
		{
			return microsecond == NADT_USEC;
		}

		inline bool isPosInf(Int32 microsecond)
		{
			return microsecond == POS_INF_USEC;
		}

		inline bool isNegInf(Int32 microsecond)
		{
			return microsecond == NEG_INF_USEC;
		}

		inline bool isInf(Int32 microsecond)
		{
			return isPosInf(microsecond) || isNegInf(microsecond);
		}

		inline bool haveSpecialValue(Int32 microsecond)
		{
			return microsecond < 0 || microsecond >= MICROSECONDS_PER_SECOND;
		}

		inline void checkNormalTime(Int32 microsecond)
		{
			if( haveSpecialValue(microsecond) )
			{
				BLOCXX_THROW(DateTimeException, "Special DateTime (infinite or NADT) used where a normal DateTime is required.");
			}
		}

		Time::ESpecialTime getSpecialTimeFlagImpl(Int32 microsecond)
		{
			return (
				microsecond < 0 ?
					Time::E_TIME_NEG_INFINITY :
				microsecond < MICROSECONDS_PER_SECOND ?
					Time::E_TIME_NOT_SPECIAL :
				microsecond > MICROSECONDS_PER_SECOND ?
					Time::E_TIME_POS_INFINITY :
				Time::E_TIME_NADT
			);
		}

		// Normalize the supplied time so the microseconds >= 0 and
		// less than MICROSECONDS_PER_SECOND
		void normalizeTime(time_t& seconds, Int32& microseconds)
		{
			time_t t1 = seconds;
			Int32 m1 = microseconds;
			t1 += divFloor(m1, MICROSECONDS_PER_SECOND);
			m1 = remFloor(m1, MICROSECONDS_PER_SECOND);
			// What about integer overflow / underflow?  It turns that any
			// integer wrap-around still results in an out-of-range value
			// for t1, because BLOCXX_DATETIME_MAXIMUM_TIME is sufficiently
			// smaller than the greatest time_t value (negative wrap-around
			// will always give a value higher than MAXIMUM_TIME), and
			// BLOCXX_DATETIME_MINIMUM_TIME is sufficiently greater than the
			// least time_t value (positive wrap-around will always give a
			// value less than MINIMUM_TIME).

			// Pre-cast the value to an Int64 to work around a compiler bug
			// (warning, which is treated as an error).
//			Int64 t1_64 = static_cast<Int64>(t1);
//			if( t1_64 < BLOCXX_DATETIME_MINIMUM_TIME ||
//				t1_64 > BLOCXX_DATETIME_MAXIMUM_TIME)
//			{
//				BLOCXX_THROW(DateTimeException, Format("Supplied time values are out of range: {%1,%2}", Int64(seconds), microseconds).c_str());
//			}

			seconds = t1;
			microseconds = m1;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	BLOCXX_DEFINE_EXCEPTION_WITH_ID(DateTime);

	//////////////////////////////////////////////////////////////////////////////
	Time::ESpecialTime DateTime::getSpecialTimeFlag() const
	{
		return getSpecialTimeFlagImpl(m_microseconds);
	}

	DateTime DateTime::getPosInfinity()
	{
		return DateTime(Time::E_TIME_POS_INFINITY);
	}

	DateTime DateTime::getNegInfinity()
	{
		return DateTime(Time::E_TIME_NEG_INFINITY);
	}

	DateTime DateTime::getNADT()
	{
		return DateTime(Time::E_TIME_NADT);
	}

//	DateTime DateTime::getMinimumTime()
//	{
//		return DateTime(time_t(BLOCXX_DATETIME_MINIMUM_TIME), 0);
//	}
//
//	DateTime DateTime::getMaximumTime()
//	{
//		return DateTime(
//			time_t(BLOCXX_DATETIME_MAXIMUM_TIME), MICROSECONDS_PER_SECOND - 1);
//	}

	DateTime::DateTime(Time::ESpecialTime timeValue)
	{
		switch(timeValue)
		{
		case Time::E_TIME_NOT_SPECIAL:
			m_seconds = 0;
			m_microseconds = 0;
			break;

		case Time::E_TIME_POS_INFINITY:
			m_seconds = POS_INF_SEC;
			m_microseconds = POS_INF_USEC;
			break;

		case Time::E_TIME_NEG_INFINITY:
			m_seconds = NEG_INF_SEC;
			m_microseconds = NEG_INF_USEC;
			break;

		case Time::E_TIME_NADT:
			m_seconds = NADT_SEC;
			m_microseconds = NADT_USEC;
			break;
		}
	}

#if 0
	//////////////////////////////////////////////////////////////////////////////
	namespace
	{
		inline void badDateTime(const String& str)
		{
			BLOCXX_THROW(DateTimeException, Format("Invalid DateTime: %1", str).c_str());
		}

#define VALIDATE_RANGE(value, min_value, max_value, str) \
		do \
		{ \
			if( ((value) < (min_value)) || ((value) > (max_value)) )	\
			{ \
				badDateTime(Format("Bad value \"%1\" should be within [%2,%3] in \"%4\". ", (value), (min_value), (max_value), (str))); \
			} \
		} \
		while(0)

#define VALIDATE_RANGES(year, month, day, hour, minute, second, microseconds, str) \
		do \
		{ \
			VALIDATE_RANGE((year), (BLOCXX_DATETIME_MINIMUM_YEAR), (BLOCXX_DATETIME_MAXIMUM_YEAR), (str)); \
			VALIDATE_RANGE((month), 1, 12, (str)); \
			VALIDATE_RANGE((day), 1, 31, (str)); \
			VALIDATE_RANGE((hour), 0, 23, (str)); \
			VALIDATE_RANGE((minute), 0, 59, (str)); \
			VALIDATE_RANGE((second), 0, 60, (str)); /* 60 to handle the occasional leap second. */ \
			VALIDATE_RANGE((microseconds), 0, (MICROSECONDS_PER_SECOND - 1), (str)); \
		} \
		while(0)

		inline bool isDOWValid(const char* str)
		{
			// a little FSM to validate the day of the week
			bool good = true;
			if (str[0] == 'S') // Sun, Sat
			{
				if (str[1] == 'u')
				{
					if (str[2] != 'n') // Sun
					{
						good = false;
					}
				}
				else if (str[1] ==  'a')
				{
					if (str[2] != 't') // Sat
					{
						good = false;
					}
				}
				else
				{
					good = false;
				}
			}
			else if (str[0] == 'M')	// Mon
			{
				if (str[1] == 'o')
				{
					if (str[2] != 'n')
					{
						good = false;
					}
				}
				else
				{
					good = false;
				}
			}
			else if (str[0] == 'T')	// Tue, Thu
			{
				if (str[1] == 'u')
				{
					if (str[2] != 'e') // Tue
					{
						good = false;
					}
				}
				else if (str[1] ==  'h')
				{
					if (str[2] != 'u') // Thu
					{
						good = false;
					}
				}
				else
				{
					good = false;
				}
			}
			else if (str[0] == 'W')	// Wed
			{
				if (str[1] == 'e')
				{
					if (str[2] != 'd')
					{
						good = false;
					}
				}
				else
				{
					good = false;
				}
			}
			else if (str[0] == 'F')	// Fri
			{
				if (str[1] == 'r')
				{
					if (str[2] != 'i')
					{
						good = false;
					}
				}
				else
				{
					good = false;
				}
			}
			else
			{
				good = false;
			}

			return good;
		}

		inline bool isLongDOWValid(const String& s)
		{
			if ( (s == "Sunday") ||
				(s == "Monday") ||
				(s == "Tuesday") ||
				(s == "Wednesday") ||
				(s == "Thursday") ||
				(s == "Friday") ||
				(s == "Saturday") )
			{
				return true;
			}
			return false;
		}

		// returns -1 if the month is invalid, 1-12 otherwise
		inline int decodeShortMonth(const char* str)
		{
			// a little FSM to calculate the month
			if (str[0] == 'J') // Jan, Jun, Jul
			{
				if (str[1] == 'a')
				{
					if (str[2] == 'n') // Jan
					{
						return 1;
					}
				}
				else if (str[1] ==  'u')
				{
					if (str[2] == 'n') // Jun
					{
						return 6;
					}
					else if (str[2] == 'l')	// Jul
					{
						return 7;
					}
				}
			}
			else if (str[0] == 'F')	// Feb
			{
				if (str[1] == 'e' && str[2] == 'b')
				{
					return 2;
				}
			}
			else if (str[0] == 'M')	// Mar, May
			{
				if (str[1] == 'a')
				{
					if (str[2] == 'r') // Mar
					{
						return 3;
					}
					else if (str[2] == 'y')	// May
					{
						return 5;
					}
				}
			}
			else if (str[0] == 'A')	// Apr, Aug
			{
				if (str[1] == 'p')
				{
					if (str[2] == 'r') // Apr
					{
						return 4;
					}
				}
				else if (str[1] == 'u')
				{
					if (str[2] == 'g') // Aug
					{
						return 8;
					}
				}
			}
			else if (str[0] == 'S')	// Sep
			{
				if (str[1] == 'e' && str[2] == 'p')
				{
					return 9;
				}
			}
			else if (str[0] == 'O')	// Oct
			{
				if (str[1] == 'c' && str[2] == 't')
				{
					return 10;
				}
			}
			else if (str[0] == 'N')	// Nov
			{
				if (str[1] == 'o' && str[2] == 'v')
				{
					return 11;
				}
			}
			else if (str[0] == 'D')	// Dec
			{
				if (str[1] == 'e' && str[2] == 'c')
				{
					return 12;
				}
			}

			return -1;
		}

		// returns -1 if the month is invalid, 1-12 otherwise
		inline int decodeLongMonth(const String& str)
		{
			if ( str.equals("January") )
			{
				return 1;
			}
			else if ( str.equals("February") )
			{
				return 2;
			}
			else if ( str.equals("March") )
			{
				return 3;
			}
			else if ( str.equals("April") )
			{
				return 4;
			}
			else if ( str.equals("May") )
			{
				return 5;
			}
			else if ( str.equals("June") )
			{
				return 6;
			}
			else if ( str.equals("July") )
			{
				return 7;
			}
			else if ( str.equals("August") )
			{
				return 8;
			}
			else if ( str.equals("September") )
			{
				return 9;
			}
			else if ( str.equals("October") )
			{
				return 10;
			}
			else if ( str.equals("November") )
			{
				return 11;
			}
			else if ( str.equals("December") )
			{
				return 12;
			}
			return -1;
		}

		// Get the timezone offset (from UTC) for the given timezone.  Valid results
		// are in the range -12 to 12, except for the case where LOCAL_TIME_OFFSET is
		// returned, in which case UTC should not be used.
		const int LOCAL_TIME_OFFSET = -24;
		bool getTimeZoneOffset(const String& timezone, int& offset)
		{
			int temp_offset = LOCAL_TIME_OFFSET -1;
			if ( timezone.length() == 1 )
			{
				// Single-letter abbrev.
				// This could be simplified into a couple of if statements with some
				// character math, but this should work for now.
				switch ( timezone[0] )
				{
				case 'Y': // Yankee   UTC-12
					temp_offset = -12;
					break;
				case 'X': // Xray     UTC-11
					temp_offset = -11;
					break;
				case 'W': // Whiskey  UTC-10
					temp_offset = -10;
					break;
				case 'V': // Victor   UTC-9
					temp_offset = -9;
					break;
				case 'U': // Uniform  UTC-8
					temp_offset = -8;
					break;
				case 'T': // Tango    UTC-7
					temp_offset = -7;
					break;
				case 'S': // Sierra   UTC-6
					temp_offset = -6;
					break;
				case 'R': // Romeo    UTC-5
					temp_offset = -5;
					break;
				case 'Q': // Quebec   UTC-4
					temp_offset = -4;
					break;
				case 'P': // Papa     UTC-3
					temp_offset = -3;
					break;
				case 'O': // Oscar    UTC-2
					temp_offset = -2;
					break;
				case 'N': // November UTC-1
					temp_offset = -1;
					break;
				case 'Z': // Zulu     UTC
					temp_offset = 0;
					break;
				case 'A': // Aplpha   UTC+1
					temp_offset = 1;
					break;
				case 'B': // Bravo    UTC+2
					temp_offset = 2;
					break;
				case 'C': // Charlie  UTC+3
					temp_offset = 3;
					break;
				case 'D': // Delta    UTC+4
					temp_offset = 4;
					break;
				case 'E': // Echo     UTC+5
					temp_offset = 5;
					break;
				case 'F': // Foxtrot  UTC+6
					temp_offset = 6;
					break;
				case 'G': // Golf     UTC+7
					temp_offset = 7;
					break;
				case 'H': // Hotel    UTC+8
					temp_offset = 8;
					break;
				case 'I': // India    UTC+9
					temp_offset = 9;
					break;
				case 'K': // Kilo     UTC+10
					temp_offset = 10;
					break;
				case 'L': // Lima     UTC+11
					temp_offset = 11;
					break;
				case 'M': // Mike     UTC+12
					temp_offset = 12;
					break;
				case 'J': // Juliet   Always local time
					temp_offset = LOCAL_TIME_OFFSET;
					break;
				default:
					break;
				}
			}
			else if ( timezone == "UTC" ) // Universal Time Coordinated, civil time
			{
				temp_offset = 0;
			}
			// European timezones
			else if ( timezone == "GMT" ) // Greenwich Mean Time   UTC
			{
				temp_offset = 0;
			}
			else if ( timezone == "BST" ) // British Summer Time   UTC+1
			{
				temp_offset = 1;
			}
			else if ( timezone == "IST" ) // Irish Summer Time     UTC+1
			{
				temp_offset = 1;
			}
			else if ( timezone == "WET" ) // Western Europe Time   UTC
			{
				temp_offset = 0;
			}
			else if ( timezone == "WEST" ) // Western Europe Summer Time   UTC+1
			{
				temp_offset = 1;
			}
			else if ( timezone == "CET" ) // Central Europe Time   UTC+1
			{
				temp_offset = 1;
			}
			else if ( timezone == "CEST" ) // Central Europe Summer Time   UTC+2
			{
				temp_offset = 2;
			}
			else if ( timezone == "EET" ) // Eastern Europe Time   UTC+2
			{
				temp_offset = 2;
			}
			else if ( timezone == "EEST" ) // Eastern Europe Summer Time   UTC+3
			{
				temp_offset = 3;
			}
			else if ( timezone == "MSK" ) // Moscow Time   UTC+3
			{
				temp_offset = 3;
			}
			else if ( timezone == "MSD" ) // Moscow Summer Time    UTC+4
			{
				temp_offset = 4;
			}
			// US and Canada
			else if ( timezone == "AST" ) // Atlantic Standard Time        UTC-4
			{
				temp_offset = -4;
			}
			else if ( timezone == "ADT" ) // Atlantic Daylight Saving Time UTC-3
			{
				temp_offset = -3;
			}
			else if ( timezone == "EST" ) // Eastern Standard Time         UTC-5
			{
				// CHECKME! This can also be Australian Eastern Standard Time UTC+10
				// (UTC+11 in Summer)
				temp_offset = -5;
			}
			else if ( timezone == "EDT" ) // Eastern Daylight Saving Time  UTC-4
			{
				temp_offset = -4;
			}
			else if ( timezone == "ET" ) // Eastern Time, either as EST or EDT
				// depending on place and time of year
			{
				// CHECKME! Assuming standard time.
				temp_offset = -5;
			}
			else if ( timezone == "CST" ) // Central Standard Time         UTC-6
			{
				// CHECKME! This can also be Australian Central Standard Time UTC+9.5
				temp_offset = -6;
			}
			else if ( timezone == "CDT" ) // Central Daylight Saving Time  UTC-5
			{
				temp_offset = -5;
			}
			else if ( timezone == "CT" ) // Central Time, either as CST or CDT
				// depending on place and time of year
			{
				// CHECKME! Assuming standard time.
				temp_offset = -6;
			}
			else if ( timezone == "MST" ) // Mountain Standard Time        UTC-7
			{
				temp_offset = -7;
			}
			else if ( timezone == "MDT" ) // Mountain Daylight Saving Time UTC-6
			{
				temp_offset = -6;
			}
			else if ( timezone == "MT" ) // Mountain Time, either as MST or MDT
				// depending on place and time of year
			{
				// CHECKME! Assuming standard time.
				temp_offset = -7;
			}
			else if ( timezone == "PST" ) // Pacific Standard Time         UTC-8
			{
				temp_offset = -8;
			}
			else if ( timezone == "PDT" ) // Pacific Daylight Saving Time  UTC-7
			{
				temp_offset = -7;
			}
			else if ( timezone == "PT" ) // Pacific Time, either as PST or PDT
				// depending on place and time of year
			{
				// CHECKME! Assuming standard time.
				temp_offset = -8;
			}
			else if ( timezone == "HST" ) // Hawaiian Standard Time        UTC-10
			{
				temp_offset = -10;
			}
			else if ( timezone == "AKST" ) // Alaska Standard Time         UTC-9
			{
				temp_offset = -9;
			}
			else if ( timezone == "AKDT" ) // Alaska Standard Daylight Saving Time UTC-8
			{
				temp_offset = -8;
			}
			// Australia
			else if ( timezone == "WST" ) // Western Standard Time         UTC+8
			{
				temp_offset = 8;
			}

			// Check the results of that huge mess.
			if ( temp_offset >= LOCAL_TIME_OFFSET )
			{
				offset = temp_offset;
				return true;
			}
			return false;
		}

		Int32 getDaysPerMonth(Int32 year, Int32 month)
		{
			const Int32 normal_days_per_month[12] =
				{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

			if ( (month >= 1) && (month <= 12) )
			{
				if ( month != 2 )
				{
					return normal_days_per_month[month - 1];
				}

				int leap_year_adjust = 0;

				if ( (year % 4) == 0 )
				{
					// Possibly a leap year.
					if ( (year % 100) == 0 )
					{
						if ( (year % 400) == 0 )
						{
							leap_year_adjust = 1;
						}
					}
					else
					{
						leap_year_adjust = 1;
					}
				}

				return normal_days_per_month[month - 1] + leap_year_adjust;
				// Check to see if it's a leap year.
			}
			return 0;
		}

		// Adjust the time given (year, month, day, hour) for the given timezone
		// offset.
		// Note: this is converting FROM local time to UTC, so the timezone offset is
		// subtracted instead of added.
		void adjustTimeForTimeZone(Int32 timezone_offset, Int32& year, Int32& month,
			Int32& day, Int32& hour)
		{
			if ( timezone_offset < 0 )
			{
				hour -= timezone_offset;

				if ( hour > 23 )
				{
					++day;
					hour -= 24;
				}
				// This assumes that the timezone will not shove a date by more than one day.
				if ( day > getDaysPerMonth(year, month) )
				{
					++month;
					day = 1;
				}
				if ( month > 12 )
				{
					month -= 12;
					++year;
				}
			}
			else if ( timezone_offset > 0 )
			{
				hour -= timezone_offset;

				if ( hour < 0 )
				{
					--day;
					hour += 24;
				}
				// This assumes that the timezone will not shove a date by more than one day.
				if ( day < 1 )
				{
					--month;
					day += getDaysPerMonth(year, month);
				}
				if ( month < 1 )
				{
					month += 12;
					--year;
				}
			}
		}


	} // end anonymous namespace

	//////////////////////////////////////////////////////////////////////////////
	DateTime::DateTime(const String& str)
	{
		// CIM format
		if ( str.length() == 25 )
		{
			// validate required characters
			if (  !(str[14] != '.' || (str[21] != '+' && str[21] != '-')) )
			{
				try
				{
					// in CIM, "Fields which are not significant must be
					// replaced with asterisk characters."  We'll convert
					// asterisks to 0s so we can process them.
					String strNoAsterisks(str);
					for (size_t i = 0; i < strNoAsterisks.length(); ++i)
					{
						if (strNoAsterisks[i] == '*')
						{
							strNoAsterisks[i] = '0';
						}
					}
					Int32 year = strNoAsterisks.substring(0, 4).toInt32();
					Int32 month = strNoAsterisks.substring(4, 2).toInt32();
					Int32 day = strNoAsterisks.substring(6, 2).toInt32();
					Int32 hour = strNoAsterisks.substring(8, 2).toInt32();
					Int32 minute = strNoAsterisks.substring(10, 2).toInt32();
					Int32 second = strNoAsterisks.substring(12, 2).toInt32();
					Int32 microseconds = strNoAsterisks.substring(15, 6).toInt32();

					VALIDATE_RANGES(year, month, day, hour, minute, second, microseconds, str);

					Int32 utc = strNoAsterisks.substring(22, 3).toInt32();
					// adjust the time to utc.  According to the CIM spec:
					// "utc is the offset from UTC in minutes"
					if (str[21] == '+')
					{
						utc = 0 - utc;
					}
					minute += utc;

					set(year, month, day, hour, minute, second,
						microseconds, E_UTC_TIME);
					return;
				}
				catch (StringConversionException&)
				{
					// Instead of throwing another exception here, we'll try to parse it in
					// a more general way below.
				}
			}
		}

		// It didn't return from above, so it's not a CIM datetime.  Try to parse
		// it as a free-form date string.
		if ( !str.empty() )
		{
			// This is a general method of extracting the date.
			// It still assumes english names for months and days of week.

			String weekday;
			String day;
			String time;
			int timezone_number = LOCAL_TIME_OFFSET - 1;
			Int32 month_number = -1;
			String year;

			StringArray tokenized_date = str.tokenize();

			// Attempt to fill in the above list of strings...
			for ( StringArray::const_iterator date_token = tokenized_date.begin();
					date_token != tokenized_date.end();
					++date_token )
			{
				// Check to see if it's a day of the week.
				if ( isDOWValid( date_token->c_str() ) )
				{
					if ( weekday.empty() )
					{
						if ( date_token->length() > 3 )
						{
							if ( isLongDOWValid( *date_token ) )
							{
								weekday = *date_token;
							}
							else
							{
								// Invalid long day of week
								badDateTime(Format("bad day of week: \"%1\" in \"%2\"", *date_token, str));
							}
						}
						else
						{
							weekday = *date_token;
						}
					}
					else
					{
						// Multiple weekdays.
						badDateTime(Format("Multiple weekdays in time string \"%1\"", str));
					}
				}
				// Only do this comparison if a month has not already been found.
				else if ( (month_number == -1) &&
					(month_number = decodeShortMonth( date_token->c_str() ) ) != -1 )
				{
					if ( date_token->length() > 3 )
					{
						month_number = decodeLongMonth( date_token->c_str() );

						if ( month_number == -1 )
						{
							// Invalid characters in the long version of the month.
							badDateTime(Format("Invalid long month: \"%1\" in \"%2\"", *date_token, str));
						}
					}
				}
				// Get the time, if the time wasn't already set.
				else if ( time.empty() && (date_token->indexOf(":") != String::npos) )
				{
					// This will be checked below... Assume it's correct.
					time = *date_token;
				}
				// If a day hasn't been found, and this is a number, assume it's the day.
				else if ( day.empty() && isdigit((*date_token)[0]) )
				{
					day = *date_token;
				}
				// If a year hasn't been found, and this is a number, assume it's the year.
				else if ( year.empty() && isdigit((*date_token)[0]) )
				{
					year = *date_token;
				}
				else if ( (timezone_number <= LOCAL_TIME_OFFSET) &&
					(date_token->length() >= 1) &&
					(date_token->length() <= 4) &&
					getTimeZoneOffset(*date_token, timezone_number) )
				{
					// Matched the timezone (nothing to do, it's already been set).
				}
				else
				{
					badDateTime(Format("Unexpected token \"%1\" in \"%2\"", *date_token, str));
				}

			} // for each token.


			// Done looking at tokens.  Verify that all the required fields are present.
			if ( (month_number >= 1) && !day.empty() && !time.empty() && !year.empty() )
			{
				// We've got enough to construct the date.

				// Parse the time
				StringArray time_fields = time.tokenize(":");

				// We need at least the hour and minute, anything other than H:M:S should
				// be in error.
				if ( (time_fields.size() < 2) || (time_fields.size() > 3) )
				{
					badDateTime(Format("Invalid time in string \"%1\" from \"%2\"", time, str));
				}

				try
				{

					Int32 hour;
					Int32 minute;
					Int32 second = 0;
					Int32 microseconds = 0;
					Int32 year_number = year.toInt32();
					Int32 day_number = day.toInt32();

					hour = time_fields[0].toInt32();
					minute = time_fields[1].toInt32();

					if ( time_fields.size() == 3 )
					{
						second = time_fields[2].toInt32();
					}

					VALIDATE_RANGES(year_number, month_number, day_number,
						hour, minute, second, microseconds, str);

					if ( timezone_number <= LOCAL_TIME_OFFSET )
					{
						set(year_number, month_number, day_number, hour,
							minute, second, microseconds, E_LOCAL_TIME);
					}
					else
					{
						// Adjust the time for the timezone.
						// The current numbers have already been validated, so any changes
						// should not do anything unexpected.

						adjustTimeForTimeZone(timezone_number, year_number, month_number, day_number, hour);

						// Check again.
						VALIDATE_RANGES(year_number, month_number, day_number, hour,
							minute, second, microseconds, str);

						set(year_number, month_number, day_number, hour,
							minute, second, microseconds, E_UTC_TIME);
					}
				}
				catch (const StringConversionException&)
				{
					badDateTime(str);
				}
			}
			else
			{
				// Not all required fields available.
				badDateTime(Format("Missing date fields: %1", str));
			}
		}
		else
		{
			// An empty string.
			badDateTime(str);
		}
	}
#endif
	//////////////////////////////////////////////////////////////////////////////
	DateTime::DateTime(time_t t, Int32 microseconds)
		: m_seconds(t)
		, m_microseconds(microseconds)
	{
		normalizeTime(m_seconds, m_microseconds);
	}
	//////////////////////////////////////////////////////////////////////////////
	DateTime::DateTime(int year, int month, int day, int hour, int minute,
		int second, UInt32 microseconds, ETimeOffset timeOffset)
	{
		set(year, month, day, hour, minute, second, microseconds, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	DateTime::~DateTime()
	{
	}
	//////////////////////////////////////////////////////////////////////////////
	inline tm
	DateTime::getTm(ETimeOffset timeOffset) const
	{
		checkNormalTime(m_microseconds);

		if (timeOffset == E_LOCAL_TIME)
		{
			tm theTime;
			localtime_r(&m_seconds, &theTime);
			return theTime;
		}
		else // timeOffset == E_UTC_TIME
		{
			tm theTime;
			gmtime_r(&m_seconds, &theTime);
			return theTime;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	// Throws if tmarg specifies a time out of the allowed range.
	inline void
	DateTime::setTime(tm& tmarg, ETimeOffset timeOffset)
	{
		if (timeOffset == E_LOCAL_TIME)
		{
			errno = 0;
			time_t t = ::mktime(&tmarg);
//			if ((t < 0 && errno != 0) ||
//				(t < BLOCXX_DATETIME_MINIMUM_TIME) ||
//				(t > BLOCXX_DATETIME_MAXIMUM_TIME))
//			{
//				BLOCXX_THROW(DateTimeException,
//					"Attempt to set value outside of allowed DateTime range "
//					"in DateTime::setTime()");
//			}
			m_seconds = t;
		}
		else // timeOffset == E_UTC_TIME
		{
			m_seconds = timeGm(tmarg); // throws if out of range
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getHour(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_hour;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getMinute(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_min;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getSecond(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_sec;
	}
	//////////////////////////////////////////////////////////////////////////////
	Int32
	DateTime::getMicrosecond() const
	{
		return m_microseconds;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getDay(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_mday;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getDow(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_wday;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getMonth(ETimeOffset timeOffset) const
	{
		return getTm(timeOffset).tm_mon+1;
	}
	//////////////////////////////////////////////////////////////////////////////
	int
	DateTime::getYear(ETimeOffset timeOffset) const
	{
		return (getTm(timeOffset).tm_year + 1900);
	}
	//////////////////////////////////////////////////////////////////////////////
	time_t
	DateTime::get() const
	{
		return m_seconds;
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setHour(int hour, ETimeOffset timeOffset)
	{
		tm theTime = getTm(timeOffset);
		theTime.tm_hour = hour;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setMinute(int minute, ETimeOffset timeOffset)
	{
		tm theTime = getTm(timeOffset);
		theTime.tm_min = minute;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setSecond(int second, ETimeOffset timeOffset)
	{
		tm theTime = getTm(timeOffset);
		theTime.tm_sec = second;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setMicrosecond(Int32 microseconds)
	{
		checkNormalTime(m_microseconds);

		if ( (microseconds < 0) || (microseconds >= MICROSECONDS_PER_SECOND) )
		{
			BLOCXX_THROW(DateTimeException, Format("invalid microseconds: %1", microseconds).c_str());
		}
		m_microseconds = microseconds;
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setTime(int hour, int minute, int second, ETimeOffset timeOffset)
	{
		tm theTime = getTm(timeOffset);
		theTime.tm_hour = hour;
		theTime.tm_min = minute;
		theTime.tm_sec = second;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setDay(int day, ETimeOffset timeOffset)
	{

		tm theTime = getTm(timeOffset);
		theTime.tm_mday = day;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setMonth(int month, ETimeOffset timeOffset)
	{
		if (month == 0)
		{
			BLOCXX_THROW(DateTimeException, "invalid month: 0");
		}

		tm theTime = getTm(timeOffset);
		theTime.tm_mon = month-1;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setYear(int year, ETimeOffset timeOffset)
	{
//		if( (year < BLOCXX_DATETIME_MINIMUM_YEAR) || (year > BLOCXX_DATETIME_MAXIMUM_YEAR) )
//		{
//			BLOCXX_THROW(DateTimeException, Format("invalid year: %1", year).c_str());
//		}

		tm theTime = getTm(timeOffset);
		theTime.tm_year = year - 1900;
		setTime(theTime, timeOffset);
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::set(int year, int month, int day, int hour, int minute, int second,
		UInt32 microseconds, ETimeOffset timeOffset)
	{
		tm tmarg;

		// If microseconds is changed to be Int32, then we'll have to
		// use remFloor and divFloor here.
		//Int32 usec = microseconds % MICROSECONDS_PER_SECOND;
		second += int(microseconds / MICROSECONDS_PER_SECOND);

		memset(&tmarg, 0, sizeof(tmarg));
		tmarg.tm_year = year - 1900;
		tmarg.tm_mon = month - 1;
		tmarg.tm_mday = day;
		tmarg.tm_hour = hour;
		tmarg.tm_min = minute;
		tmarg.tm_sec = second;
		if (timeOffset == E_UTC_TIME)
		{
			tmarg.tm_isdst = 0; // don't want dst applied to utc time!
		}
		else
		{
			tmarg.tm_isdst = -1; // don't know about daylight savings time
		}
		setTime(tmarg, timeOffset); // normalizes and throws if out of range
		m_microseconds = microseconds;
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::setToCurrent()
	{
#ifdef BLOCXX_HAVE_GETTIMEOFDAY
		timeval tv;
		if( ::gettimeofday(&tv, NULL) == 0 )
		{
			m_seconds = tv.tv_sec;
			m_microseconds = tv.tv_usec;
		}
		else
		{
			*this = DateTime::getNADT();
		}
#elif defined(BLOCXX_WIN32)
		SYSTEMTIME st; 
		GetSystemTime(&st);
		tm theTime;
	
		theTime.tm_hour = st.wHour;
		theTime.tm_min = st.wMinute;
		theTime.tm_sec = st.wSecond;
		theTime.tm_year = st.wYear - 1900;
		theTime.tm_mon = st.wMonth - 1;
		theTime.tm_mday = st.wDay;
		theTime.tm_wday = st.wDayOfWeek;
		theTime.tm_yday = 0;
		theTime.tm_isdst = -1; 

		m_seconds = mktime(&theTime);
		m_microseconds = st.wMilliseconds*1000;
#else
		errno = 0;
		m_seconds = ::time(NULL);
		m_microseconds = 0;

		if( (m_seconds < 0) && (errno != 0) )
		{
			*this = DateTime::getNADT();
		}
#endif
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::addDays(int days)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			tm theTime = getTm(E_UTC_TIME);
			theTime.tm_mday += days;
			setTime(theTime, E_UTC_TIME);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::addYears(int years)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			tm theTime = getTm(E_UTC_TIME);
			theTime.tm_year += years;

//			VALIDATE_RANGE(theTime.tm_year, BLOCXX_DATETIME_MINIMUM_YEAR, BLOCXX_DATETIME_MAXIMUM_YEAR, Format("addyears(%1)", years));

			setTime(theTime, E_UTC_TIME);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::addMonths(int months)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			tm theTime = getTm(E_UTC_TIME);
			theTime.tm_mon += months;
			setTime(theTime, E_UTC_TIME);
		}
	}
	//////////////////////////////////////////////////////////////////////////////
	void DateTime::addSeconds(long seconds)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			m_seconds += seconds;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	void DateTime::addMinutes(long minutes)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			m_seconds += minutes * 60;
		}
	}

	void DateTime::addMicroseconds(long microseconds)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			m_microseconds += microseconds;
			normalizeTime(m_seconds, m_microseconds);
		}
	}

	void DateTime::addHours(long hours)
	{
		if( !haveSpecialValue(m_microseconds) )
		{
			m_seconds += hours * 60 * 60;
		}
	}

	bool DateTime::operator< ( const DateTime& tm ) const
	{
		if( Time::isInvalid(*this) || Time::isInvalid(tm) )
		{
			return false;
		}
		if( m_seconds == tm.m_seconds )
		{
			return m_microseconds < tm.m_microseconds;
		}

		return m_seconds < tm.m_seconds;
	}

	bool DateTime::operator<= ( const DateTime& tm ) const
	{
		return (*this == tm) || (*this < tm);
	}

	bool DateTime::operator> ( const DateTime& tm ) const
	{
		return tm < *this;
	}

	bool DateTime::operator>= ( const DateTime& tm ) const
	{
		return (*this == tm) || (tm < *this);
	}

	//////////////////////////////////////////////////////////////////////////////
	String
	DateTime::toString(ETimeOffset timeOffset) const
	{
		// This used to call asctime_r(), but that always appends a nasty newline.
		return toString("%a %b %d %H:%M:%S %Y", timeOffset);
	}

	//////////////////////////////////////////////////////////////////////////////
	String DateTime::toString(char const * format, ETimeOffset timeOffset) const
	{
		Time::ESpecialTime flag = getSpecialTimeFlag();

		if( flag == Time::E_TIME_NOT_SPECIAL )
		{
			tm theTime = getTm(timeOffset);
			size_t const BUFSZ = 1024;
			char buf[BUFSZ];
			size_t n = strftime(buf, BUFSZ, format, &theTime);
			buf[n >= BUFSZ ? 0 : n] = '\0';
			return String(buf);
		}
		return Time::getDescriptionForFlag(flag);
	}

	//////////////////////////////////////////////////////////////////////////////
	char const DateTime::DEFAULT_FORMAT[] = "%c";

	//////////////////////////////////////////////////////////////////////////////
	Int16 DateTime::localTimeAndOffset(time_t t, struct tm & t_loc)
	{
		struct tm t_utc;
		struct tm * ptm_utc = ::gmtime_r(&t, &t_utc);
		struct tm * ptm_loc = ::localtime_r(&t, &t_loc);
		if (!ptm_utc || !ptm_loc)
		{
			BLOCXX_THROW(DateTimeException, Format("Invalid time_t: %1", t).c_str());
		}
		int min_diff =
			(t_loc.tm_min - t_utc.tm_min) + 60 * (t_loc.tm_hour - t_utc.tm_hour);
		// Note: UTC offsets can be greater than 12 hours, but are guaranteed to
		// be less than 24 hours.
		int day_diff = t_loc.tm_mday - t_utc.tm_mday;
		int const one_day = 24 * 60;
		if (day_diff == 0)
		{
			return min_diff;
		}
		else if (day_diff == 1 || day_diff < -1)
		{
			// if day_diff < -1, then UTC day is last day of month and local day
			// is 1st of next month.
			return min_diff + one_day;
		}
		else /* day_diff == -1 || day_diff > 1 */
		{
			// if day_diff > 1, then UTC day is 1st of month and local day is last
			// day of previous month.
			return min_diff - one_day;
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	void
	DateTime::set(time_t t, Int32 microseconds)
	{
		time_t t1 = t;
		Int32 m1 = microseconds;
		normalizeTime(t1, m1);

		m_seconds = t1;
		m_microseconds = m1;
	}

	//////////////////////////////////////////////////////////////////////////////
	Int16 DateTime::toLocal(struct tm& tt) const
	{
		checkNormalTime(m_microseconds);

		return DateTime::localTimeAndOffset(time_t(m_seconds), tt);
	}

	//////////////////////////////////////////////////////////////////////////////
	// static
	DateTime
	DateTime::getCurrent()
	{
		DateTime current;
		current.setToCurrent();
		return current;
	}

	namespace Time
	{
		bool isInvalid(const DateTime& dt)
		{
			return isNADT(dt.m_microseconds);
		}

		bool isPosInfinity(const DateTime& dt)
		{
			return isPosInf(dt.m_microseconds);
		}

		bool isNegInfinity(const DateTime& dt)
		{
			return isNegInf(dt.m_microseconds);
		}

		bool isInfinite(const DateTime& dt)
		{
			return isInf(dt.m_microseconds);
		}

		bool isSpecial(const DateTime& dt)
		{
			return haveSpecialValue(dt.m_microseconds);
		}

		TimeDuration timeBetween(const DateTime& dt1, const DateTime& dt2)
		{
			return dt2 - dt1;
		}
	}

	DateTime operator-(DateTime const& x)
	{
		switch(x.getSpecialTimeFlag())
		{
		case Time::E_TIME_NOT_SPECIAL:
			{
				if( x.getMicrosecond() > 0 )
				{
					return DateTime(-x.get() - 1, MICROSECONDS_PER_SECOND - x.getMicrosecond());
				}
				return DateTime(-x.get(),0);
			}
		case Time::E_TIME_NADT:
			return x;
		case Time::E_TIME_POS_INFINITY:
			return DateTime::getNegInfinity();
		case Time::E_TIME_NEG_INFINITY:
			return DateTime::getPosInfinity();
		}

		// never reached.
		return DateTime::getNADT();
	}

	//////////////////////////////////////////////////////////////////////////////
	Time::TimeDuration operator-(DateTime const & x, DateTime const & y)
	{
		Time::ESpecialTime flag1 = x.getSpecialTimeFlag();
		Time::ESpecialTime flag2 = y.getSpecialTimeFlag();

		if( (flag1 != Time::E_TIME_NOT_SPECIAL) || (flag2 != Time::E_TIME_NOT_SPECIAL) )
		{
			if( (flag1 == Time::E_TIME_NADT) || (flag2 == Time::E_TIME_NADT) )
			{
				return Time::TimeDuration(Time::E_TIME_NADT);
			}

			if( flag1 == flag2 )
			{
				// +inf - +inf --> nadt
				// -inf - -inf --> nadt
				return Time::TimeDuration(Time::E_TIME_NADT);
			}

			if( flag2 == Time::E_TIME_NEG_INFINITY )
			{
				// non-special - -inf --> +inf
				return Time::TimeDuration(Time::E_TIME_POS_INFINITY);
			}
			else if( flag2 == Time::E_TIME_POS_INFINITY)
			{
				// non-special - +inf --> -inf
				return Time::TimeDuration(Time::E_TIME_NEG_INFINITY);
			}
			else
			{
				// This covers the case where ( flag1 != E_TIME_NOT_SPECIAL ).
				// x must be +inf or -inf and y must be non-special.
				//
				// +inf - non-special --> +inf
				// -inf - non-special --> -inf
				return Time::TimeDuration(flag1);
			}

			// Never reached unless more time flags are added.
			return Time::TimeDuration(Time::E_TIME_NADT);
		}

		// Neither dt1 nor dt2 are special, so we just do simple subtraction.
		Int64 seconds = (Int64)x.get() - (Int64)y.get();
		Int64 microseconds = x.getMicrosecond() - y.getMicrosecond();

		return Time::TimeDuration( seconds * MICROSECONDS_PER_SECOND + microseconds);
	}

	//////////////////////////////////////////////////////////////////////////////
	DateTime operator+(DateTime dt, Real64 secs)
	{
		return dt + Time::TimeDuration(secs);
	}

	DateTime operator+(const DateTime& dt, const Time::TimeDuration& td)
	{
		Time::ESpecialTime flag1 = dt.getSpecialTimeFlag();
		Time::ESpecialTime flag2 = td.getSpecialFlag();

		if( (flag1 == Time::E_TIME_NADT) || (flag2 == Time::E_TIME_NADT) )
		{
			// nadt + (non-special|-inf|+inf|nadt) --> nadt
			// (non-special|-inf|+inf) + nadt --> nadt
			return DateTime::getNADT();
		}
		else if( ( (flag1 == Time::E_TIME_POS_INFINITY) &&
				(flag2 == Time::E_TIME_NEG_INFINITY) ) ||
			( (flag1 == Time::E_TIME_NEG_INFINITY) &&
				(flag2 == Time::E_TIME_POS_INFINITY) ) )
		{
			// +inf + -inf --> nadt
			// -inf + +inf --> nadt
			return DateTime::getNADT();
		}
		else if( flag1 != Time::E_TIME_NOT_SPECIAL )
		{
			// +inf + (non-special|+inf) --> +inf
			// -inf + (non-special|-inf) --> -inf
			return dt;
		}

		switch(flag2)
		{
		case Time::E_TIME_NOT_SPECIAL:
			// non-special + non-special --> non-special
			{
				Int64 td_seconds = (td.microseconds() - td.microsecondInSecond()) / MICROSECONDS_PER_SECOND;
				Int64 td_microseconds = td.microsecondInSecond();

				// The addition of microseconds (both are Int32) is safe and cannot
				// overflow because they are both guaranteed to be between zero and
				// one million (inclusive).
				return DateTime(time_t(Int64(dt.get()) + td_seconds),
					dt.getMicrosecond() + td_microseconds);
			}
		case Time::E_TIME_NEG_INFINITY:
			// non-special + -inf --> -inf
			return DateTime::getNegInfinity();
		case Time::E_TIME_POS_INFINITY:
			// non-special + +inf --> +inf
			return DateTime::getPosInfinity();
		case Time::E_TIME_NADT:
			// Already handled above, not reached.  Provided only to complete the enum.
			break;
		}

		// Never reached.
		return DateTime::getNADT();
	}

	DateTime operator+(const Time::TimeDuration& td, const DateTime& dt)
	{
		return dt + td;
	}

	DateTime operator-(const DateTime& dt, const Time::TimeDuration& td)
	{
		return dt + -td;
	}

	DateTime& operator+=(DateTime& dt, const Time::TimeDuration& td)
	{
		dt = dt + td;

		return dt;
	}

	DateTime& operator-=(DateTime& dt, const Time::TimeDuration& td)
	{
		dt = dt - td;

		return dt;
	}

} // end namespace BLOCXX_NAMESPACE

