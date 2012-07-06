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



#include "TestSuite.hpp"
#include "TestResult.hpp"
#include <stdio.h>


// Deletes all tests in the suite.
void TestSuite::deleteContents ()
{
	for (std::vector<Test *>::iterator it = m_tests.begin ();
			it != m_tests.end ();
			++it)
		delete *it;

}


// Runs the tests and collects their result in a TestResult.
void TestSuite::run (TestResult *result)
{
	for (std::vector<Test *>::iterator it = m_tests.begin ();
			it != m_tests.end ();
			++it) {
		if (result->shouldStop ())
			break;

		Test *test = *it;
		test->run (result);
	}

}


// Counts the number of test cases that will be run by this test.
int TestSuite::countTestCases ()
{
	int count = 0;

	for (std::vector<Test *>::iterator it = m_tests.begin ();
			it != m_tests.end ();
			++it)
		count += (*it)->countTestCases ();

	return count;

}


// Default constructor
TestSuite::TestSuite (const char* name)
: m_name (name)
{}


// Destructor
TestSuite::~TestSuite ()
{ deleteContents (); }


// Adds a test to the suite. 
void TestSuite::addTest (Test *test)
{ m_tests.push_back (test); }


// Returns a string representation of the test suite.
const char* TestSuite::toString ()
{
	static char buff[128];
	snprintf( buff, sizeof(buff), "suite %s", m_name );
	return buff;
}
