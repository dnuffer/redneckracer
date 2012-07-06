/*
 * Part of GraphLib -- a framework for graphics production.
 * Copyright (C) 2010 Kevin Harris
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
#if !defined(GRAPHLIB_POLYGON_2D_HPP_INCLUDED)
#define GRAPHLIB_POLYGON_2D_HPP_INCLUDED

#include "graphlib/Types.hpp"
#include "graphlib/Bounding2d.hpp"
#include "miniblocxx/Array.hpp"

namespace graphlib
{
	// A class for convex polygons.  Points that cause concave polygons
	// are not allowed and have undefined behavior.
	class Polygon2d : public BoundingBase2d
	{
	public:
		Polygon2d() { }
		Polygon2d(const blocxx::Array<Point2>& points) : m_points(points) { }
		virtual ~Polygon2d();

		void addPoint(const Point2& p) { m_points.push_back(p); }

		blocxx::IntrusiveReference<Cloneable> clone() const;
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String name() const;

		bool intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const;
		bool intersects(const Line2& l) const;
		bool encompasses(const Point2& p) const;
		BoundingBox2dRef getBoundingBox() const;

	private:
		blocxx::Array<Point2> m_points;
	};
}

#endif /* !defined(GRAPHLIB_POLYGON_2D_HPP_INCLUDED) */
