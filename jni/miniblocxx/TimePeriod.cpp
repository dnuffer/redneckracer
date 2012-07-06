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
#include "miniblocxx/TimePeriod.hpp"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/DateTime.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/TimeUtils.hpp"

#undef min
namespace BLOCXX_NAMESPACE
{
	namespace Time
	{
		namespace
		{
			void normalizeEndpoints(DateTime& start, DateTime& finish)
			{
				if( isInvalid(start) || isInvalid(finish) )
				{
					start = DateTime::getNADT();
					finish = DateTime::getNADT();
				}
				else if( start >= finish )
				{
					start = DateTime::getPosInfinity();
					finish = DateTime::getNegInfinity();
				}
			}
		}

		TimePeriod::TimePeriod()
			: m_start()
			, m_finish()
		{
		}

		TimePeriod::TimePeriod(const DateTime& start, const DateTime& finish)
			: m_start(start)
			, m_finish(finish)
		{
			normalizeEndpoints(m_start, m_finish);
		}

		TimePeriod::TimePeriod(const DateTime& start, const TimeDuration& length)
			: m_start(start)
			, m_finish(start + length)
		{
			normalizeEndpoints(m_start, m_finish);
		}

		TimePeriod::TimePeriod(const TimePeriod& tp)
			: m_start(tp.m_start)
			, m_finish(tp.m_finish)
		{
		}

		TimePeriod::~TimePeriod()
		{
		}

		TimePeriod& TimePeriod::operator=(const TimePeriod& tp)
		{
			m_start = tp.m_start;
			m_finish = tp.m_finish;

			return *this;
		}

		TimeDuration TimePeriod::length() const
		{
			if( isInvalid(m_start) )
			{
				return TimeDuration(E_TIME_NADT);
			}

			if( m_start >= m_finish )
			{
				return TimeDuration(Int64(0));
			}

			return timeBetween(m_start, m_finish);
		}

		bool TimePeriod::empty() const
		{
			return m_start >= m_finish;
		}

		bool TimePeriod::contains(const DateTime& dt) const
		{
			return (m_start <= dt) && (dt < m_finish);
		}

		bool TimePeriod::contains(const TimePeriod& tp) const
		{
			if( isInvalid(m_start) && !isInvalid(tp.m_start) )
			{
				return false;
			}
			if( tp.empty() )
			{
				return true;
			}
			return (m_start <= tp.m_start) && (tp.m_finish <= m_finish);
		}

		String TimePeriod::toString() const
		{
			return toString("%Y-%m-%d %H:%M:%S");
		}

		namespace
		{
			String realDateString(const DateTime& dt, const String& format)
			{
				ESpecialTime flag = dt.getSpecialTimeFlag();

				if( flag == E_TIME_NADT )
				{
					return "*INVALID TIME*";
				}
				else if( flag == E_TIME_POS_INFINITY )
				{
					return "*INFINITY*";
				}
				else if( flag == E_TIME_NEG_INFINITY )
				{
					return "*-INFINITY*";
				}
				else
				{
					return dt.toString(format.c_str());
				}
			}
		}

		String TimePeriod::toString(const String& format) const
		{
			return Format("[%1, %2)",
				realDateString(m_start, format),
				realDateString(m_finish, format));
		}

		bool operator==(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			return (tp1.begin() == tp2.begin()) && (tp1.end() == tp2.end());
		}

		bool operator!=(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			return (tp1.begin() != tp2.begin()) || (tp1.end() != tp2.end());
		}


		bool intersect(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			if( isInvalid(tp1) || isInvalid(tp2) )
			{
				return false;
			}

			return ((tp1.end() > tp2.begin()) && (tp1.begin() < tp2.end()));
		}

		TimePeriod intersection(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			if( !intersect(tp1, tp2) )
			{
				return TimePeriod(DateTime::getNADT(), DateTime::getNADT());
			}

			return TimePeriod(std::max(tp1.begin(), tp2.begin()), std::min(tp1.end(), tp2.end()));
		}

		TimePeriod merge(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			if( isInvalid(tp1) || isInvalid(tp2) )
			{
				return TimePeriod(DateTime::getNADT(), DateTime::getNADT());
			}
			else if( intersect(tp1, tp2) || (tp1.end() == tp2.begin()) || (tp1.begin() == tp2.begin()) )
			{
				return TimePeriod(std::min(tp1.begin(), tp2.begin()), std::max(tp1.end(), tp2.end()));
			}

			return TimePeriod(DateTime::getNADT(), DateTime::getNADT());
		}

		TimePeriod span(const TimePeriod& tp1, const TimePeriod& tp2)
		{
			if( isInvalid(tp1) || isInvalid(tp2) )
			{
				return TimePeriod(DateTime::getNADT(), DateTime::getNADT());
			}

			return TimePeriod(std::min(tp1.begin(), tp2.begin()), std::max(tp1.end(), tp2.end()));
		}

	} // end namespace Time

} // end namespace BLOCXX_NAMESPACE
