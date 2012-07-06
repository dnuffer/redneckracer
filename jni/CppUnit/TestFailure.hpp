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



#ifndef CPPUNIT_TESTFAILURE_H
#define CPPUNIT_TESTFAILURE_H

#include "Guards.hpp"
#include "CppUnitException.hpp"

class Test;


/*
 * A TestFailure collects a failed test together with
 * the caught exception.
 *
 * TestFailure assumes lifetime control for any exception
 * passed to it.  The lifetime of tests is handled by
 * their TestSuite (if they have been added to one) or
 * whomever creates them.
 * 
 * see TestResult
 * see TestSuite
 *
 */

class TestFailure 
{
	REFERENCEOBJECT (TestFailure)

public:
						TestFailure (Test *failedTest, CppUnitException *thrownException);
						~TestFailure ();

	Test                *failedTest ();
	CppUnitException    *thrownException ();
	const char*         toString ();

protected:
	Test                *m_failedTest;
	CppUnitException    *m_thrownException;

};




#endif
