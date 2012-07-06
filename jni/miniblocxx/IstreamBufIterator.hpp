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
 * @author Dan Nuffer
 */

#ifndef BLOCXX_ISTREAM_BUF_ITERATOR_HPP_INCLUDE_GUARD_
#define BLOCXX_ISTREAM_BUF_ITERATOR_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include <iostream>
#include <cstdio>

namespace BLOCXX_NAMESPACE
{

	/**
	 * This class differs from a std::istreambuf_iterator in that dereferncing an eof iterator returns 0
	 * This class is mostly just a replacement for std::istreambuf_iterator<>, since it
	 * doesn't exist in the gcc 2.95.x standard lib.  We also added/removed some features to make it more efficient.
	 */
	class IstreamBufIterator
	{
	public:
		class proxy
		{
			friend class IstreamBufIterator;
		public:
			char operator*() {  return m_keep; }
		private:
			proxy(char c) : m_keep(c) {}
			char m_keep;
		};
		IstreamBufIterator() : m_sbuf(0) {}
		IstreamBufIterator(std::istream& s) : m_sbuf(s.rdbuf()->sgetc() == EOF ? 0 : s.rdbuf()) {}
		IstreamBufIterator(std::streambuf* b) : m_sbuf(b->sgetc() == EOF ? 0 : b) {}
		char operator*() const
		{
			if (m_sbuf)
			{
				return (m_sbuf->sgetc());
			}
			else
			{
				return 0;
			}
		}
		IstreamBufIterator& operator++()
		{
			m_sbuf->sbumpc();
			if (m_sbuf->sgetc() == EOF)
			{
				m_sbuf = 0;
			}
			return *this;
		}
		proxy operator++(int)
		{
			proxy temp(m_sbuf->sgetc());
			m_sbuf->sbumpc();
			if (m_sbuf->sgetc() == EOF)
			{
				m_sbuf = 0;
			}
			return temp;
		}

		bool equal(const IstreamBufIterator& b) const
		{
			return (m_sbuf == 0) ^ (b.m_sbuf != 0);	// do a little manual strength reduction
			//         if (((m_sbuf == 0) && (b.m_sbuf == 0)) || ((m_sbuf != 0) && (b.m_sbuf != 0)))
			//         {
			//             return true;
			//         }
			//         else
			//         {
			//             return false;
			//         }
		}
	private:
		std::streambuf* m_sbuf;
	};
	inline bool operator==(const IstreamBufIterator& lhs, const IstreamBufIterator& rhs)
	{
		return lhs.equal(rhs);
	}
	inline bool operator!=(const IstreamBufIterator& lhs, const IstreamBufIterator& rhs)
	{
		return !lhs.equal(rhs);
	}

} // end namespace BLOCXX_NAMESPACE

#endif
