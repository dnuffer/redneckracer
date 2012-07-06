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
#include "graphlib/Polygon2d.hpp"

namespace graphlib
{
	Polygon2d::~Polygon2d()
	{
	}

	blocxx::IntrusiveReference<Cloneable> Polygon2d::clone() const
	{
		return blocxx::IntrusiveReference<Cloneable>(new Polygon2d(*this));
	}
	void Polygon2d::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("points", m_points);
	}
	blocxx::String Polygon2d::name() const
	{
		return "Polygon2d";
	}

	namespace
	{
		inline bool segmentsIntersect(
			const Point2& o1, const Vector2& d1,
			const Point2& o2, const Vector2& d2,
			NumberType& t1, NumberType& t2)
		{
			NumberType denom = d2.y() * d1.x() - d2.x() * d1.y();

			// Presumably, this test is faster than doing the fp
			// multiplication/divison below
			if( (denom < 4 * std::numeric_limits<NumberType>::epsilon()) &&
				(denom > -4 * std::numeric_limits<NumberType>::epsilon()) )
			{
				// parallel lines.
				return false;
			}

			Vector2 s3 = o1 - o2;
			t1 = (d2.x() * s3.y() - d2.y() * s3.x()) / denom;
			if( !(t1 >= 0) || !(t1 <= 1) )
			{
				return false;
			}

			t2 = (d1.x() * s3.y() - d1.y() * s3.x()) / denom;
			if( !(t2 >= 0) || !(t2 <= 1) )
			{
				return false;
			}
			return true;
		}
	}

	bool Polygon2d::intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const
	{
		Point2 endpoint = origin + direction;
		if( Polygon2d::encompasses(origin) || Polygon2d::encompasses(endpoint) )
		{
			return true;
		}

		// We need to test each minor line segment to see if it intersects.
		// NOTE: There are more efficient algorithms for doing this, which
		// involve sorting a list of points.  This was easy to write.
		blocxx::Array<Point2>::const_iterator first = m_points.begin();
		blocxx::Array<Point2>::const_iterator previous = first;
		blocxx::Array<Point2>::const_iterator current = previous + 1;

		NumberType unused1, unused2;

		while( current != m_points.end() )
		{
			if( segmentsIntersect(origin, direction, *previous, *current - *previous, unused1, unused2) )
			{
				return true;
			}
			previous = current;
			++current;
		}

		// CHECKME! I think this test is redundant for any enclosed polygon.
		if( segmentsIntersect(origin, direction, *previous, *first - *previous, unused1, unused2) )
		{
			return true;
		}

		return false;
	}

	bool Polygon2d::intersects(const Line2& l) const
	{
		// FIXME! We need to do something with the bounds.
		return Polygon2d::intersects(l.origin(), l.direction(), LineSegment);
	}

	namespace
	{
		// Not really a cross product.  It is as close of a mapping as
		// you can get in 2d.
		inline NumberType CrossProd2d(const Vector2& v1, const Vector2& v2)
		{
			return (v1.x()*v2.y()) - (v2.x()*v1.y());
		}
	}

	bool Polygon2d::encompasses(const Point2& p) const
	{
		if( m_points.size() < 3u )
		{
			return false;
		}

		// This is a neat trick involving cross products (or the minor
		// 2d equivalent).  For each sequential pair of vertices, create
		// a pair of vectors from each of those points to the point
		// being tested. If the cross product of those two vectors has a
		// negative sign, it must be outside the polygon.

		blocxx::Array<Point2>::const_iterator previous = m_points.begin();

		Vector2 first = *previous - p;
		Vector2 v1;
		Vector2 v2 = first;

		// Test all sequential pairs
		blocxx::Array<Point2>::const_iterator current = previous + 1;
		while( current != m_points.end() )
		{
			v1 = v2;
			v2 = *current - p;

			if( CrossProd2d(v1, v2) < 0 )
			{
				return false;
			}

			previous = current;
			++current;
		}
		// Test the pair of last vertex and first vertex to avoid a
		// gap.
		if( CrossProd2d(v2, first) < 0 )
		{
			return false;
		}

		return true;
	}

	BoundingBox2dRef Polygon2d::getBoundingBox() const
	{
		// Can't be a polygon with under 3 points.
		if( m_points.size() < 3u )
		{
			return BoundingBox2dRef();
		}

		blocxx::Array<Point2>::const_iterator point = m_points.begin();
		NumberType minX = point->x();
		NumberType minY = point->y();
		NumberType maxX = minX;
		NumberType maxY = minY;

		for( ++point; point != m_points.end(); ++point )
		{
			const Point2& p = *point;
			minX = std::min(minX, p.x());
			minY = std::min(minY, p.y());
			maxX = std::max(maxX, p.x());
			maxY = std::max(maxY, p.y());
		}

		return BoundingBox2dRef(new AlignedBoundingBox2d(
				Point2(minX, minY), Point2(maxX, maxY)));
	}
}
