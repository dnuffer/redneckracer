// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __ROADBOUND_HPP__
#define __ROADBOUND_HPP__

#include "RRFwd.hpp"
#include "RRConfig.hpp"
#include <vector>
#include "engine/Collidable.hpp"
#include "engine/Point.hpp"

//#define TEST_DRAWING_ROAD_BOUND

#ifdef TEST_DRAWING_ROAD_BOUND
	#include "engine/Drawable.hpp"
	#include "engine/Collider.hpp"
#endif
namespace rr
{

	class RoadBound:
#ifdef TEST_DRAWING_ROAD_BOUND

	  public Drawable
	, public Collider

#else

	public Collidable

#endif
	{
	public:

		 // xCenter - screen center relatively OpenGl screen coords(0, 0 - top left corner).
		RoadBound(float xCenter, float yCenter);

#ifdef TEST_DRAWING_ROAD_BOUND

		virtual void draw(const Rectangle& screen);
		virtual bool shouldCheckForCollision() const { return false; }
		virtual bool doesCollideWith(const Collidable& other) const { return false; }
		virtual void handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime){};

#endif

		void loadRoadSectionBorder(const String& name);

		// Return left and right x coordinates
		Coord getBoundCoordinates(float yCoord) const;
		bool isPointOnRoad(float x, float y) const;
		Point getPointOnRoad(float y) const;

		void setScreenCenter(float xCenter, float yCenter)
		{
			lastSectionsHeight = -yCenter;
			centerX = xCenter;
			updateBoundingRect();
		}

		Coord getLeftBoundByIndex(unsigned int index);
		Coord getRightBoundByIndex(unsigned int index);
		unsigned int getBoundIndexSize() const { return roadBorder.size(); }

		void updateBoundingRect();

	private:

		struct DoubleCoord
		{
			DoubleCoord(Coord l, Coord r): left(l), right(r){}
			Coord left;
			Coord right;
			bool operator<(DoubleCoord dp) const
			{
				// For use in road border: left.y == right.y.
				if (left.y() < dp.left.y()) return true;
				else return false;
			}

			bool operator<(float yCoord) const
			{
				if (left.y() < yCoord) return true;
				else return false;
			}
		};

		vector<DoubleCoord> roadBorder;
		float lastSectionsHeight;
		float scaleX;
		float scaleY;
		float centerX;

	};
} // namespace rr
#endif // __ROADBOUND_HPP__
