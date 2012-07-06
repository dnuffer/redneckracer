/*******************************************************************************
* Copyright (C) 2008-2009 Quest Software, Inc. All rights reserved.
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

#ifndef CPPUNIT_AUTO_TEST_H
#define CPPUNIT_AUTO_TEST_H

#include "Test.hpp"
#include "TestCase.hpp"
#include "TestResult.hpp"

/**
 *@author Kevin Harris
 */


// The macro to create an auto unit test.  When using this macro, you must
// define the body of your test.  This can be any number of times within a
// single .cpp file.
// Example:
//   AUTO_UNIT_TEST(testme)
//   {
//     unitAssert(something);
//   }
//   AUTO_UNIT_TEST(testme2)
//   {
//     unitAssert(somethingelse);
//   }
//
// If you define PROVIDE_AUTO_TEST_MAIN before including this header file,
// there is nothing else to do to produce the code for a complete test
// executable.
#define AUTO_UNIT_TEST(name) \
class AutoTest##name : public TestCase \
{ \
public: \
	AutoTest##name() \
		: TestCase(#name) \
		, TEST_COMPONENT_NAME("AutoTest." # name) \
	{ \
	} \
	~AutoTest##name() { } \
	const char* const TEST_COMPONENT_NAME; \
protected: \
	virtual void runTest(); \
}; \
TestNamespace::AutoTestCreator<AutoTest##name> AutoTestCreator_##name = TestNamespace::AutoTestCreator<AutoTest##name>(#name); \
void AutoTest##name::runTest()

// This one is for the easy conversion of existing test suites into an auto
// test.  This also allows mixing of existing tests with new AUTO_UNIT_TESTs.
#define AUTO_UNIT_TEST_SUITE(name) \
TestNamespace::AutoTestSuiteCreator<name> AutoTestSuiteCreator_##name = TestNamespace::AutoTestSuiteCreator<name>(#name)

#define AUTO_UNIT_TEST_SUITE_NAMED(classname, name) \
TestNamespace::AutoTestSuiteCreator<classname> AutoTestSuiteCreator_##classname = TestNamespace::AutoTestSuiteCreator<classname>(name)

namespace TestNamespace
{
	// Add a unit test (or test suite).  The object passed must be deleteable.
	void AddAutoTest(const char* name, Test* t);

	// An implementation of main().
	int unitTestAutoMainImpl(int argc, const char** argv);

	// A helper class to automatically add an auto unit test at construction
	// time.
	template <typename TestT>
	struct AutoTestCreator
	{
		AutoTestCreator(const char* name)
		{
			AddAutoTest(name, new TestT());
		}
	};

	// This one is probably very specific to OpenWBEM/BloCxx.
	template <typename TestClass>
	struct AutoTestSuiteCreator
	{
		AutoTestSuiteCreator(const char* name)
		{
			AddAutoTest(name, TestClass::suite());
		}
	};
}

// Define this if you want a main function provided for you.  Normally
// you want this.
#if defined(PROVIDE_AUTO_TEST_MAIN)
int main(int argc, const char** argv)
{
	return TestNamespace::unitTestAutoMainImpl(argc, argv);
}
#endif

#endif
