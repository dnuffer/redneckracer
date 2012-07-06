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


#ifndef CPPUNIT_TESTCASE_H
#define CPPUNIT_TESTCASE_H

#include "Guards.hpp"
#include "Test.hpp"
#include "CppUnitException.hpp"
#include <iostream>
#include <sstream>
#include <string.h>

class TestResult;



/*
 * A test case defines the fixture to run multiple tests. To define a test case
 * 1) implement a subclass of TestCase
 * 2) define instance variables that store the state of the fixture
 * 3) initialize the fixture state by overriding setUp
 * 4) clean-up after a test by overriding tearDown.
 *
 * Each test runs in its own fixture so there
 * can be no side effects among test runs.
 * Here is an example:
 *
 * class MathTest : public TestCase {
 *     protected: int m_value1;
 *     protected: int m_value2;
 *
 *     public: MathTest (string name)
 *                 : TestCase (name) {
 *     }
 *
 *     protected: void setUp () {
 *         m_value1 = 2;
 *         m_value2 = 3;
 *     }
 * }
 *
 *
 * For each test implement a method which interacts
 * with the fixture. Verify the expected results with assertions specified
 * by calling assert on the expression you want to test:
 *
 *    protected: void testAdd () {
 *        int result = value1 + value2;
 *        assert (result == 5);
 *    }
 *
 * Once the methods are defined you can run them. To do this, use
 * a TestCaller.
 *
 * Test *test = new TestCaller<MathTest>("testAdd", MathTest::testAdd);
 * test->run ();
 *
 *
 * The tests to be run can be collected into a TestSuite. CppUnit provides
 * different test runners which can run a test suite and collect the results.
 * The test runners expect a static method suite as the entry
 * point to get a test to run.
 *
 * public: static MathTest::suite () {
 *      TestSuite *suiteOfTests = new TestSuite;
 *      suiteOfTests->addTest(new TestCaller<MathTest>("testAdd", testAdd));
 *      suiteOfTests->addTest(new TestCaller<MathTest>("testDivideByZero", testDivideByZero));
 *      return suiteOfTests;
 *  }
 *
 * Note that the caller of suite assumes lifetime control
 * for the returned suite.
 *
 * see TestResult, TestSuite and TestCaller
 *
 */

enum BooleanOperatorType
{
	OP_GREATER,
	OP_GREATER_EQUAL,
	OP_LESS,
	OP_LESS_EQUAL,
	OP_EQUAL,
	OP_NOT_EQUAL
};

const char* getOperatorText(BooleanOperatorType op);

// These specialized comparison operators are used instead of std::equal_to and
// such because the standard versions require that there be a single templated
// type.  For the test cases it is useful to compare differing types, so long
// as a valid conversion exists.
template <BooleanOperatorType op>
struct ComparisonOperator
{
	template <typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return ComparisonOperator::ThisShouldNeverBeAccessed;
	}
};

template<> struct ComparisonOperator<OP_EQUAL>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 == t2;
	}
};

template<> struct ComparisonOperator<OP_NOT_EQUAL>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 != t2;
	}
};

template<> struct ComparisonOperator<OP_GREATER>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 > t2;
	}
};

template<> struct ComparisonOperator<OP_LESS>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 < t2;
	}
};

template<> struct ComparisonOperator<OP_GREATER_EQUAL>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 >= t2;
	}
};

template<> struct ComparisonOperator<OP_LESS_EQUAL>
{
	template<typename T1, typename T2>
	bool operator()(const T1& t1, const T2& t2)
	{
		return t1 <= t2;
	}
};

class TestCase : public Test
{
	REFERENCEOBJECT (TestCase)

public:
	TestCase(const char* Name);
	~TestCase();

	virtual void run(TestResult *result);
	virtual TestResult *run();

	template <class T>
	bool runFuncAndCatchErrors( T func, const char* msg, TestResult* result );

	virtual int countTestCases();
	const char* name();
	virtual const char* toString();

	virtual void setUp();
	virtual void tearDown();

	// Evil, but we need to propogate the TestResult down...  The
	// design of this testing framework seems a little lacking.
	void setTestResult(TestResult* foo) { m_testResult = foo; }

protected:
	virtual void runTest();

