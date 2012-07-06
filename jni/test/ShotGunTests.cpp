/*
 * SpriteTests.cpp
 *
 *  Created on: July 7, 2011
 *      Author: Michael Landes
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "game/ShotGun.hpp"
#include "engine/Point.hpp"


using namespace rr;


AUTO_UNIT_TEST(TestDamageMath)
{
	// Damage is calculated linearly, within inner radius, full damage is taken
	// Damage outside outer radius is zero
	// There is a linear decrease by a factor of [y = -1/2(x) + range] in between radii
	ShotGun shotgun;
	float range = shotgun.getShotRange();
	// two points together
	Point p1(10,0);
	Point p2(10,0);
	unitAssert(shotgun.calculateDamage(p1,p2) == shotgun.getDamage());
	// negative and positive points the same within inner radius
	Point p3(10,range/2-10);
	Point p4(10,-range/2+10);
	unitAssert(shotgun.calculateDamage(p1,p3) == shotgun.calculateDamage(p1,p4));
	// point on inner radius gets full damage
	unitAssert(shotgun.calculateDamage(p1,p3) == shotgun.getDamage());
	// point on edge of radius gets full damage
	Point p5(10,range/2);
	unitAssert(shotgun.calculateDamage(p1,p5) == shotgun.getDamage());
	// point on edge of outer radius gets zero damage
	Point p6(10,range);
	unitAssert(shotgun.calculateDamage(p1,p6) == 0);
	// point outside outer radius gets zero damage
	Point p7(10,range+10);
	unitAssert(shotgun.calculateDamage(p1,p7) == 0);
	// point halfway in between radii gets half damage
	Point p8(10,range/2+range/4);
	unitAssert(shotgun.calculateDamage(p1,p8) == shotgun.getDamage()/2);
	// negative the last one just for fun
	Point p9(10,-range/2-range/4);
	unitAssert(shotgun.calculateDamage(p1,p8) == shotgun.calculateDamage(p1,p9));
}
