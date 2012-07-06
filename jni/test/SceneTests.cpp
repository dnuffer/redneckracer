/*
 * SceneTests.cpp
 *
 *  Created on: July 1, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Animation.hpp"
#include "engine/Collidable.hpp"
#include "engine/Drawable.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/Scene.hpp"
#include "engine/Sprite.hpp"
#include "miniblocxx/DateTime.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MockGLMock.h"

using namespace engine;

vector<TexturedQuadPtr> createFrames()
{
	vector<TexturedQuadPtr> frames;// To go in the animation object
	for(int i = 0; i < 2; i++)
	{
		frames.push_back(new TexturedQuad(0, 1, 2, 3, new Texture(), 4, 4));
	}// Note the texture is invalid.  I don't know what if any effect this will have on testing.
	return frames;
}

const SpritePtr first = new Sprite(new Animation(createFrames()));
const SpritePtr second = new Sprite(new Animation(createFrames()));

AUTO_UNIT_TEST(SceneAddChildren)
{
	// Arrange
	Scene scene;
	DrawablePtr primero = new DrawableRectangle(1,1);
	DrawablePtr segundo = new DrawableRectangle(2,2);
	
	// Act
	scene.addChild(primero);
	
	// Assert
	unitAssert(scene.getChildren().size() == 1);
	unitAssert(scene.getCollidableChildren().size() == 0);
	
	// Act
	scene.addChild(segundo);
	
	// Assert
	unitAssert(scene.getChildren().size() == 2);
	unitAssert(scene.getCollidableChildren().size() == 0);
}

AUTO_UNIT_TEST(SceneRemoveChildren)
{
	// Arrange
	Scene scene;
	DrawablePtr primero = new DrawableRectangle(1,1);
	DrawablePtr segundo = new DrawableRectangle(2,2);
	DrawablePtr trecero = new DrawableRectangle(3,3);
	scene.addChild(primero);
	scene.addChild(segundo);
	
	// Act
	scene.removeChild(trecero);// Not in children
	
	// Assert
	unitAssert(scene.getChildren().size() == 2);
	unitAssert(scene.getCollidableChildren().size() == 0);
	
	// Act
	scene.removeChild(segundo);// In children
	
	// Assert
	unitAssert(scene.getChildren().size() == 1);
	unitAssert(scene.getCollidableChildren().size() == 0);
}

AUTO_UNIT_TEST(SceneRemoveAllChildren)
{
	// Arrange
	Scene scene;
	DrawablePtr primero = new DrawableRectangle(1,1);
	DrawablePtr segundo = new DrawableRectangle(2,2);
	DrawablePtr trecero = new DrawableRectangle(3,3);
	scene.addChild(primero);
	scene.addChild(segundo);
	
	// Act
	scene.removeAllChildren();
	
	// Assert
	unitAssert(scene.getChildren().size() == 0);
	unitAssert(scene.getCollidableChildren().size() == 0);
}

AUTO_UNIT_TEST(SceneAddCollidableChildren)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	SpritePtr second = new Sprite(new Animation(createFrames()));
	
	// Act
	scene.addCollidableChild(first);
	
	// Assert
	unitAssert(scene.getChildren().size() == 0);
	unitAssert(scene.getCollidableChildren().size() == 1);
	
	// Act
	scene.addCollidableChild(second);
	
	// Assert
	unitAssert(scene.getChildren().size() == 0);
	unitAssert(scene.getCollidableChildren().size() == 2);
}

AUTO_UNIT_TEST(SceneRemoveCollidableChild)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	scene.addCollidableChild(first);
	
	// Act
	scene.removeChild(first);// Cannot remove a collidable child.
	
	// Assert
	unitAssert(scene.getChildren().size() == 0);
	unitAssert(scene.getCollidableChildren().size() == 1);
}

AUTO_UNIT_TEST(SceneRemoveAllChildrenMixed)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	scene.addCollidableChild(first);
	DrawablePtr primero = new DrawableRectangle(1,1);
	scene.addChild(primero);
	
	// Act
	scene.removeAllChildren();// Removes all of both.
	
	// Assert
	unitAssert(scene.getChildren().size() == 0);
	unitAssert(scene.getCollidableChildren().size() == 0);
}

AUTO_UNIT_TEST(SceneDrawOne)
{
	using namespace gl;
	using ::testing::InSequence;
	using ::testing::_;
	using ::testing::Mock;
	using ::testing::AtLeast;
 
	// Arrange
	Scene scene;
	scene.addChild(first);
	Rectangle screen(0,5,5,0);
 
	MockGLMock mock;
	glMock = &mock;
 
 
	{
		InSequence dummy;
		
		EXPECT_CALL(mock, MatrixScope());
		EXPECT_CALL(mock, translate(first->getPositionRelativeToOrigin(screen).x(),
									first->getPositionRelativeToOrigin(screen).y(),0));
		EXPECT_CALL(mock, scale(first->size().width(),first->size().height(),1));
	}
 
	// A call to the TexturedQuad or Animation draw method occurs here.
	// If tested, we should do seperate tests for those paths.
 
	EXPECT_CALL(mock, DestMatrixScope())
		.Times(AtLeast(1));

	// Act
	scene.draw(screen);

	// Assert
	unitAssert(Mock::VerifyAndClearExpectations(&mock));
}

// TODO: Write a DrawTwo test.
// Warning: GoogleMock wasn't easy to write this for
//  (and thus the TODO) because it hasn't readily
//  accounted for a partly ordered sequence of sequences.

AUTO_UNIT_TEST(SceneCollisionsNonCollidableOne)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	SpritePtr second = new Sprite(new Animation(createFrames()));
	DateTime date;
	TimeDuration time(5.0f);
	
	scene.addChild(first);
	
	// Act
	scene.handleCollisions(date, time);
	
	// Assert
	unitAssert(first->getBoundingRect() == Rectangle(0,4,4,0));
}

AUTO_UNIT_TEST(SceneCollisionsNonCollidableTwo)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	SpritePtr second = new Sprite(new Animation(createFrames()));
	DateTime date;
	TimeDuration time(5.0f);
	
	scene.addChild(first);
	scene.update(date, time);
	scene.addChild(second);
	
	// Act
	scene.handleCollisions(date, time);
	
	// Assert
	unitAssert(first->getBoundingRect() == Rectangle(-2,2,2,-2));
}
	
AUTO_UNIT_TEST(SceneCollisionsCollidableOne)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	SpritePtr second = new Sprite(new Animation(createFrames()));
	DateTime date;
	TimeDuration time(5.0f);
	
	scene.addCollidableChild(first);
	
	// Act
	scene.handleCollisions(date, time);
	scene.update(date, time);// Doesn't change collidableChildren
	
	// Assert
	unitAssert(first->getBoundingRect() == Rectangle(0,4,4,0));
}

AUTO_UNIT_TEST(SceneCollisionsCollidableTwo)
{
	// Arrange
	Scene scene;
	SpritePtr first = new Sprite(new Animation(createFrames()));
	SpritePtr second = new Sprite(new Animation(createFrames()));
	DateTime date;
	TimeDuration time(5.0f);
	
	scene.addCollidableChild(first);
	scene.handleCollisions(date, time);
	scene.update(date, time);// Doesn't change collidableChildren
	scene.addCollidableChild(second);
	
	// Act
	scene.handleCollisions(date, time);
	
	// Assert
	unitAssert(first->getBoundingRect() == Rectangle(0,4,4,0));
	unitAssert(second->getBoundingRect() == Rectangle(0,4,4,0));
}
