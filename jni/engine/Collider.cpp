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

#include "Collider.hpp"
#include "graphlib/Fuzzy.hpp"
#include "engine/Log.hpp"
#include <algorithm>
#include <set>

namespace engine
{
	namespace
	{
		enum ColliderBoundary
		{
			E_Entry,
			E_Exit
		};

		struct ColliderData
		{
			ColliderBoundary boundary;
			float value;
			CollidablePtr object;

			ColliderData( ColliderBoundary bounds, float location, CollidablePtr ptr)
				: boundary(bounds), value(location), object(ptr)
			{
			}
		};

		bool operator<(const ColliderData& c1, const ColliderData& c2)
		{
			return fuzzyLess(c1.value, c2.value);
		}

		void addColliders(std::vector<ColliderData>& data, std::vector<CollidablePtr>& objects,
			ColliderBoundary boundary, Rectangle::Components component)
		{
			data.reserve(data.size() + objects.size());

			for( std::vector<CollidablePtr>::iterator obj = objects.begin(); obj != objects.end(); ++obj )
			{
				if( *obj )
				{
					data.push_back( ColliderData( boundary, (*obj)->getBoundingRect()[component], *obj ) );
				}
			}
		}
	}

	// This implements a variant of the sorted
	void doCollisionChecks(std::vector<CollidablePtr>& objects, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		std::vector<ColliderData> data;
		data.reserve(objects.size() * 2);

		// Enter at the bottom, exit at the top.
		addColliders(data, objects, E_Entry, Rectangle::E_Bottom);
		addColliders(data, objects, E_Exit, Rectangle::E_Top);

		// Sort by vertical (Y) coordinate of either the top or bottom.
		std::sort(data.begin(), data.end());

		// Go through all of the sorted objects and at the bottom (Entry), add
		// each into the given set.  We can continue adding objects to this set
		// until we hit the top (Exit) of a rectangle.  If the object leaving the
		// set is a collider, all currently overlapping objects need to be tested
		// for collision.
		std::set<CollidablePtr> verticallyOverlappingObjects;
		for( std::vector<ColliderData>::iterator iter = data.begin(); iter != data.end(); ++iter )
		{
			if( iter->boundary == E_Entry )
			{
				verticallyOverlappingObjects.insert(iter->object);
			}
			else if( iter->boundary == E_Exit )
			{
				verticallyOverlappingObjects.erase(iter->object);

				ColliderPtr ptr = boost::dynamic_pointer_cast<Collider>(iter->object);

				for( std::set<CollidablePtr>::iterator otherObject = verticallyOverlappingObjects.begin();
						  otherObject != verticallyOverlappingObjects.end();
						  ++otherObject )
				{
					// Now test.
					if( iter->object != *otherObject )
					{
						if( ptr && ptr->shouldCheckForCollision() && ptr->doesCollideWith(**otherObject) )
						{
							ptr->handleCollision(**otherObject, thisFrameStartTime, deltaTime);
						}
						else
						{
							ColliderPtr oth = boost::dynamic_pointer_cast<Collider>(*otherObject);
							if( oth && oth->shouldCheckForCollision() && iter->object && oth->doesCollideWith(*(iter->object)) )
							{
								oth->handleCollision(*(iter->object), thisFrameStartTime, deltaTime);
							}
						}
					}
				}
			}
		}
	}
}
