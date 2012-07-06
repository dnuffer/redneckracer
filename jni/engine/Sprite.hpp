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

#ifndef SPRITE_HPP_INCLUDED
#define SPRITE_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "miniblocxx/IntrusiveReference.hpp"
#include "miniblocxx/String.hpp"
#include "TexturedQuad.hpp"
#include "Collider.hpp"
#include "Rectangle.hpp"

namespace engine
{

class Sprite : public virtual Boundable, public Drawable, public Collider
{
public:
	Sprite(const TexturedQuadPtr& texturedQuad, const std::string& name = "");
	Sprite(const AnimationPtr& animation, const std::string& name = "");

	virtual ~Sprite();

	virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	virtual void draw(const Rectangle& screen);
	virtual std::string name() const;

	virtual bool doesCollideWith(const Collidable& other) const;
	virtual void handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);

	void updateBoundingRect();

	Size size() const;
	void animation(const AnimationPtr& newAnimation);
	void texturedQuad(const TexturedQuadPtr& newTexturedQuad);

	std::string spriteName;

	bool flippedHorizontal();
	void setFlippedHorizontal(bool x);
	bool flippedVertical();
	void setFlippedVertical(bool x);

	void setPosition(const Point& position);

private:
	TexturedQuadPtr texturedQuad_;
	AnimationPtr animation_;
};

}

#endif
