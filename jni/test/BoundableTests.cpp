/*
 * BoundableTests.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Boundable.cpp"

using namespace engine;
using namespace graphlib;

AUTO_UNIT_TEST(BoundableDefaultConstructor)
{
	// Arrange
	Boundable bounded;
	
	// Assert
	unitAssert(bounded.getBoundingRect() == Rectangle(0,0,0,0));
}

AUTO_UNIT_TEST(BoundableExplicitConstructors)
{
	// Arrange
	Boundable bound(Rectangle(0,1,1,0));
	Boundable bounds(Size(1,1));
	
	// Assert
	unitAssert(bound.getBoundingRect() == bounds.getBoundingRect());
}

AUTO_UNIT_TEST(BoundableSetBoundingRect)
{
	// Arrange
	Boundable bound;
	Boundable bounds(Size(1,1));
	
	// Act
	bound.setBoundingRect(Rectangle(0,1,1,0));
	
	// Assert
	unitAssert(bound.getBoundingRect() == bounds.getBoundingRect());
}
