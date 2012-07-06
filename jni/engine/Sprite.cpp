// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EngineConfig.hpp"
#include "Sprite.hpp"
#include "Animation.hpp"
#include "Rectangle.hpp"

namespace engine
{

Sprite::Sprite(const TexturedQuadPtr& texturedQuad, const string& name)
	: Boundable(texturedQuad->size())
	, spriteName(name)
	, texturedQuad_(texturedQuad)
{
}

Sprite::Sprite(const AnimationPtr& animation, const string& name)
	: Boundable(animation->currentSize())
	, spriteName(name)
	, animation_(animation)
{
}

Sprite::~Sprite() {}

void Sprite::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	Drawable::update(thisFrameStartTime, deltaTime);

	if (animation_)
		animation_->update(thisFrameStartTime, deltaTime);

	updateBoundingRect();
}

void Sprite::draw(const Rectangle& screen)
{
	using namespace gl;

	//LOGD("Sprite::draw() name: %s", name().c_str());
	Size size = this->size();
	Point position = this->getPositionRelativeToOrigin(screen);
	if( intersecting(screen, Rectangle::makeCenteredOn(position, size)) )
	{
		//		LOGD("It is really on the screen...");
		MatrixScope ms;
		
		if (animation_)
		{
			Point frameOffset = animation_->frameOffset();
			translate(frameOffset.x() + position.x(), frameOffset.y() + position.y(), 0);
		}
		else 
		{
			translate(position.x(), position.y(), 0);
		}

		
		// Rotation around the Z axis in gl is inverted. So invert the rotation to use standard planar trig angles.
		if (rotation() != 0.0)
			rotate(-1 * rotation(), 0.0f, 0.0f, 1.0f);
		
		scale(size.width(), size.height(), 1.0f);

		if (texturedQuad_)
		{
			texturedQuad_->draw(screen);
		}
		else if (animation_)
		{
			animation_->draw(screen);
		}
	}
}

string Sprite::name() const
{
	return spriteName;
}

void Sprite::updateBoundingRect()
{
	Point center;
	Size sz;
	if (texturedQuad_)
	{
		center = texturedQuad_->getRealBoundCenter();
		sz = texturedQuad_->getRealSize();
	}
	else
	{
		center = animation_->getCurrentRealBoundCenter();
		sz = animation_->currentRealSize();
	}
	Rectangle imageRectangle = Rectangle::makeCenteredOn(position(), size());
	Rectangle boundRectangle = Rectangle::makeCenteredOn(center, sz);
	boundRectangle.left += imageRectangle.left;
	boundRectangle.right += imageRectangle.left;
	boundRectangle.top += imageRectangle.bottom;
	boundRectangle.bottom += imageRectangle.bottom;

	setBoundingRect(boundRectangle);
}

bool Sprite::doesCollideWith(const Collidable& other) const
{
	return intersecting(this->getBoundingRect(), other.getBoundingRect());
}

void Sprite::handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	//	LOGASSERT(false, "Sprite::handleCollision() should never be called. Derived classes must override it.");
}

Size Sprite::size() const
{
	if (texturedQuad_)
		return texturedQuad_->size();
	else
		return animation_->currentSize();
}

void Sprite::animation(const AnimationPtr& newAnimation)
{
	//LOGASSERT(newAnimation != NULL, "Animation ==  NULL");

	bool vflip = flippedVertical(), hflip = flippedHorizontal();
	animation_ = newAnimation;
	animation_->setFlippedVertical(vflip);
	animation_->setFlippedHorizontal(hflip);
	texturedQuad_ = NULL;
}

void Sprite::texturedQuad(const TexturedQuadPtr& newTexturedQuad)
{
	bool vflip = flippedVertical(), hflip = flippedHorizontal();
	texturedQuad_ = newTexturedQuad;
	texturedQuad_->setFlippedVertical(vflip);
	texturedQuad_->setFlippedHorizontal(hflip);
	animation_ = NULL;
}

	bool Sprite::flippedHorizontal()
	{
		if (texturedQuad_)
			return texturedQuad_->flippedHorizontal();
		else
			return animation_->flippedHorizontal();
	}
	
	void Sprite::setFlippedHorizontal(bool x)
	{
		if (texturedQuad_)
			texturedQuad_->setFlippedHorizontal(x);
		else
			animation_->setFlippedHorizontal(x);
	}
	
	bool Sprite::flippedVertical()
	{
		if (texturedQuad_)
			return texturedQuad_->flippedVertical();
		else
			return animation_->flippedVertical();
	}
	
	void Sprite::setFlippedVertical(bool x)
	{
		if (texturedQuad_)
			texturedQuad_->setFlippedVertical(x);
		else
			animation_->setFlippedVertical(x);
	}

	void Sprite::setPosition(const Point& position)
	{
		Drawable::setPosition(position);
		updateBoundingRect();
	}
}
