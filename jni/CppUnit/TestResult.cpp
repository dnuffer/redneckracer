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

#include "TestResult.hpp"
#include "TestFailure.hpp"

// Destroys a test result
TestResult::~TestResult ()
{
	std::vector<TestFailure *>::iterator it;

	for (it = m_errors.begin (); it != m_errors.end (); ++it)
		delete *it;

	for (it = m_failures.begin (); it != m_failures.end (); ++it)
		delete *it;

	delete m_syncObject;
}

// Construct a TestResult
TestResult::TestResult ()
: m_syncObject (new SynchronizationObject)
{ m_runTests = 0; m_runConditions = 0; m_stop = false; }


// Adds an error to the list of errors. The passed in exception
// caused the error
void TestResult::addError (Test *test, CppUnitException *e)
{ ExclusiveZone zone (m_syncObject); m_errors.push_back (new TestFailure (test, e)); }


// Adds a failure to the list of failures. The passed in exception
// caused the failure.
void TestResult::addFailure (Test *test, CppUnitException *e)
{ ExclusiveZone zone (m_syncObject); m_failures.push_back (new TestFailure (test, e)); }


// Informs the result that a test will be started.
void TestResult::startTest (Test *test)
{
	ExclusiveZone zone (m_syncObject); 
	m_runTests++;
}

// Informs the result that a test will be started.
void TestResult::testCondition (Test *test)
{
	ExclusiveZone zone (m_syncObject); 
	m_runConditions++;
}

// Informs the result that a test was completed.
void TestResult::endTest (Test *test)
{
	ExclusiveZone zone (m_syncObject);
}


// Gets the number of run tests.
int TestResult::runTests ()
{ ExclusiveZone zone (m_syncObject); return m_runTests; }

// Gets the number of run tests.
int TestResult::testedConditions ()
{ ExclusiveZone zone (m_syncObject); return m_runConditions; }

// Gets the number of detected errors.
int TestResult::testErrors ()
{ ExclusiveZone zone (m_syncObject); return m_errors.size (); }


// Gets the number of detected failures.
int TestResult::testFailures ()
{ ExclusiveZone zone (m_syncObject); return m_failures.size (); }


// Returns whether the entire test was successful or not.
bool TestResult::wasSuccessful ()
{ ExclusiveZone zone (m_syncObject); return m_failures.empty() && m_errors.empty(); }


// Returns a vector of the errors.
std::vector<TestFailure *>& TestResult::errors ()
{ ExclusiveZone zone (m_syncObject); return m_errors; }


// Returns a vector of the failures.
std::vector<TestFailure *>& TestResult::failures ()
{ ExclusiveZone zone (m_syncObject); return m_failures; }


// Returns whether testing should be stopped
bool TestResult::shouldStop ()
{ ExclusiveZone zone (m_syncObject); return m_stop; }


// Stop testing
void TestResult::stop ()
{ ExclusiveZone zone (m_syncObject); m_stop = true; }

// Accept a new synchronization object for protection of this instance
// TestResult assumes ownership of the object
void TestResult::setSynchronizationObject (SynchronizationObject *syncObject)
{ delete m_syncObject; m_syncObject = syncObject; }