	TestResult *defaultResult();
	void assertImplementation(bool condition, const char* conditionExpression, long lineNumber, const char* fileName);
	void assertEquals(long expected, long actual, long lineNumber, const char* fileName);
	void assertEquals(double expected, double actual, double delta, long lineNumber, const char* fileName);
	template <typename T1, typename T2>
	void assertEquals(const T1& expected, const T2& actual, long lineNumber, const char* fileName)
	{
		if (expected != actual)
		{
			std::ostringstream oss;
			oss << "expected: \"" << expected << "\" but was: \"" << actual << "\"";
			assertImplementation(false, oss.str().c_str(), lineNumber, fileName);
		}
	}

	template <typename T1, typename T2>
	void assertNotEquals(const T1& expected, const T2& actual, long lineNumber, const char* fileName)
	{
		if (expected == actual)
		{
			std::ostringstream oss;
			oss << "expected not equal to: \"" << expected << "\" but it was.";
			assertImplementation(false, oss.str().c_str(), lineNumber, fileName);
		}
	}

	template <BooleanOperatorType op, typename T1, typename T2>
	void assertBooleanOperator(const T1& t1, const T2& t2, const char* exp1, const char* exp2, long lineNumber, const char* fileName)
	{
		const char* optext = getOperatorText(op);

		if( !ComparisonOperator<op>()(t1, t2) )
		{
			std::ostringstream oss;
			oss << "expected: " << exp1 << " " << optext << " " << exp2 << " (" << t1 << " " << optext << " " << t2 << ") but it was not.";
			assertImplementation(false, oss.str().c_str(), lineNumber, fileName);
		}
	}

	const char* notEqualsMessage(long expected, long actual);
	const char* notEqualsMessage(double expected, double actual);

private:
	const char*   m_name;

protected:
	TestResult* m_testResult;

};




// A set of macros which allow us to get the line number
// and file name at the point of an error.
// Just goes to show that preprocessors do have some
// redeeming qualities.

#define CPPUNIT_SOURCEANNOTATION

#ifdef CPPUNIT_SOURCEANNOTATION

#undef FlagTestCondition
#define FlagTestCondition() do { m_testResult->testCondition(this); } while(0)

