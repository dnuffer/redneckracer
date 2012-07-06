/*
 * AnimationTests.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Animation.hpp"
#include "engine/Texture.hpp"
#include "miniblocxx/DateTime.hpp"

using namespace engine;
using namespace blocxx;

vector<TexturedQuadPtr> createFrames()
{
	vector<TexturedQuadPtr> frames;// To go in the animation object
	for(int i = 0; i < 3; i++)
	{
		frames.push_back(new TexturedQuad(0, 1, 2, 3, new Texture(), 4, 5));
	}// Note the texture is invalid.  I don't know what if any effect this will have on testing.
	return frames;
}

AUTO_UNIT_TEST(AnimationFlippedVertical)
{
	// Variable
	Animation animation(createFrames());
	
	// Tests
	unitAssert(!animation.flippedVertical());
	
	animation.setFlippedVertical(true);
	unitAssert(animation.flippedVertical());
	
	animation.setFlippedVertical(false);
	unitAssert(!animation.flippedVertical());
}

AUTO_UNIT_TEST(AnimationFlippedHorizontal)
{
	// Variable
	Animation animation(createFrames());
	
	// Tests
	unitAssert(!animation.flippedHorizontal());
	
	animation.setFlippedHorizontal(true);
	unitAssert(animation.flippedHorizontal());
	
	animation.setFlippedHorizontal(false);
	unitAssert(!animation.flippedHorizontal());
}

AUTO_UNIT_TEST(AnimationUpdate)
{
	// Variables
	Animation animation(createFrames());
	
	// Tests
	unitAssert(animation.getCurrentFrame() == 0);
	unitAssert(animation.getElapsedTime().realSeconds() == 0.0);
	
	animation.update(DateTime(), TimeDuration(0.2f));
	animation.update(DateTime(), TimeDuration(0.2f));
	animation.update(DateTime(), TimeDuration(0.2f));
	
	// Assert
	unitAssert(animation.getCurrentFrame() == 2);// E_PLAY_ONCE
	unitAssert(animation.getElapsedTime().realSeconds() == 0.6);
}
	
AUTO_UNIT_TEST(AnimationResetLoop)
{
	// Arrange
	Animation animation(createFrames());
	animation.reset(Animation::E_LOOP);
	
	// Tests
	animation.update(DateTime(), TimeDuration(1.0f/3.0f));
	unitAssert(animation.getCurrentFrame() == 1);
	
	animation.update(DateTime(), TimeDuration(1.0f/3.0f));
	unitAssert(animation.getCurrentFrame() == 0);
	
	animation.update(DateTime(), TimeDuration(1.0f/3.0f));
	unitAssert(animation.getCurrentFrame() == 2);
	
	animation.update(DateTime(), TimeDuration(1.0f/3.0f));
	unitAssert(animation.getCurrentFrame() == 1);
}

AUTO_UNIT_TEST(AnimationResetPlayOnce)
{
	// Arrange
	Animation animation(createFrames());
	
	// Tests
	animation.reset(Animation::E_PLAY_ONCE);
	animation.update(DateTime(), TimeDuration(1.0f));
	unitAssert(animation.getCurrentFrame() == 2);
	unitAssert(animation.getElapsedTime().realSeconds() == 1);
}
