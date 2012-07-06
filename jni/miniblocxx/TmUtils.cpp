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
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/TmUtils.hpp"
#include "miniblocxx/stl/limits"
#include "miniblocxx/DivRem.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/DateTime.hpp"

using namespace std;
using namespace blocxx;

/**
 * @author Kevin S. Van Horn
 */

namespace
{
	int const epochYear = 1970;
	int const tmYearBase = 1900;

	/** 
	 * @todo Fold these in with the other tables like these somewhere
	 * in DateTime.cpp.
	 */
	int daysInMonthArr[] =
	{
		31, // jan
		28, // feb
		31, // mar
		30, // apr
		31, // may
		30, // jun
		31, // jul
		31, // aug
		30, // sep
		31, // oct
		30, // nov
		31  // dec
	};

	// first index: month (0 to 11)
	// second index: 0 == not leap year, 1 == leap year
	int daysToMonthStartArr[][2] =
	{
		{   0,   0 }, // jan
		{  31,  31 }, // feb
		{  59,  60 }, // mar
		{  90,  91 }, // apr
		{ 120, 121 }, // may
		{ 151, 152 }, // jun
		{ 181, 182 }, // jul
		{ 212, 213 }, // aug
		{ 243, 244 }, // sep
		{ 273, 274 }, // oct
		{ 304, 305 }, // nov
		{ 334, 335 }  // dec
	};

	inline int year(struct tm const & tm)
	{
		return tm.tm_year + tmYearBase;
	}

	inline int isLeapYear(struct tm const & tm)
	{
		int y = year(tm);
		return (
			remFloor(y, 4) == 0 &&
			(remFloor(y, 100) != 0 || remFloor(y, 400) == 0));
	}

	inline int daysToMonthStart(struct tm const & tm)
	{
		return daysToMonthStartArr[tm.tm_mon][isLeapYear(tm)];
	}

	inline int leapDaysFromEpochToYear(int year)
	{
		int const year1 = year - 1;
		int const ep1 = epochYear - 1;
		int const leapDaysBeforeEpoch = ep1 / 4 - ep1 / 100 + ep1 / 400;
		int const leapDaysBeforeYear = year1 / 4 - year1 / 100 + year1 / 400;
		return leapDaysBeforeYear - leapDaysBeforeEpoch;
	}

	int daysInMonth(struct tm const & tm)
	{
		int mon = tm.tm_mon;
		int n = daysInMonthArr[mon];
		if (mon == 1 /*feb*/)
		{
			n += isLeapYear(tm);
		}
		return n;
	}

	void normMonth(struct tm & tm)
	{
		int mon = tm.tm_mon;
		tm.tm_mon = remFloor(mon, 12);
		tm.tm_year += divFloor(mon, 12);
	}
}

namespace BLOCXX_NAMESPACE
{
	void normTm(struct tm & tm)
	{
		int sec = tm.tm_sec;
		tm.tm_sec = remFloor(sec, 60);
		tm.tm_min += divFloor(sec, 60);

		int minute = tm.tm_min;
		tm.tm_min = remFloor(minute, 60);
		tm.tm_hour += divFloor(minute, 60);

		int hour = tm.tm_hour;
		tm.tm_hour = remFloor(hour, 24);
		tm.tm_mday += divFloor(hour, 24);

		normMonth(tm);

		int maxMday;
		while (tm.tm_mday > (maxMday = daysInMonth(tm)))
		{
			tm.tm_mday -= maxMday;
			++tm.tm_mon;
			normMonth(tm);
		}
		while (tm.tm_mday < 1)
		{
			--tm.tm_mon;
			normMonth(tm);
			tm.tm_mday += daysInMonth(tm);			
		}
	}

	time_t timeGm(struct tm & tm)
	{
		time_t const daysInNonLeapYear = 365;
		time_t const secsInDay = 86400;
		time_t const secsInHour = 3600;
		time_t const secsInMinute = 60;

		normTm(tm);
		int y = year(tm);
//		if (y < BLOCXX_DATETIME_MINIMUM_YEAR ||
//			y > BLOCXX_DATETIME_MAXIMUM_YEAR)
//		{
//			BLOCXX_THROW(DateTimeException,
//				Format("invalid year: %1", y).c_str());
//		}
		time_t days = time_t(y - epochYear) * daysInNonLeapYear +
		              leapDaysFromEpochToYear(y) +
		              daysToMonthStart(tm) + (tm.tm_mday - 1);
		return days * secsInDay + tm.tm_hour * secsInHour +
		       tm.tm_min * secsInMinute + tm.tm_sec;
	}
}
