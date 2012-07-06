/*******************************************************************************
* Copyright (C) 2009, Quest Software, Inc. All rights reserved.
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
 * @author Dan Nuffer
 * @author Kevin Harris
 */

#ifndef BLOCXX_STRICT_WEAK_ORDERING_HPP_INCLUDE_GUARD_
#define BLOCXX_STRICT_WEAK_ORDERING_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

namespace BLOCXX_NAMESPACE
{

template<class T1>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1)
{
	return lhs1 < rhs1;
}
#if defined(BLOCXX_USE_CXX_0X)
// C++0x implementation
template<typename T1, typename ...Args>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1, const Args& ...args)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(args...);
}
// End of C++0x implementation
#else
template<class T1, class T2>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2);
}
template<class T1, class T2, class T3>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3);
}
template<class T1, class T2, class T3, class T4>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4);
}
template<class T1, class T2, class T3, class T4, class T5>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5);
}
template<class T1, class T2, class T3, class T4, class T5, class T6>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5,
		const T6& lhs6, const T6& rhs6)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5,
			lhs6, rhs6);
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class
T7>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5,
		const T6& lhs6, const T6& rhs6,
		const T7& lhs7, const T7& rhs7)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5,
			lhs6, rhs6,
			lhs7, rhs7);
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class
T7, class T8>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5,
		const T6& lhs6, const T6& rhs6,
		const T7& lhs7, const T7& rhs7,
		const T8& lhs8, const T8& rhs8)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5,
			lhs6, rhs6,
			lhs7, rhs7,
			lhs8, rhs8);
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class
T7, class T8, class T9>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5,
		const T6& lhs6, const T6& rhs6,
		const T7& lhs7, const T7& rhs7,
		const T8& lhs8, const T8& rhs8,
		const T9& lhs9, const T9& rhs9)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5,
			lhs6, rhs6,
			lhs7, rhs7,
			lhs8, rhs8,
			lhs9, rhs9);
}
template<class T1, class T2, class T3, class T4, class T5, class T6, class
T7, class T8, class T9, class T10>
bool StrictWeakOrdering(const T1& lhs1, const T1& rhs1,
		const T2& lhs2, const T2& rhs2,
		const T3& lhs3, const T3& rhs3,
		const T4& lhs4, const T4& rhs4,
		const T5& lhs5, const T5& rhs5,
		const T6& lhs6, const T6& rhs6,
		const T7& lhs7, const T7& rhs7,
		const T8& lhs8, const T8& rhs8,
		const T9& lhs9, const T9& rhs9,
		const T10& lhs10, const T10& rhs10)
{
	if (lhs1 < rhs1) return true;
	else if (rhs1 < lhs1) return false;
	else return StrictWeakOrdering(lhs2, rhs2,
			lhs3, rhs3,
			lhs4, rhs4,
			lhs5, rhs5,
			lhs6, rhs6,
			lhs7, rhs7,
			lhs8, rhs8,
			lhs9, rhs9,
			lhs10, rhs10);
}
// End of non-C++0x implementation
#endif

} // end namespace BLOCXX_NAMESPACE

#endif
