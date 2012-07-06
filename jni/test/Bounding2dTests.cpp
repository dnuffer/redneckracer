
#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "graphlib/Bounding2d.hpp"

using namespace blocxx;
using namespace graphlib;

#include <iostream>

AUTO_UNIT_TEST(Bounding2dEncompasses)
{
	Point2 p1(0,0);
	Point2 p2(1,1);
	AlignedBoundingBox2d box(p1, p2);

	// Check the center and all 4 corners.
	unitAssert(box.encompasses(Point2(0.5,0.5)));
	unitAssert(box.encompasses(Point2(0.0,0.0)));
	unitAssert(box.encompasses(Point2(0.0,1.0)));
	unitAssert(box.encompasses(Point2(1.0,0.0)));
	unitAssert(box.encompasses(Point2(1.0,1.0)));

	// Check stuff outside.
	// In the middle, just outside
	unitAssert(!box.encompasses(Point2(0.5, -0.1)));
	unitAssert(!box.encompasses(Point2(0.5, 1.1)));
	unitAssert(!box.encompasses(Point2(-0.1, 0.5)));
	unitAssert(!box.encompasses(Point2(1.1, 0.5)));
	// Diagonal outside
	unitAssert(!box.encompasses(Point2(-0.1, -0.1)));
	unitAssert(!box.encompasses(Point2(-0.1, 1.1)));
	unitAssert(!box.encompasses(Point2(1.1, 1.1)));
	unitAssert(!box.encompasses(Point2(1.1, -0.1)));
}

AUTO_UNIT_TEST(Bounding2dIntersectsIntersecting)
{
	Point2 p1(0,0);
	Point2 p2(1,1);

	// A line that slices right through the middle, diagonally,
	// intersecting both control corners.  The origin is far enough
	// away that the scaled direction should discount any intersection
	// if tested as a line segment.
	Point2 o(-1,-1);
	Vector2 v = unit(Vector2(1,1));

	AlignedBoundingBox2d box(p1, p2);

	unitAssert(box.intersects(o, v, InfiniteLine));
	unitAssert(box.intersects(o, -v, InfiniteLine));

	// As line segments, the intersection is out of range.
	unitAssert(!box.intersects(o, v, LineSegment));
	unitAssert(!box.intersects(o, -v, LineSegment));

	// FIXME! More tests.
}

AUTO_UNIT_TEST(Bounding2dIntersectsGlancing)
{
	Point2 p1(0,0);
	Point2 p2(1,1);
	AlignedBoundingBox2d box(p1, p2);

	// Bumping a corner, but not actually penetrating the box (evil
	// corner case).
	{
		Point2 o1(-1, 0);
		Point2 o2(0, -1);
		Vector2 v1 = unit(Vector2(1,1));

		// None of these should be considered a hit.
		unitAssert(!box.intersects(o1, v1, InfiniteLine));
		unitAssert(!box.intersects(o1, -v1, InfiniteLine));
		unitAssert(!box.intersects(o1, v1, LineSegment));
		unitAssert(!box.intersects(o1, -v1, LineSegment));

		unitAssert(!box.intersects(o2, v1, InfiniteLine));
		unitAssert(!box.intersects(o2, -v1, InfiniteLine));
		unitAssert(!box.intersects(o2, v1, LineSegment));
		unitAssert(!box.intersects(o2, -v1, LineSegment));
	}
}
