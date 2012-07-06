/*
 * DrawableTests.cpp
 *
 *  Created on: June 23, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "miniblocxx/DateTime.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/MoveAction.hpp"
#include "engine/Drawable.hpp"

using namespace engine;
using namespace blocxx;

// Constants
const static double positive = 5.0;
const static double negative = -5.0;

const Rectangle screen(1, 3, 3, 1);

const TimeDuration delta(positive);

AUTO_UNIT_TEST(DrawableGetPositionRelativeToOrigin)
{	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);

	// Tests
	drawn->setPosition(Point(2,2));
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(2,2));
	
	drawn->setPositionInterpretation(Drawable::E_ORIGIN);
	// This should be the default, so nothing should have changed
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(2,2));
}

AUTO_UNIT_TEST(DrawableGetPositionRelativeToOriginScreen)
{// This function has a rather misleading name...I don't think it works as expected.
	
	// Arrange
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	drawn->setPosition(Point(0,0));
	
	// Tests
	drawn->setPositionInterpretation(Drawable::E_SCREEN);
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(2,2));
	
	drawn->setPosition(Point(2,2));
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(4,4));
}

AUTO_UNIT_TEST(DrawableAddActionNoUpdate)
{
	// Test object
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	
	// Tests
	drawn->addAction(new MoveAction(0, positive));
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(0,0));
}

AUTO_UNIT_TEST(DrawableAddActionUpdate)
{
	// Test object
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	
	// Tests
	drawn->addAction(new MoveAction(0, positive));
	drawn->update(DateTime(), delta);
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(0,25));
}

AUTO_UNIT_TEST(DrawableAddActionTwiceAndUpdate)
{
	// Test object
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	
	// Tests
	drawn->addAction(new MoveAction(0, negative));
	drawn->addAction(new MoveAction(0, negative));
	drawn->update(DateTime(), delta);
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(0,-50));
}

AUTO_UNIT_TEST(DrawableSetAction)
{
	// Test object
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	
	// Tests
	drawn->addAction(new MoveAction(0, negative));
	drawn->setAction(new MoveAction(0, positive));
	drawn->update(DateTime(), delta);
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(0,25));
}

AUTO_UNIT_TEST(DrawableClearActions)
{
	// Test object
	DrawablePtr drawn = new DrawableRectangle(positive, positive);
	
	// Tests
	drawn->addAction(new MoveAction(0, positive));
	drawn->setAction(new MoveAction(0, positive));
	drawn->clearActions();
	drawn->update(DateTime(), delta);
	unitAssert(drawn->getPositionRelativeToOrigin(screen) == Point(0,0));
}