#undef unitAssert
#define unitAssert(condition) \
	FlagTestCondition(); \
	try \
	{ \
		this->assertImplementation ((condition),(#condition), __LINE__, __FILE__); \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch( const std::exception& e ) \
	{ \
		throw CppUnitException( e.what(), __LINE__, __FILE__); \
	} \
	catch (...) \
	{ \
		throw CppUnitException((#condition), __LINE__, __FILE__); \
	}

#undef unitAssertFail
#define unitAssertFail(condition) \
	FlagTestCondition(); \
	try \
	{ \
		this->assertImplementation ((!(condition)),("!("#condition")"),	__LINE__, __FILE__); \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch( const std::exception& e ) \
	{ \
		throw CppUnitException( e.what(), __LINE__, __FILE__); \
	} \
	catch (...) \
	{ \
		throw CppUnitException((#condition), __LINE__, __FILE__); \
	}

#undef unitAssertThrows
#define unitAssertThrows(condition) \
	FlagTestCondition(); \
	try \
	{ \
		condition; \
		this->assertImplementation (false, #condition, __LINE__, __FILE__); \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch (...) \
	{ \
	}

#undef unitAssertThrowsEx
#define unitAssertThrowsEx(condition, exceptionType) \
	FlagTestCondition(); \
	try \
	{ \
		condition; \
		this->assertImplementation(false, #condition, __LINE__, __FILE__); \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch (const exceptionType& e) \
	{ \
	} \
	catch (...) \
	{ \
		this->assertImplementation(false, #condition,__LINE__, __FILE__); \
	}

#undef unitAssertThrowsExWhat
#define unitAssertThrowsExWhat(condition, exceptionType, whatMsg) \
	FlagTestCondition(); \
	try \
	{ \
		condition; \
		this->assertImplementation(false, #condition, __LINE__, __FILE__); \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch (const exceptionType& e) \
	{ \
		this->assertEquals((whatMsg), e.what(), __LINE__, __FILE__); \
	} \
	catch (...) \
	{ \
		this->assertImplementation(false, #condition, __LINE__, __FILE__); \
	}

#undef unitAssertNoThrow
#define unitAssertNoThrow(condition) \
	FlagTestCondition(); \
	try \
	{ \
		condition; \
	} \
	catch (const CppUnitException& e) \
	{ \
		throw e; \
	} \
	catch (const std::exception& e) \
	{ \
		std::cout << "Caught exception " << e.what() << std::endl; \
		this->assertImplementation( false, #condition, __LINE__, __FILE__); \
	} \
	catch (...) \
	{ \
		this->assertImplementation( false, #condition, __LINE__, __FILE__); \
	}

#else

	#undef unitAssert
	#define unitAssert(condition) \
	FlagTestCondition(); \
	(this->assertImplementation ((condition),"", __LINE__, __FILE__))

#endif


// Macros for primitive value comparisons
#define unitAssertDoublesEqual(expected,actual,delta) \
	FlagTestCondition(); \
	(this->assertEquals ((expected), \
		(actual),(delta),__LINE__,__FILE__))

#define unitAssertOperator(v1,v2,op) \
	FlagTestCondition(); \
	(this->assertBooleanOperator<op>((v1),(v2), #v1, #v2, __LINE__, __FILE__))

#define unitAssertLongsEqual(expected,actual) unitAssertOperator(expected, actual, OP_EQUAL)
#define unitAssertEquals(expected,actual) unitAssertOperator(expected, actual, OP_EQUAL)
#define unitAssertNotEquals(expected,actual) unitAssertOperator(expected, actual, OP_NOT_EQUAL)

#define unitAssertLess(v1,v2) unitAssertOperator(v1,v2,OP_LESS)
#define unitAssertGreater(v1,v2)	unitAssertOperator(v1,v2,OP_GREATER)
#define unitAssertLessOrEqual(v1,v2) unitAssertOperator(v1,v2,OP_LESS_EQUAL)
#define unitAssertGreaterOrEqual(v1,v2) unitAssertOperator(v1,v2,OP_GREATER_EQUAL)

#define unitAssertEqualsOneOf_1(text, actual, expected1) \
	do \
	{ \
		if( (actual) != (expected1) ) \
		{ \
			std::ostringstream oss; \
			oss << "expected: \"" #actual "\" (" << (actual) <<  ") to be in the set (" text "), but it was not."; \
			this->assertImplementation(false, oss.str().c_str(), __LINE__, __FILE__); \
		} \
	} \
	while(false)

#define unitAssertEqualsOneOf_2(text, actual, expected1, ...) \
	do \
	{ \
		if( (actual) != (expected1) ) \
		{ \
			unitAssertEqualsOneOf_1(text, actual, __VA_ARGS__); \
		} \
	} \
	while(false)

#define unitAssertEqualsOneOf_3(text, actual, expected1, ...) \
	do \
	{ \
		if( (actual) != (expected1) ) \
		{ \
			unitAssertEqualsOneOf_2(text, actual, __VA_ARGS__); \
		} \
	} \
	while(false)

#define unitAssertEqualsOneOf_4(text, actual, expected1, ...) \
	do \
	{ \
		if( (actual) != (expected1) ) \
		{ \
			unitAssertEqualsOneOf_3(text, actual, __VA_ARGS__); \
		} \
	} \
	while(false)

#define unitAssertEqualsOneOf_5(text, actual, expected1, ...) \
	do \
	{ \
		if( (actual) != (expected1) ) \
		{ \
			unitAssertEqualsOneOf_4(text, actual, __VA_ARGS__); \
		} \
	} \
	while(false)

#define assertEqualsArgTextHelper(...) #__VA_ARGS__
#define unitAssertEqualsOneOfHelper_1(actual, A) unitAssertEqualsOneOf_1(assertEqualsArgTextHelper(A), actual, A)
#define unitAssertEqualsOneOfHelper_2(actual, A, B) unitAssertEqualsOneOf_2(assertEqualsArgTextHelper(A, B), actual, A, B)
#define unitAssertEqualsOneOfHelper_3(actual, A, B, C) unitAssertEqualsOneOf_3(assertEqualsArgTextHelper(A, B, C), actual, A, B, C)
#define unitAssertEqualsOneOfHelper_4(actual, A, B, C, D) unitAssertEqualsOneOf_4(assertEqualsArgTextHelper(A, B, C, D), actual, A, B, C, D)
#define unitAssertEqualsOneOfHelper_5(actual, A, B, C, D, E) unitAssertEqualsOneOf_5(assertEqualsArgTextHelper(A, B, C, D, E), actual, A, B, C, D, E)

// This is a macro for testing if an actual value is in a list of expected values.
// Use this like: unitAssertEqualsOneOf(2)('a', 1, 65)
#define unitAssertEqualsOneOf(count) FlagTestCondition(); unitAssertEqualsOneOfHelper_##count

#endif
