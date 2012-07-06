/*
 * MoveActionTests.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "miniblocxx/DateTime.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/MoveAction.hpp"

using namespace engine;
using namespace blocxx;

// Constants
const static double positive = 5.0;
const static double negative = -5.0;
const TimeDuration delta(positive);

// Tests
AUTO_UNIT_TEST(MoveActionApplyForward)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction forward(0, positive);
	
	// Act
	forward.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(0,25));
}

AUTO_UNIT_TEST(MoveActionApplyBackward)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction backward(0, negative);
	
	// Act
	backward.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(0,-25));
}

AUTO_UNIT_TEST(MoveActionApplyLeft)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction lleft(negative, 0);
	
	// Act
	lleft.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-25,0));
}

AUTO_UNIT_TEST(MoveActionApplyRight)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction rright(positive, 0);
	
	// Act
	rright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(25,0));
}

AUTO_UNIT_TEST(MoveActionApplyForwardRight)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction fright(positive, positive);
	
	// Act
	fright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(25,25));
}

AUTO_UNIT_TEST(MoveActionApplyForwardLeft)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction fleft(negative, positive);
	
	// Act
	fleft.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-25,25));
}

AUTO_UNIT_TEST(MoveActionApplyBackwardLeft)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction bleft(negative, negative);
	
	// Act
	bleft.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-25,-25));
}

AUTO_UNIT_TEST(MoveActionApplyBackwardRight)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	MoveAction bright(positive, negative);
	
	// Act
	bright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(25,-25));
}
