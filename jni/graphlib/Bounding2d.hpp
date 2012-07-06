/*
 * Part of GraphLib -- A framework for graphics production.
 * Copyright (C) 2009 Kevin Harris
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
#if !defined(BOUNDING_2D_HPP_INCLUDED)
#define BOUNDING_2D_HPP_INCLUDED

#include "graphlib/Types.hpp"
#include "graphlib/BaseClass.hpp"
#include "graphlib/Intersectable2d.hpp"

namespace graphlib
{
	class BoundingBase2d : public Intersectable2d
	{
	public:
		BoundingBase2d() { }
		virtual ~BoundingBase2d();

		virtual bool encompasses(const Point2& p) const = 0;
	};

	// A 2d axis-aligned bounding box (square).
	class AlignedBoundingBox2d : public BoundingBase2d
	{
	public:
		AlignedBoundingBox2d() { }
		AlignedBoundingBox2d(const Point2& p1, const Point2& p2) : c1(p1), c2(p2) { }
		~AlignedBoundingBox2d();

		// Useful intersection/containment tests
		bool intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const;
		bool intersects(const Line2& l) const;
		bool encompasses(const Point2& p) const;
		blocxx::IntrusiveReference<Cloneable> clone() const;

		// Simple getters.
		const Point2& corner1() const { return c1; }
		const Point2& corner2() const { return c2; }
		Point2& corner1() { return c1; }
		Point2& corner2() { return c2; }

		// To be string dumpable.
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String name() const;

		BoundingBox2dRef getBoundingBox() const;

		// Some internal details exposed for use in other functions.
		static void getSlopeParameters(
			const Point2& c1, const Point2& c2,
			const Point2& origin, const Vector2& direction,
			NumberType& tx_min, NumberType& tx_max,
			NumberType& ty_min, NumberType& ty_max);

	private:
		Point2 c1, c2; // The two opposite corners of the box.  The midpoint of the line between them is the center of the box.
	};

	bool pointWithinRectangle(const Point2& p, const Point2& c1, const Point2& c2);

} // end namespace graphlib

#endif /* defined(BOUNDING_2D_HPP_INCLUDED) */
