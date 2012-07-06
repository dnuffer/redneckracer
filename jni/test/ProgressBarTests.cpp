/*
 * ProgressBarTests.cpp
 *
 *  Created on: July 6, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/ProgressBar.hpp"
#include "engine/Rectangle.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MockGLMock.h"

using namespace engine;

// Constants
float five = 5.0f;

// Tests
AUTO_UNIT_TEST(ProgressBarConnectVariable)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	
	// Act
	bar.connectVariable(variable);
	
	// Assert
	unitAssert(bar.getCurrentValue() == 0);
}

AUTO_UNIT_TEST(ProgressBarChangeVariableDirectly)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	variable = 50;// Changing the variable directly is acceptable
	
	// Assert
	unitAssert(bar.getCurrentValue() == 50);
	// Note that changes in this manner allow the variable outside 
	//  the range of the ProgressBar.
}

AUTO_UNIT_TEST(ProgressBarChangeVariableIndirectly)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar.setCurrentValue(five);
	
	// Assert
	unitAssert(bar.getCurrentValue() == 5);
}

AUTO_UNIT_TEST(ProgressBarSetBelowMin)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar.setCurrentValue(-five);
	
	// Assert
	unitAssert(bar.getCurrentValue() == 0);
}

AUTO_UNIT_TEST(ProgressBarPlusEquals)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar += 3;
	
	// Assert
	unitAssert(bar.getCurrentValue() == 3);
}

AUTO_UNIT_TEST(ProgressBarPlusEqualsMax)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar += five + five;
	
	// Assert
	unitAssert(bar.getCurrentValue() == 5);
}

AUTO_UNIT_TEST(ProgressBarMinusEquals)
{
	// Arrange
	float variable = 5;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar -= 3;
	
	// Assert
	unitAssert(bar.getCurrentValue() == 2);
}

AUTO_UNIT_TEST(ProgressBarMinusEqualsMin)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar -= 3;
	
	// Assert
	unitAssert(bar.getCurrentValue() == 0);
}

AUTO_UNIT_TEST(ProgressBarChangeConnectedVariable)
{
	// Arrange
	float variable = 0;
	ProgressBar bar(five, variable, five);
	bar.connectVariable(variable);
	
	// Act
	bar.connectVariable(five);
	
	// Assert
	unitAssert(bar.getCurrentValue() == 5);
}

AUTO_UNIT_TEST(ProgressBarDraw)
{
	// Arrange
	ProgressBar bar(five, five, five);
	Rectangle screen(0,five,five,0);
	
	using namespace gl;
	MockGLMock mock;
	glMock = &mock;
	
	using ::testing::InSequence;
	using ::testing::_;
	using ::testing::Mock;
	{
		InSequence dummy;
	
		EXPECT_CALL(mock, MatrixScope());
		EXPECT_CALL(mock, disableTexture2D());
		EXPECT_CALL(mock, color(4,GL_FLOAT,0,_));
		EXPECT_CALL(mock, vertex(2,GL_FLOAT,0,_));
		EXPECT_CALL(mock, translate(_,_,0));
		EXPECT_CALL(mock, setGLLineWidth(_));
		EXPECT_CALL(mock, drawArrays(GL_LINE_LOOP,0,4));
		EXPECT_CALL(mock, DestMatrixScope());
		EXPECT_CALL(mock, MatrixScope());
		EXPECT_CALL(mock, disableTexture2D());
		EXPECT_CALL(mock, color(4,GL_FLOAT,0,_));
		EXPECT_CALL(mock, vertex(2,GL_FLOAT,0,_));
		EXPECT_CALL(mock, translate(_,_,0));
		EXPECT_CALL(mock, drawArrays(GL_TRIANGLE_STRIP,0,4));
		EXPECT_CALL(mock, DestMatrixScope());
	}
	
	// Act
	bar.draw(screen);
	
	// Assert
	unitAssert(Mock::VerifyAndClearExpectations(&mock));
}
