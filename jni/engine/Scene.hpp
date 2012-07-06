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

#ifndef SCENE_HPP_INCLUDED
#define SCENE_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "Collidable.hpp"
#include "miniblocxx/vector.hpp"
#include "TouchEvent.hpp"

namespace engine
{
class Scene : public Drawable
{
public:
	
	struct ChildInfo
	{
		ChildInfo(const DrawablePtr& d, int z = 0) : drawable(d), zOrder(z) {}
		DrawablePtr drawable;
		int zOrder;
	};
	
#if DEBUG
	Scene() {}
#endif
	virtual ~Scene();
	
	// Accessor methods (for testing purposes)
	std::vector<ChildInfo> getChildren() { return children; }
	std::vector<CollidablePtr> getCollidableChildren() { return collidableChildren; }

	// children are drawn lowest z-order first. children with the same z-order are drawn in order of addition.
	Scene& addChild(const DrawablePtr& child, int zOrder = 0);
	Scene& removeChild(const DrawablePtr& child);
	Scene& removeAllChildren();
	Scene& addCollidableChild(const CollidablePtr& collidableChild);

	virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	virtual void handleCollisions(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	virtual void draw(const Rectangle& screen);
	virtual void handleTouchEvent(const TouchEvent& touchEvent);
	virtual void handleActivated();
	
private:
	std::vector<ChildInfo> children;
	std::vector<CollidablePtr> collidableChildren;
};

}

#endif
