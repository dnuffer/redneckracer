/*
 * AccelerateActionTests.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "miniblocxx/DateTime.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/AccelerateAction.hpp"

using namespace engine;
using namespace blocxx;

// Constants
const static double positive = 5.0;
const static double negative = -5.0;

const TimeDuration delta(positive);

AUTO_UNIT_TEST(AccelerateActionApplyForward)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction forward(0, positive);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));

	// Act
	forward.apply(drawn, DateTime(), delta);

	// Assert
	unitAssert(drawn->position() == Point(0,62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyBackward)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction backward(0, negative);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act
	backward.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(0,-62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyLeft)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction lleft(negative, 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));

	// Act
	lleft.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-62.5,0));
}

AUTO_UNIT_TEST(AccelerateActionApplyRight)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction rright(positive, 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act
	rright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(62.5,0));
}

AUTO_UNIT_TEST(AccelerateActionApplyForwardRight)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction fright(positive, positive);// Forward right
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));

	// Act
	fright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(62.5,62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyForwardLeft)
{
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction fleft(negative, positive);// Forward left
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act
	fleft.apply(drawn, DateTime(), delta);
	unitAssert(drawn->position() == Point(-62.5,62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyBackwardLeft)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction bleft(negative, negative);// Backward left
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act
	bleft.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-62.5,-62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyBackwardRight)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction bright(positive, negative);// Backward right
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act
	bright.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(62.5,-62.5));
}

AUTO_UNIT_TEST(AccelerateActionApplyVerticalMaxForward)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction forwardMax(0, positive, /* Max speed */ 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));

	// Act with useMaxSpeed = true
	forwardMax.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(0,0));
}

AUTO_UNIT_TEST(AccelerateActionApplyVerticalMaxBackward)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction backwardMax(0, negative, /* Max speed */ 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Act with useMaxSpeed = true
	backwardMax.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(0,-62.5));
	// Note::Vertical Max backwards will always execute the 
	//  first if in AccelerateAction::apply because
	//  current speed will be negative going backwards.
}

AUTO_UNIT_TEST(AccelerateActionApplyHorizontalMaxLeft)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction leftMax(negative, 0, /* Max speed */ 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));

	// Horizontals with useMaxSpeed = true
	//  useMaxSpeed should not affect x position
	
	// Act
	leftMax.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(-62.5,0));
}

AUTO_UNIT_TEST(AccelerateActionApplyHorizontalMaxRight)
{
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	AccelerateAction rightMax(positive, 0, /* Max speed */ 0);
	
	// Assert that we start at the origin
	unitAssert(drawn->position() == Point(0,0));
	
	// Horizontals with useMaxSpeed = true
	//  useMaxSpeed should not affect x position
	
	// Act
	rightMax.apply(drawn, DateTime(), delta);
	
	// Assert
	unitAssert(drawn->position() == Point(62.5,0));
}
