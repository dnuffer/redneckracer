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


#ifndef CPPUNIT_TESTSUITE_H
#define CPPUNIT_TESTSUITE_H

#include "Guards.hpp"

#include "Test.hpp"
#include "TestResult.hpp"

#include <vector>

class TestResult;

/*
 * A TestSuite is a Composite of Tests.
 * It runs a collection of test cases. Here is an example.
 *
 * TestSuite *suite= new TestSuite;
 * suite->addTest(new TestCaller<MathTest> ("testAdd", testAdd));
 * suite->addTest(new TestCaller<MathTest> ("testDivideByZero", testDivideByZero));
 *
 * Note that TestSuites assume lifetime
 * control for any tests added to them.
 *
 * see Test and TestCaller
 */


class TestSuite : public Test
{
	REFERENCEOBJECT (TestSuite)

public:
						TestSuite       (const char* name = "");
						~TestSuite      ();

	void                run             (TestResult *result);
	int                 countTestCases  ();
	void                addTest         (Test *test);
	const char*         toString        ();

	virtual void        deleteContents  ();

private:
	std::vector<Test *> m_tests;
	const char*   m_name;


};

#define ADD_TEST_TO_SUITE(cls, test) \
	testSuite->addTest (new TestCaller <cls> \
			(#test, \
			&cls::test));



#endif
