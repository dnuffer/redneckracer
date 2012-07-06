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


#ifndef CPPUNIT_TESTRESULT_H
#define CPPUNIT_TESTRESULT_H

#include <vector>

#include "Guards.hpp"


class CppUnitException;
class Test;
class TestFailure;


/*
 * A TestResult collects the results of executing a test case. It is an 
 * instance of the Collecting Parameter pattern.
 *
 * The test framework distinguishes between failures and errors.
 * A failure is anticipated and checked for with assertions. Errors are
 * unanticipated problems signified by exceptions that are not generated
 * by the framework.
 *
 * TestResult supplies a template method 'setSynchronizationObject ()'
 * so that subclasses can provide mutual exclusion in the face of multiple
 * threads.  This can be useful when tests execute in one thread and
 * they fill a subclass of TestResult which effects change in another 
 * thread.  To have mutual exclusion, override setSynchronizationObject ()
 * and make sure that you create an instance of ExclusiveZone at the 
 * beginning of each method.
 *
 * see Test
 */

class TestResult
{
	REFERENCEOBJECT (TestResult)

public:
										TestResult  ();
	virtual                             ~TestResult ();

	virtual void                        addError       (Test *test, CppUnitException *e);
	virtual void                        addFailure     (Test *test, CppUnitException *e);
	virtual void                        startTest      (Test *test);
	virtual void                        testCondition  (Test *test);
	virtual void                        endTest        (Test *test);
	virtual int                         runTests       ();
	virtual int                         testedConditions();
	virtual int                         testErrors     ();
	virtual int                         testFailures   ();
	virtual bool                        wasSuccessful  ();
	virtual bool                        shouldStop     ();
	virtual void                        stop           ();

	virtual std::vector<TestFailure *>& errors         ();
	virtual std::vector<TestFailure *>& failures       ();


	class SynchronizationObject
	{
	public:
								SynchronizationObject  () {}
		virtual                 ~SynchronizationObject () {}

		virtual void            lock                   () {}
		virtual void            unlock                 () {}
	};

	class ExclusiveZone
	{
		SynchronizationObject   *m_syncObject;

	public:
								ExclusiveZone (SynchronizationObject *syncObject) 
								: m_syncObject (syncObject) 
								{ m_syncObject->lock (); }

								~ExclusiveZone () 
								{ m_syncObject->unlock (); }
	};

protected:
	virtual void                setSynchronizationObject (SynchronizationObject *syncObject);

	std::vector<TestFailure *>  m_errors;
	std::vector<TestFailure *>  m_failures;
	int                         m_runTests;
	int                         m_runConditions;
	bool                        m_stop;
	SynchronizationObject       *m_syncObject;

};







#endif


