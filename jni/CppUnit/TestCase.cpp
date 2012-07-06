/*******************************************************************************
* Copyright (C) 2001-2009 Quest Software, Inc. All rights reserved.
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

#include "miniblocxx/BLOCXX_config.h"
#include "TestCase.hpp"
#include "TestResult.hpp"
#include <stdio.h>
#include <typeinfo>
#include <signal.h>
#include <setjmp.h>
#include <string>
#include <cstring>  // for strncpy
#include <math.h>

#include "miniblocxx/Exception.hpp"

using std::string;

// Constructs a test case
TestCase::TestCase (const char* name)
		: m_name (name)
		, m_testResult(NULL)
{}


// Destructs a test case

TestCase::~TestCase ()
{}


// Returns a count of all the tests executed

int TestCase::countTestCases ()
{
	return 1;
}


// Returns the name of the test case
const char* TestCase::name ()
{
	return m_name;
}


// A hook for fixture set up
void TestCase::setUp ()
{}


// A hook for fixture tear down

void TestCase::tearDown ()
{}


// Returns the name of the test case instance

const char* TestCase::toString ()
{
	static char buff[128];
	snprintf( buff, sizeof(buff), "%s", name ());
	return buff;
}

const char* getOperatorText(BooleanOperatorType op)
{
	switch(op)
	{
	case OP_GREATER:
		return ">";
	case OP_GREATER_EQUAL:
		return ">=";
	case OP_LESS:
		return "<";
	case OP_LESS_EQUAL:
		return "<=";
	case OP_EQUAL:
		return "==";
	case OP_NOT_EQUAL:
		return "!=";
		// No default is provided so the compiler can emit a warning if the case is not complete.
	}
	return "ERROR";
}


// Create a default TestResult
TestResult *TestCase::defaultResult ()
{
	return new TestResult;
}


// Check for a failed general assertion
void TestCase::assertImplementation (bool          condition,
									 const char*   conditionExpression,
									 long          lineNumber,
									 const char*   fileName)
{
	if (!condition)
		throw CppUnitException (conditionExpression, lineNumber, fileName);
}


// Check for a failed equality assertion
void TestCase::assertEquals (long        expected,
							 long        actual,
							 long        lineNumber,
							 const char* fileName)
{
	if (expected != actual)
		assertImplementation (false, notEqualsMessage(expected, actual), lineNumber, fileName);
}


// Check for a failed equality assertion
void TestCase::assertEquals (double        expected,
							 double        actual,
							 double        delta,
							 long          lineNumber,
							 const char*   fileName)
{
	if (fabs (expected - actual) > delta)
		assertImplementation (false, notEqualsMessage(expected, actual), lineNumber, fileName);

}

static jmp_buf unitTestJumpBuffer;

extern "C" {
static void unitTestSignalHandler(int sig)
{
	longjmp(unitTestJumpBuffer, sig);
}
}

void handleSignal(int sigtype, const char* where, TestResult* result, TestCase* tc)
{
	string report = std::string("Failure in ") + where + tc->name();

	switch (sigtype)
	{
#if !defined(BLOCXX_WIN32) && !defined(OW_NETWARE)
	case SIGTRAP:
		report += " - SIGTRAP (perhaps integer divide by zero)";
		break;
	case SIGBUS:
#endif
	case SIGSEGV:
		report += " - memory access violation";
		break;
	case SIGFPE:
		report += " - SIGFPE (arithmetic exception)";
		break;
	case SIGABRT:
		report += " - SIGABRT abort() called or assert failed";
		break;
	default:
		report += " - unrecognized signal";
	}

	const int BUFF_SIZE = 1024;
	static char buff[BUFF_SIZE];
	strncpy( buff, report.c_str(), BUFF_SIZE );
	buff[BUFF_SIZE-1] = 0;
	result->addFailure( tc, new CppUnitException( buff, -1, "<unknown>" ) );

}

extern "C" {
typedef void (*sighandler_t)(int);
}

class SignalScope
{
	public:
		SignalScope( int sig, sighandler_t handler )
			: m_sig( sig ), m_oldHandler( 0 )
			{
				m_oldHandler = signal( m_sig, handler );
			}

		~SignalScope()
		{
			signal( m_sig, m_oldHandler );
		}

	private:
		SignalScope(const SignalScope&);
		const SignalScope& operator=(const SignalScope&);
		int m_sig;
		sighandler_t m_oldHandler;
};

template <class T>
bool TestCase::runFuncAndCatchErrors( T func, const char* msg, TestResult* result )
{
	int sigtype;
	SignalScope(SIGFPE, &unitTestSignalHandler);
#if !defined(BLOCXX_WIN32) && !defined(OW_NETWARE)
	SignalScope(SIGTRAP, &unitTestSignalHandler);
	SignalScope(SIGBUS, &unitTestSignalHandler);
#endif
	SignalScope(SIGSEGV, &unitTestSignalHandler);
	SignalScope(SIGABRT, &unitTestSignalHandler);

	try
	{
		sigtype = setjmp(unitTestJumpBuffer);
		if ( sigtype == 0 )
		{
			(this->*func)();
			return true;
		}
		else
		{
			handleSignal( sigtype, msg, result, this );
		}
	}
	catch (const CppUnitException& e)
	{
		CppUnitException *copy = new CppUnitException (e);
		result->addFailure (this, copy);
	}
	// Added special case to catch blocxx exceptions and properly report them.
	catch (const blocxx::Exception& e)
	{
		result->addError (this, new CppUnitException (std::string(std::string(e.type()) + ": " + std::string(e.what())).c_str(), e.getLine(), e.getFile()));
	}
	catch (const std::exception& e)
	{
		result->addError (this, new CppUnitException (e.what(), -1, "<unknown>"));

	}
	catch (...)
	{
		result->addError (this, new CppUnitException ("unknown exception", -1, "<unknown>") );

	}
	return false;
}


// Run the test and catch any exceptions that are triggered by it
void TestCase::run (TestResult *result)
{
	result->startTest (this);

	m_testResult = result;
	if (runFuncAndCatchErrors( &TestCase::setUp, "initialization of ", result ))
		if (runFuncAndCatchErrors( &TestCase::runTest, "", result ))
			runFuncAndCatchErrors( &TestCase::tearDown, "destruction of ", result );

	m_testResult = NULL;

	result->endTest (this);

}


// A default run method
TestResult *TestCase::run ()
{
	TestResult *result = defaultResult ();

	run (result);
	return result;

}


// All the work for runTest is deferred to subclasses
void TestCase::runTest ()
{}


// Build a message about a failed equality check

const char* TestCase::notEqualsMessage (long expected, long actual)
{
	static char buff[128];
	snprintf( buff, sizeof(buff), "expected: %ld but was: %ld", expected, actual );
	return buff;
}


// Build a message about a failed equality check
const char* TestCase::notEqualsMessage (double expected, double actual)
{
	static char buff[128];
	snprintf( buff, sizeof(buff), "expected: %f but was: %f", expected, actual);
	return buff;
}

