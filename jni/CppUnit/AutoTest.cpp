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

#include "AutoTest.hpp"
#include "TestRunner.hpp"

/**
 *@author Kevin Harris
 */

namespace TestNamespace
{
	namespace // anonymous
	{
		struct AutoTestList
		{
			const char* name;
			Test* test;
			AutoTestList* next;
		};

		AutoTestList g_AutoTests = { NULL, NULL, NULL };

		// Stick the first test into the given test runner.
		void popTest(TestRunner& runner)
		{
			AutoTestList* tl = g_AutoTests.next;

			if( tl )
			{
				runner.addTest( tl->name, tl->test );
				g_AutoTests.next = tl->next;
				delete tl;
			}
		}

		bool haveMoreTests()
		{
			return g_AutoTests.next != NULL;
		}

	} // end anonymous namespace


	void AddAutoTest(const char* name, Test* t)
	{
		AutoTestList* tl = new AutoTestList;
		tl->name = name;
		tl->test = t;
		tl->next = NULL;

		// Add to the end of the list.
		AutoTestList* current = &g_AutoTests;
		for(; current->next; current = current->next)
		{
		}
		current->next = tl;
	}

	int unitTestAutoMainImpl(int argc, const char** argv)
	{
		TestRunner runner;

		// Pull all the tests out of the global object and shove them in the TestRunner.
		while( haveMoreTests() )
		{
			popTest(runner);
		}

		int failureCount = 0;

		if ( argc < 2 || ( argc == 2 && (::strcmp("all", argv[1]) == 0) ) )
		{
			failureCount = runner.runAll();
		}
		else
		{
			failureCount = runner.run( argv[1] );
		}

		if( failureCount > 0 )
		{
			return 1;
		}
		return 0;
	}
} // end namespace TestNamespace
