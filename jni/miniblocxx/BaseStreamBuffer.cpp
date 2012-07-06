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
 * @author Bart Whiteley
 * @author Dan Nuffer
 */

#include "miniblocxx/BLOCXX_config.h"

#ifndef BLOCXX_NO_IOSTREAMS

#include "miniblocxx/BaseStreamBuffer.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Assertion.hpp"
#include <iostream> // for cerr
#include <cstring> // for memcpy
#include <cstdio>

namespace BLOCXX_NAMESPACE
{

//////////////////////////////////////////////////////////////////////////////
BaseStreamBuffer::BaseStreamBuffer(EDirectionFlag direction, size_t bufSize)
	: m_bufSize(bufSize), m_inputBuffer(NULL), m_outputBuffer(NULL)
{
	if (direction == E_IN || direction == E_IN_OUT)
	{
		m_inputBuffer = new char[m_bufSize];
		initGetBuffer();
	}
	if (direction == E_OUT || direction == E_IN_OUT)
	{
		m_outputBuffer = new char[m_bufSize];
		initPutBuffer();
	}
}
//////////////////////////////////////////////////////////////////////////////
void
BaseStreamBuffer::initBuffers()
{
	initPutBuffer();
	initGetBuffer();
}
//////////////////////////////////////////////////////////////////////////////
void
BaseStreamBuffer::initPutBuffer()
{
	setp(m_outputBuffer, m_outputBuffer + m_bufSize);
}
//////////////////////////////////////////////////////////////////////////////
void
BaseStreamBuffer::initGetBuffer()
{
	setg(m_inputBuffer, m_inputBuffer, m_inputBuffer);
}
//////////////////////////////////////////////////////////////////////////////
BaseStreamBuffer::~BaseStreamBuffer()
{
	delete [] m_inputBuffer;
	delete [] m_outputBuffer;
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::sync()
{
	return buffer_out();
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::buffer_out()
{
    int cnt = int(pptr() - pbase());
    int retval = buffer_to_device(m_outputBuffer, cnt);
    pbump(-cnt);
    return retval;
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::overflow(int c)
{
	if (buffer_out() < 0)
	{
		return EOF;
	}
	else
	{
		if (c != EOF)
		{
			return sputc(char(c));
		}
		else
		{
			return c;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
std::streamsize
BaseStreamBuffer::xsputn(const char* s, std::streamsize n)
{
	if (n < epptr() - pptr())
	{
		memcpy(pptr(), s, n * sizeof(char));
		pbump(int(n));
		return n;
	}
	else
	{
		for (std::streamsize i = 0; i < n; i++)
		{
			if (sputc(s[i]) == EOF)
			{
				return i;
			}
		}
		return n;
	}
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::underflow()
{
    if (gptr() < egptr())
    {
        return static_cast<unsigned char>(*gptr());	// need a static_cast so a -1 doesn't turn into an EOF
    }
    if (buffer_in() < 0)
    {
        return EOF;
    }
    else
    {
        return static_cast<unsigned char>(*gptr());	// need a static_cast so a -1 doesn't turn into an EOF
    }
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::buffer_in()
{
	int retval = buffer_from_device(m_inputBuffer, int(m_bufSize));
	if (retval <= 0)
	{
		setg(0,0,0);
		return -1;
	}
	else
	{
		setg(m_inputBuffer, m_inputBuffer, m_inputBuffer + retval);
		return retval;
	}
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::buffer_to_device(const char* c, int n)
{
	(void) c;
	(void) n;
	BLOCXX_ASSERT("Not implemented, should overwrite" == 0);
	return -1; // make the compiler happy
}
//////////////////////////////////////////////////////////////////////////////
int
BaseStreamBuffer::buffer_from_device(char* c, int n)
{
	(void) c;
	(void) n;
	BLOCXX_ASSERT("Not implemented, should overwrite" == 0);
	return -1; // make the compiler happy
}

} // end namespace BLOCXX_NAMESPACE

#endif // BLOCXX_NO_IOSTREAMS
