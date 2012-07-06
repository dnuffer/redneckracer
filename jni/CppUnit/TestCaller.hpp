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


#ifndef CPPUNIT_TESTCALLER_H
#define CPPUNIT_TESTCALLER_H

#include "TestCase.hpp"
/* 
 * A test caller provides access to a test case method 
 * on a test case class.  Test callers are useful when 
 * you want to run an individual test or add it to a 
 * suite.
 * 
 * Here is an example:
 * 
 * class MathTest : public TestCase {
 *         ...
 *     public:
 *         void         setUp ();
 *         void         tearDown ();
 *
 *         void         testAdd ();
 *         void         testSubtract ();
 * };
 *
 * Test *MathTest::suite () {
 *     TestSuite *suite = new TestSuite;
 *
 *     suite->addTest (new TestCaller<MathTest> ("testAdd", testAdd));
 *     return suite;
 * }
 *
 * You can use a TestCaller to bind any test method on a TestCase
 * class, as long as it returns accepts void and returns void.
 * 
 * See TestCase
 */


template <class Fixture> class TestCaller : public TestCase
{ 
   REFERENCEOBJECT (TestCaller)

   typedef void             (Fixture::*TestMethod)();
	
public:
							TestCaller (const char* name, TestMethod test)
							: TestCase (name), m_fixture (new Fixture (name)), m_test (test)
							{}
									 ~TestCaller()
									 {
										 delete m_fixture;
									 }

protected:
	void                    runTest () 
	{ m_fixture->setTestResult(m_testResult); (m_fixture->*m_test)(); m_fixture->setTestResult(NULL); }

	void                    setUp ()
							{ m_fixture->setUp (); }

	void                    tearDown ()
							{ m_fixture->tearDown (); }

private:
   Fixture*   m_fixture;
   TestMethod               m_test;

};



#endif
