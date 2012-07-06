/*
 * SpriteTests.cpp
 *
 *  Created on: July 4, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Animation.hpp"
#include "engine/EngineFwd.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Sprite.hpp"
#include "engine/TexturedQuad.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MockGLMock.h"

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

const AnimationPtr anime = new Animation(createFrames());
const TexturedQuadPtr texture = new TexturedQuad(0,0,positive,positive,new Texture(),positive,positive);

// Tests
AUTO_UNIT_TEST(SpriteUpdateBoundingRectTexturedQuad)
{
	// Arrange
	Sprite sprite(anime);
	unitAssert(sprite.getBoundingRect() == Rectangle(0,positive-1,positive,0));
	
	// Act
	sprite.texturedQuad(texture);
	sprite.updateBoundingRect();
	
	// Assert
	unitAssert(sprite.getBoundingRect() == Rectangle(-positive/2,positive/2,positive/2,-positive/2));
}// Note that the first assert differs from the last.  Method updateBoundingRect has a centering effect.

AUTO_UNIT_TEST(SpriteUpdateBoundingRectAnimation)
{
	// Arrange
	Sprite sprite(texture);
	unitAssert(sprite.getBoundingRect() == Rectangle(0,positive,positive,0));
	
	// Act
	sprite.animation(anime);
	sprite.updateBoundingRect();
	
	// Assert
	unitAssert(sprite.getBoundingRect() == Rectangle(-2,2,positive/2,-positive/2));
}

AUTO_UNIT_TEST(SpriteDrawAnimation)
{	
	// Arrange
	Sprite sprite(anime);
	Rectangle screen(0,5,5,0);
	
	using namespace gl;
	MockGLMock mock;
	glMock = &mock;
	
	using ::testing::InSequence;
	using ::testing::_;
	using ::testing::Mock;
	using ::testing::AtLeast;
	{
		InSequence dummy;
		 
		EXPECT_CALL(mock, MatrixScope());
		EXPECT_CALL(mock, translate(sprite.getPositionRelativeToOrigin(screen).x(),
									sprite.getPositionRelativeToOrigin(screen).y(),0));
		EXPECT_CALL(mock, scale(sprite.size().width(),sprite.size().height(),1));
	}
	 
	// A call to the TexturedQuad draw method or Animation draw method (which just drops 
	//  to the right TexturedQuad and calls TexturedQuad::draw) occurs here.
	
	EXPECT_CALL(mock, DestMatrixScope())
		.Times(AtLeast(1));
	
	// Act
	sprite.draw(screen);
	
	// Assert
	unitAssert(Mock::VerifyAndClearExpectations(&mock));
}
