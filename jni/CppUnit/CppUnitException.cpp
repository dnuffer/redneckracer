/*******************************************************************************
* Copyright (C) 2001-2004 Quest Software, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
*  - Neither the name of Quest Software, Inc. nor the names of its
*    contributors may be used to endorse or promote products derived from this
*    software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL Quest Software, Inc. OR THE CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "CppUnitException.hpp"
#include <cstring>
#include <stdlib.h>
#include <algorithm> // for std::swap

CppUnitException::CppUnitException (const char* message, long lineNumber,
		const char* fileName)
: m_message (strdup(message)), m_lineNumber (lineNumber), m_fileName (strdup(fileName))
{}


// Destruct the exception
CppUnitException::~CppUnitException () throw()
{
	free(m_message);
	free(m_fileName);
}

CppUnitException::CppUnitException(CppUnitException const& x)
	: exception(x)
	, m_message(x.m_message ? strdup(x.m_message) : 0)
	, m_lineNumber(x.m_lineNumber)
	, m_fileName(x.m_fileName ? strdup(x.m_fileName) : 0)
{
}

CppUnitException& CppUnitException::operator=(CppUnitException x)
{
	x.swap(*this);
	return *this;
}

void CppUnitException::swap(CppUnitException& x)
{
	std::swap(m_message, x.m_message);
	std::swap(m_lineNumber, x.m_lineNumber);
	std::swap(m_fileName, x.m_fileName);
}

// Return descriptive message
const char *CppUnitException::what() const throw ()
{ return m_message; }

// The line on which the error occurred
long CppUnitException::lineNumber ()
{ return m_lineNumber; }


// The file in which the error occurred
const char* CppUnitException::fileName ()
{ return m_fileName; }

