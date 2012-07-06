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



/*
 * A command line based tool to run tests.
 * TestRunner expects as its only argument the name of a TestCase class.
 * TestRunner prints out a trace as the tests are executed followed by a
 * summary at the end.
 *
 * You can add to the tests that the TestRunner knows about by
 * making additional calls to "addTest (...)" in main.
 *
 * Here is the synopsis:
 *
 * TestRunner [-wait] ExampleTestCase
 *
 */

#include <iostream>
#include <vector>
#include <cstdlib> // for exit()
#include "TestRunner.hpp"
#include "TextTestResult.hpp"
#include "Test.hpp"

using std::cout;
using std::endl;
using std::string;
using std::cin;

void TestRunner::printBanner ()
{
	cout << "Usage: driver [ -wait ] testName, where name is the name of a test case class" << endl;
}


int TestRunner::run (int ac, char **av)
{
	string  testCase;
	int numberOfTests = 0;
	int numberOfFailures = 0;

	for (int i = 1; i < ac; i++) {

		if (string (av [i]) == "-wait") {
			m_wait = true;
			continue;
		}

		testCase = av [i];

		cout << testCase;
		numberOfFailures += run( testCase );
		numberOfTests++;
	}

	if (numberOfTests == 0) {
		printBanner ();
		return numberOfFailures;
	}

	if (m_wait) {
		cout << "<RETURN> to continue" << endl;
		cin.get ();

	}
	return numberOfFailures;
}

int TestRunner::run (string testCase)
{
	int numberOfFailures = 0;
	if (testCase == "")
	{
		printBanner ();
		return numberOfFailures;
	}

	Test *testToRun = NULL;

	for (mappings::iterator it = m_mappings.begin ();
		  it != m_mappings.end ();
		  ++it)
	{
		if ((*it).first == testCase)
		{
			testToRun = (*it).second;
			numberOfFailures += run (testToRun);
		}
	}
	if (!testToRun)
	{
		cout << "Test " << testCase << " not found." << endl;
	}
	return numberOfFailures;
}

int TestRunner::runAll ()
{
	string  testCase;
	int numberOfTests = 0;
	int numberOfFailures = 0;

	Test *testToRun = NULL;

	for (mappings::iterator it = m_mappings.begin ();
			it != m_mappings.end ();
			++it)
	{
		cout << (*it).first << std::flush;
		testToRun = (*it).second;
		numberOfFailures += run (testToRun);
		numberOfTests++;
	}

	if (numberOfTests == 0)
	{
		printBanner ();
	}

	return numberOfFailures;
}



TestRunner::~TestRunner ()
{
	for (mappings::iterator it = m_mappings.begin ();
		it != m_mappings.end ();
		++it)
	{
		delete it->second;
	}
}


int TestRunner::run (Test *test)
{
	TextTestResult  result;

	test->run (&result);

	cout << result << endl;

	if( result.shouldStop() )
	{
		exit(1);
	}

	if( result.wasSuccessful() )
	{
		return 0;
	}
	return 1;
}

