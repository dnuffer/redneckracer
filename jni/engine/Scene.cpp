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
#include "Scene.hpp"
#include "Collidable.hpp"
#include "Collider.hpp"
#include "boost/foreach.hpp"
#define foreach BOOST_FOREACH
#include <algorithm> // for remove

#include "engine/Log.hpp"

namespace engine
{
	namespace
	{
		struct CompareZOrder
		{
			bool operator()(const Scene::ChildInfo& x, const Scene::ChildInfo& y) { return x.zOrder < y.zOrder; }
		};
		
		struct DrawableEquals
		{
			DrawablePtr drawable;
			DrawableEquals(const DrawablePtr& d) : drawable(d) {}
			bool operator()(const Scene::ChildInfo& x) { return x.drawable == drawable; }
		};
	}
	
	Scene::~Scene()
	{}
	
	Scene& Scene::addChild(const DrawablePtr& child, int zOrder)
	{
		children.push_back(ChildInfo(child, zOrder));
		stable_sort(children.begin(), children.end(), CompareZOrder());

#if defined(AUTO_COLLIDER)
		CollidablePtr c = dynamic_pointer_cast<Collidable>(child);
		if( c )
		{
			addCollidableChild(c);
		}
#endif

		return *this;
	}

	Scene& Scene::removeChild(const DrawablePtr& child)
	{
		children.erase(remove_if(children.begin(), children.end(), DrawableEquals(child)), children.end());
		return *this;
	}
	
	Scene& Scene::removeAllChildren()
	{
		children.clear();
		collidableChildren.clear();
		return *this;

	}

	Scene& Scene::addCollidableChild(const CollidablePtr& collidableChild)
	{
		collidableChildren.push_back(collidableChild);
		return *this;
	}
	
	void Scene::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		Drawable::update(thisFrameStartTime, deltaTime);
		
		// make a copy so that children objects can change the children in update()
		vector<ChildInfo> childrenCopy(children);
		foreach (ChildInfo& ci, childrenCopy)
			ci.drawable->update(thisFrameStartTime, deltaTime);
	}
	
	void Scene::handleCollisions(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		doCollisionChecks(collidableChildren, thisFrameStartTime, deltaTime);
	}
	
	void Scene::draw(const Rectangle& screen)
	{
		foreach (ChildInfo& ci, children)
			ci.drawable->draw(screen);
	}
	
	void Scene::handleTouchEvent(const TouchEvent& touchEvent)
	{

	}

	void Scene::handleActivated()
	{
	}
}
