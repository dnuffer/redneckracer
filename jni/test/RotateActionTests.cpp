/*
 * RotateActionTests.cpp
 *
 *  Created on: Jun 24, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/DrawableRectangle.hpp"
#include "engine/RotateAction.hpp"
#include "miniblocxx/DateTime.hpp"

using namespace engine;
using namespace blocxx;

// Constants
const static double positive = 5.0;
const static double negative = -5.0;
const TimeDuration delta(positive);

/* Quick Ref: RotateAction(rotationalSpeed, minAngle, maxAngle)*/

// Tests
AUTO_UNIT_TEST(RotateActionApplyNoChange)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	RotateAction noChange(0,0,positive*2);
	
	// Act
	noChange.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->rotation() == 0.0f);
}

AUTO_UNIT_TEST(RotateActionApplyForceMax)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	RotateAction forceMax(positive,positive,positive*2);
	
	// Act
	forceMax.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->rotation() == 10.0f);
}

AUTO_UNIT_TEST(RotateActionApplyForceMin)
{// Note: min{-10,-5} = -10
	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	RotateAction forceMin(negative,negative*2,negative);
	
	// Act
	forceMin.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->rotation() == -10.0f);
}

AUTO_UNIT_TEST(RotateActionApplyMiddle)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	RotateAction middle(-1.5f,negative*2,negative);
	
	// Act
	middle.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->rotation() == -7.5f);
}
