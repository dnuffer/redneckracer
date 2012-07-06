/*
 * Copyright 2011 Kevin Harris
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
#include "graphlib/Bounding2d.hpp"

namespace graphlib
{
	using namespace blocxx;

	BoundingBase2d::~BoundingBase2d()
	{
	}

	AlignedBoundingBox2d::~AlignedBoundingBox2d()
	{
	}

	void AlignedBoundingBox2d::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("corner1", c1);
		collector.addMember("corner2", c2);
	}

	String AlignedBoundingBox2d::name() const
	{
		return "AlignedBoundingBox2d";
	}

	BoundingBox2dRef AlignedBoundingBox2d::getBoundingBox() const
	{
		return BoundingBox2dRef(new AlignedBoundingBox2d(*this));
	}

	void AlignedBoundingBox2d::getSlopeParameters(
		const Point2& c1, const Point2& c2,
		const Point2& origin, const Vector2& direction,
		NumberType& tx_min, NumberType& tx_max,
		NumberType& ty_min, NumberType& ty_max)
	{
		tx_min = (c2.x() - origin.x()) / direction.x();
		ty_min = (c2.y() - origin.y()) / direction.y();
		tx_max = (c1.x() - origin.x()) / direction.x();
		ty_max = (c1.y() - origin.y()) / direction.y();

		// If the swaps don't inline, this will need to be done twice.  The
		// optimizer can merge identical subtractions/divisions.
		if( direction.x() >= 0 )
		{
			std::swap(tx_min, tx_max);
		}
		if( direction.y() >= 0 )
		{
			std::swap(ty_min, ty_max);
		}
	}

	namespace
	{
		// For most shapes, it is slow to calculate the t (distance from origin,
		// scaled by the direction vector) intersection point.  For these axis
		// aligned boxes, it is trivial.
		bool findAxisAlignedBoundingIntersectionPoint(
			const Point2& c1, const Point2& c2,
			const Point2& origin, const Vector2& direction,
			NumberType& t)
		{
			// This function relies on floating point properties of infinite
			// numbers and division.
			NumberType tx_min, tx_max;
			NumberType ty_min, ty_max;

			AlignedBoundingBox2d::getSlopeParameters(c1, c2, origin, direction, tx_min, tx_max, ty_min, ty_max);

			NumberType t0 = std::max(tx_min, ty_min);
			NumberType t1 = std::min(tx_max, ty_max);

			if( t0 < t1 )
			{
				// There is a hit here... (according to Pete Shirley's book "Realistic
				// Raytracing").
				t = t0;

				return true;
			}
			return false;
		}

	}

	bool AlignedBoundingBox2d::intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const
	{
		NumberType t;
		if( findAxisAlignedBoundingIntersectionPoint(c1, c2, origin, direction, t) )
		{
			if( it != LineSegment )
			{
				return true;
			}
			return (t >= 0) && (t < 1);
		}
		return false;
	}

	bool AlignedBoundingBox2d::intersects(const Line2& l) const
	{
		NumberType t;
		if( findAxisAlignedBoundingIntersectionPoint(c1, c2, l.origin(), l.direction(), t) )
		{
			return l.limits().inside(t);
		}
		return false;
	}

	bool AlignedBoundingBox2d::encompasses(const Point2& p) const
	{
		return pointWithinRectangle(p, c1, c2);
	}

	IntrusiveReference<Cloneable> AlignedBoundingBox2d::clone() const
	{
		return IntrusiveReference<Cloneable>(new AlignedBoundingBox2d(c1, c2));
	}

	bool pointWithinRectangle(const Point2& p, const Point2& c1, const Point2& c2)
	{
		NumberType x1 = c1.x();
		NumberType x2 = c2.x();

		NumberType y1 = c1.y();
		NumberType y2 = c2.y();

		NumberType x = p.x();
		NumberType y = p.y();

		if( x1 > x2 )
		{
			std::swap(x1,x2);
		}
		if( y1 > y2 )
		{
			std::swap(y1,y2);
		}

		return (x >= x1) && (x <= x2) && (y >= y1) && (y <= y2);
	}
}
