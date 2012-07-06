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
#include "graphlib/BoundingGrid2d.hpp"

namespace graphlib
{
	namespace
	{
		template<typename T>
		inline void clamp(T& n, T minimum, T maximum)
		{
			if( n < minimum )
			{
				n = minimum;
			}
			if( n > maximum )
			{
				n = maximum;
			}
		}

		template <typename T>
		inline int sign(T n)
		{
			if( n >= 0 )
			{
				return 1;
			}
			return -1;
		}
	}

	GridEntry2d::~GridEntry2d()
	{
	}

	void GridEntry2d::addObject(ObjectRef obj, BoundingBox2dRef bounds)
	{
		m_containedObjects.push_back(obj);
		if( m_subGrid )
		{
			if( bounds )
			{
				m_subGrid->addObject(obj, bounds);
			}
			else
			{
				// Given a NULL bounding box... Create a new one
				m_subGrid->addObject(obj, obj->getBoundingBox());
			}
		}
	}

	blocxx::String GridEntry2d::name() const
	{
		return "GridEntry2d";
	}

	void GridEntry2d::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("subgrid", m_subGrid);
		collector.addMember("containedObjects", m_containedObjects);
	}

	blocxx::IntrusiveReference<Cloneable> GridEntry2d::clone() const
	{
		return blocxx::IntrusiveReference<Cloneable>(new GridEntry2d(*this));
	}

	bool GridEntry2d::intersects(const Point2& origin, const Vector2& direction,
		RayIntersectionType it) const
	{
		if( m_subGrid )
		{
			return m_subGrid->intersects(origin, direction, it);
		}

		for( blocxx::Array<ObjectRef>::const_iterator object = m_containedObjects.begin();
			  object != m_containedObjects.end();
			  ++object )
		{
			if( (*object)->intersects(origin, direction, it) )
			{
				return true;
			}
		}
		return false;
	}

	bool GridEntry2d::encompasses(const Point2& p) const
	{
		if( m_subGrid )
		{
			return m_subGrid->encompasses(p);
		}
		return false;
	}

	BoundingBox2dRef GridEntry2d::getBoundingBox() const
	{
		if( m_subGrid )
		{
			return m_subGrid->getBoundingBox();
		}
		return BoundingBox2dRef();
	}

	bool GridEntry2d::intersects(const Line2& l) const
	{
		// FIXME! We need to do something with the bounds.
		return intersects(l.origin(), l.direction(), LineSegment);
	}

	bool GridEntry2d::allIntersections(
		const Point2& origin, const Vector2& direction, RayIntersectionType it,
		blocxx::Array<ObjectRef>& intersectedObjects) const
	{
		if( m_subGrid )
		{
			return m_subGrid->allIntersections(origin, direction, it, intersectedObjects);
		}

		bool hitSomething = false;
		for( blocxx::Array<ObjectRef>::const_iterator object = m_containedObjects.begin();
			  object != m_containedObjects.end();
			  ++object )
		{
			if( (*object)->intersects(origin, direction, it) )
			{
				intersectedObjects.push_back(*object);
				hitSomething = true;
			}
		}
		return hitSomething;
	}

	blocxx::IntrusiveReference<Cloneable> AccelGrid2d::clone() const
	{
		return blocxx::IntrusiveReference<Cloneable>(new AccelGrid2d(*this));
	}

	void AccelGrid2d::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("nx", m_nx);
		collector.addMember("ny", m_ny);
		collector.addMember("minCoord", m_minCoord);
		collector.addMember("maxCoord", m_maxCoord);
		collector.addMember("containedObjects", m_containedObjects);
		collector.addMember("gridEntries", m_gridEntries);
	}

	blocxx::String AccelGrid2d::name() const
	{
		return "AccelGrid2d";
	}

	void AccelGrid2d::addObject(ObjectRef obj, BoundingBox2dRef bounds)
	{
		BoundingBox2dRef box = bounds;
		if( !box )
		{
			// Given a NULL bounding box... Create a new one
			box = obj->getBoundingBox();
		}
		// If we have a box, put the object in the correct bins.
		if( box )
		{
			Vector2 gridSize = m_maxCoord - m_minCoord;

			NumberType x_step = gridSize.x() / m_nx;
			NumberType y_step = gridSize.x() / m_ny;

			// Calculate the min and max values for the boxes...
			NumberType fx_bin_min = (box->corner1().x() - m_minCoord.x()) / x_step;
			NumberType fx_bin_max = (box->corner2().x() - m_minCoord.x()) / x_step;

			NumberType fy_bin_min = (box->corner1().y() - m_minCoord.y()) / y_step;
			NumberType fy_bin_max = (box->corner2().y() - m_minCoord.y()) / y_step;

			clamp(fx_bin_min, NumberType(0), NumberType(m_nx - 1));
			clamp(fx_bin_max, NumberType(0), NumberType(m_nx - 1));

			clamp(fy_bin_min, NumberType(0), NumberType(m_ny - 1));
			clamp(fy_bin_max, NumberType(0), NumberType(m_ny - 1));

			int x_bin_min = int(fx_bin_min);
			int x_bin_max = int(fx_bin_max);

			int y_bin_min = int(fy_bin_min);
			int y_bin_max = int(fy_bin_max);


			// Add it to all blocks between the min and max blocks...
			// And yes, to answer future questions, it should be added to all of
			// these blocks, as the bounding box is axis aligned.
			for(int y = y_bin_min; y <= y_bin_max; ++y)
			{
				for(int x = x_bin_min; x <= x_bin_max; ++x)
				{
					gridEntry(x,y).addObject(obj, box);
				} // x loop
			} // y loop
		}
		else
		{
			// No bounding box (still null, after requesting again)!!!
			// We'll just have to add it to every cell, to avoid creating special
			// cases.
			// THIS IS HORRIBLE!
			for(int y = 0; y < m_ny; ++y)
			{
				for(int x = 0; x < m_nx; ++x)
				{
					gridEntry(x,y).addObject(obj, box);
				} // for all x divisions
			} // for all y divisions
		}
	}

	namespace
	{
		typedef bool (*IntersectionLoop)(
			int ix, int iy,
			int ix_step, int iy_step,
			int ix_stop, int iy_stop,
			NumberType t_min, NumberType t_max,
			NumberType tx_next, NumberType ty_next,
			NumberType dtx, NumberType dty,
			const Point2& origin, const Vector2& direction, RayIntersectionType it,
			const AccelGrid2d* grid, blocxx::Array<GridEntry2d::ObjectRef>* intersectedObjects);

		bool intersectionLoopFindFirst(
			int ix, int iy,
			int ix_step, int iy_step,
			int ix_stop, int iy_stop,
			NumberType t_min, NumberType t_max,
			NumberType tx_next, NumberType ty_next,
			NumberType dtx, NumberType dty,
			const Point2& origin, const Vector2& direction, RayIntersectionType it,
			const AccelGrid2d* grid, blocxx::Array<GridEntry2d::ObjectRef>* intersectedObjects)
		{
			// Nearly identical to intersectionLoopFindAll
			while(true)
			{
				if( grid->gridEntry(ix, iy).intersects(origin, direction, it) )
				{
					return true;
				}

				if( tx_next < ty_next )
				{
					// X grid boundary is next...
					t_min = tx_next;
					tx_next += dtx;
					ix += ix_step;
					if( ix == ix_stop )
					{
						return false;
					}
				}
				else
				{
					// Y grid boundary is next...
					t_min = ty_next;
					ty_next += dty;
					iy += iy_step;
					if( iy == iy_stop )
					{
						return false;
					}
				}

				if( t_min >= t_max )
				{
					// Went too far, and no hit.
					return false;
				}
			}
		}

		bool intersectionLoopFindAll(
			int ix, int iy,
			int ix_step, int iy_step,
			int ix_stop, int iy_stop,
			NumberType t_min, NumberType t_max,
			NumberType tx_next, NumberType ty_next,
			NumberType dtx, NumberType dty,
			const Point2& origin, const Vector2& direction, RayIntersectionType it,
			const AccelGrid2d* grid, blocxx::Array<GridEntry2d::ObjectRef>* intersectedObjects)
		{
			// Nearly identical to intersectionLoopFindFirst

			bool retval = false;
			while(true)
			{
				if( grid->gridEntry(ix, iy).allIntersections(origin, direction, it, *intersectedObjects) )
				{
					retval = true;
				}

				if( tx_next < ty_next )
				{
					// X grid boundary is next...
					t_min = tx_next;
					tx_next += dtx;
					ix += ix_step;
					if( ix == ix_stop )
					{
						break;
					}
				}
				else
				{
					// Y grid boundary is next...
					t_min = ty_next;
					ty_next += dty;
					iy += iy_step;
					if( iy == iy_stop )
					{
						break;
					}
				}

				if( t_min >= t_max )
				{
					// Went too far, and no hit.
					break;
				}
			}

			return retval;
		}

		bool intersectionHelper(
			const Point2& origin, const Vector2& direction, RayIntersectionType it,
			const AccelGrid2d* grid,
			blocxx::Array<GridEntry2d::ObjectRef>* intersectedObjects,
			IntersectionLoop mainIntersectionLoop )
		{
			if( grid->countObjects() == 0 )
			{
				return false;
			}

			// This is a long and complex function.  The bulk of it is from Peter
			// Shirley's "Realist Raytracing" (section 11.1 -- almost 3 pages worth)
			NumberType tx_min, tx_max;
			NumberType ty_min, ty_max;

			// This uses exactly the same algorithm as in the AxisAlignedBoundingBox.
			// We do it here to efficiently walk through the grid.
			AlignedBoundingBox2d::getSlopeParameters(
				grid->minCoord(), grid->maxCoord(),
				origin, direction,
				tx_min, tx_max, ty_min, ty_max);

			NumberType t0 = std::max(tx_min, ty_min);
			NumberType t1 = std::min(tx_max, ty_max);

			// The AxisAlignedBoundingBox uses t0 < t1 to detect a hit.  We don't
			// care about the point where they touch, so we just use t0 > t1 when
			// inverting the logic.
			if( t0 > t1 )
			{
				// Didn't hit the grid at all.
				return false;
			}

			// If we are using a line segment instead of an infinite line, we can
			// limit the T values to [0,1].
			if( it == LineSegment )
			{
				clamp<NumberType>(t0, 0, 1);
				clamp<NumberType>(t1, 0, 1);
			}

			int nx = grid->nx();
			int ny = grid->ny();

			// Now that we know it hit the grid, we need to follow it through the
			// grid, only walking through the cells that need it.
			NumberType dtx = (tx_max - tx_min) / NumberType(nx);
			NumberType dty = (ty_max - ty_min) / NumberType(ny);

			Point2 p = origin + t0 * direction;
			Vector2 diff = grid->maxCoord() - grid->minCoord();
			Vector2 diff1 = p - grid->minCoord();

			// Find the initial x, and y spots where the ray enters the grid.
			int ix = int(nx * diff1.x() / diff.x());
			int iy = int(ny * diff1.y() / diff.y());

			clamp(ix, 0, nx - 1);
			clamp(iy, 0, ny - 1);

			// The next T value in each dimension
			NumberType tx_next, ty_next;

			// The direction, amount of change...
			int ix_step = sign(direction.x());
			int iy_step = sign(direction.y());

			// The max before bail.
			int ix_stop, iy_stop;

			if(direction.x() > 0)
			{
				tx_next = tx_min + (ix + 1) * dtx;
				ix_stop = nx;
			}
			else
			{
				tx_next = tx_min + (nx - ix) * dtx;
				ix_stop = -1;
			}

			if(direction.y() > 0)
			{
				ty_next = ty_min + (iy + 1) * dty;
				iy_stop = ny;
			}
			else
			{
				ty_next = ty_min + (ny - iy) * dty;
				iy_stop = -1;
			}

			NumberType t_min = t0;
			NumberType t_max = t1;

			// To keep things small and reduce duplication, this calls
			// another helper to do the walking.
			return mainIntersectionLoop(
				ix, iy,
				ix_step, iy_step,
				ix_stop, iy_stop,
				t_min, t_max,
				tx_next, ty_next,
				dtx, dty,
				origin, direction, it,
				grid, intersectedObjects);
		}
	}

	bool AccelGrid2d::intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const
	{
		return intersectionHelper(origin, direction, it, this, NULL, intersectionLoopFindFirst);
	}

	bool AccelGrid2d::intersects(const Line2& l) const
	{
		// FIXME! We need to do something with the bounds.
		return AccelGrid2d::intersects(l.origin(), l.direction(), LineSegment);
	}

	bool AccelGrid2d::allIntersections(
		const Point2& origin, const Vector2& direction, RayIntersectionType it,
		blocxx::Array<ObjectRef>& intersectedObjects) const
	{
		return intersectionHelper(origin, direction, it, this, &intersectedObjects, intersectionLoopFindAll);
	}

	bool AccelGrid2d::encompasses(const Point2& p) const
	{
		return pointWithinRectangle(p, m_minCoord, m_maxCoord);
	}

	BoundingBox2dRef AccelGrid2d::getBoundingBox() const
	{
		return BoundingBox2dRef(new AlignedBoundingBox2d(m_minCoord, m_maxCoord));
	}
}
