/*
 * Part of "Graphlib"
 * Copyright (C) 2003 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(INTERSECTABLE_2D_HPP_INCLUDED)
#define INTERSECTABLE_2D_HPP_INCLUDED

#include "graphlib/CommonFwd.hpp"
#include "graphlib/Types.hpp"
#include "graphlib/BaseClass.hpp"
#include "miniblocxx/IntrusiveReference.hpp"

namespace graphlib
{
	enum RayIntersectionType { LineSegment, InfiniteLine };

	typedef blocxx::IntrusiveReference<BoundingBase2d> BoundingVolume2dRef;
	typedef blocxx::IntrusiveReference<AlignedBoundingBox2d> BoundingBox2dRef;

	class Intersectable2d : public BaseClass
	{
	public:
		virtual ~Intersectable2d();

		virtual bool intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const = 0;
		virtual bool intersects(const Line2& l) const = 0;
		virtual BoundingBox2dRef getBoundingBox() const = 0;
		virtual BoundingVolume2dRef getBoundingVolume() const;
	};
}

#endif // !defined(INTERSECTABLE_2D_HPP_INCLUDED)
