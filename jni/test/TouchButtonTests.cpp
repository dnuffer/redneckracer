/*
 * TouchButtonTests.cpp
 *
 *  Created on: July 5, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Animation.hpp"
#include "engine/EngineFwd.hpp"
#include "engine/Rectangle.hpp"
#include "engine/TouchButton.hpp"
#include "engine/TexturedQuad.hpp"

using namespace engine;
using namespace blocxx;

vector<TexturedQuadPtr> createFrames()
{
	vector<TexturedQuadPtr> frames;// To go in the animation object
	for(int i = 0; i < 2; i++)
	{
		frames.push_back(new TexturedQuad(0, 1, 2, 3, new Texture(), 4, 5));
	}// Note the texture is invalid.  I don't know what if any effect this will have on testing.
	return frames;
}

// Constants
const static float positive = 5.0f;

// Tests
AUTO_UNIT_TEST(TouchButtonUpdateBoundingRect)
{	
	// Arrange
	TouchButton button(createFrames());
	unitAssert(button.getBoundingRect() == Rectangle(0,positive-1,positive,0));
	
	// Act
	button.setPosition(Point(0,0));
	
	// Assert
	unitAssert(button.getBoundingRect() == Rectangle(-2,2,positive/2,-positive/2));
}

AUTO_UNIT_TEST(TouchButtonCheckHit)
{	
	// Arrange
	TouchButton button(createFrames());
	
	// Act
	button.setPosition(Point(0,0));
	
	// Assert
	unitAssert(button.checkHit(0,0));
	unitAssert(button.checkHit(-2,-positive/2));
	unitAssert(button.checkHit(-2,positive/2));
	unitAssert(button.checkHit(2,positive/2));
	unitAssert(button.checkHit(2,-positive/2));
	unitAssert(!button.checkHit(positive,0));
	unitAssert(!button.checkHit(positive,positive));
	unitAssert(!button.checkHit(0,positive));
	unitAssert(!button.checkHit(-positive,positive));
	unitAssert(!button.checkHit(-positive,0));
	unitAssert(!button.checkHit(-positive,-positive));
	unitAssert(!button.checkHit(0,-positive));
	unitAssert(!button.checkHit(positive,-positive));
}
