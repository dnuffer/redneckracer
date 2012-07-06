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
#if !defined(BOUNDING_GRID_2D_HPP)
#define BOUNDING_GRID_2D_HPP

#include "graphlib/Bounding2d.hpp"
#include "graphlib/Intersectable2d.hpp"
#include "miniblocxx/Array.hpp"

namespace graphlib
{
	class AccelGrid2d;
	typedef blocxx::IntrusiveReference<AccelGrid2d> AccelGrid2dRef;

	class GridEntry2d : public BoundingBase2d
	{
	public:
		typedef Intersectable2d Object; // FIXME! For use in the game, we'll need to use sprits, collidables, whatever.
		typedef blocxx::IntrusiveReference<Object> ObjectRef;

		GridEntry2d()
			: m_containedObjects()
			, m_subGrid()
		{
		}

		~GridEntry2d();

		const ObjectRef& operator[](size_t index) const
		{
			return m_containedObjects[index];
		}
		ObjectRef operator[](size_t index)
		{
			return m_containedObjects[index];
		}
		size_t objects() const
		{
			return m_containedObjects.size();
		}

		void addObject(ObjectRef obj, BoundingBox2dRef bounds = BoundingBox2dRef());

		// Required virtual overrides
		blocxx::IntrusiveReference<Cloneable> clone() const;
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String name() const;
		bool intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const;
		bool intersects(const Line2& l) const;
		bool encompasses(const Point2& p) const;
		BoundingBox2dRef getBoundingBox() const;

		bool allIntersections(const Point2& origin, const Vector2& direction, RayIntersectionType it, blocxx::Array<ObjectRef>& intersectedObjects) const;

	private:
		blocxx::Array<ObjectRef> m_containedObjects;
		AccelGrid2dRef m_subGrid;
	};

	class AccelGrid2d : public BoundingBase2d
	{
	public:
		typedef GridEntry2d::Object Object;
		typedef GridEntry2d::ObjectRef ObjectRef;

		void addObject(ObjectRef obj, BoundingBox2dRef bounds);

		// These are intended to be called after some number of objects
		// has been added.
		void createBlockGrid(size_t subgrids = 0);
		void createSelfSizedGrid(int subgrids = 0);

		size_t countObjects() const
		{
			return m_containedObjects.size();
		}

		GridEntry2d& gridEntry(size_t i, size_t j)
		{
			const size_t x_step = 1;
			const size_t y_step = x_step * m_ny;
			return m_gridEntries[i * x_step + j * y_step];
		}

		const GridEntry2d& gridEntry(size_t i, size_t j) const
		{
			const size_t x_step = 1;
			const size_t y_step = x_step * m_ny;
			return m_gridEntries[i * x_step + j * y_step];
		}

		// Required virtual overrides
		blocxx::IntrusiveReference<Cloneable> clone() const;
		void collectInternalMembers(MemberStringDumpCollector& collector) const;
		blocxx::String name() const;
		bool intersects(const Point2& origin, const Vector2& direction, RayIntersectionType it) const;
		bool intersects(const Line2& l) const;
		bool encompasses(const Point2& p) const;
		BoundingBox2dRef getBoundingBox() const;

		bool allIntersections(const Point2& origin, const Vector2& direction, RayIntersectionType it, blocxx::Array<ObjectRef>& intersectedObjects) const;

		const Point2& minCoord() const { return m_minCoord; }
		const Point2& maxCoord() const { return m_maxCoord; }
		int nx() const { return m_nx; }
		int ny() const { return m_ny; }

	private:
		int m_nx, m_ny; // Number of subdivisions in x,y
		Point2 m_minCoord;
		Point2 m_maxCoord;
		blocxx::Array<GridEntry2d> m_gridEntries;
		blocxx::Array<ObjectRef> m_containedObjects;
	};

}

#endif
