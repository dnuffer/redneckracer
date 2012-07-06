/*
 * ColliderTests.cpp
 *
 *  Created on: July 7, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "boost/smart_ptr/intrusive_ptr.hpp"
#include "engine/Collidable.hpp"
#include "engine/Collider.hpp"
#include "miniblocxx/DateTime.hpp"

using namespace engine;
// Constants
const TimeDuration delta(5.0f);

class Hitter;
typedef boost::intrusive_ptr<Hitter> HitterPtr;

class Hitter : public Collider
{
public:
	Hitter() { reset(); }
	virtual ~Hitter() {}
	
	void reset() { hit = 0; }
	
	virtual bool shouldCheckForCollision() const { return true; }
	virtual bool doesCollideWith(const Collidable& other) const
	{
		return intersecting(this->getBoundingRect(), other.getBoundingRect());
	}
	virtual void handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		hit++;
	}
	
	int hit;
};

AUTO_UNIT_TEST(doCollisionChecks)
{
	// Arrange
	std::vector<CollidablePtr> hitters;
	hitters.reserve(2);
	
	HitterPtr one = new Hitter();
	one->setBoundingRect(Rectangle(0,2,2,0));
	hitters.push_back(one);
	
	HitterPtr two = new Hitter();
	two->setBoundingRect(Rectangle(1,3,3,1));
	hitters.push_back(two);
	
	HitterPtr three = new Hitter();
	three->setBoundingRect(Rectangle(0,3,3,0));
	hitters.push_back(three);
	
	HitterPtr four = new Hitter();
	four->setBoundingRect(Rectangle(3,4,4,3));
	hitters.push_back(four);
	
	// Act
	doCollisionChecks(hitters, DateTime(), delta);
	
	// Assert
	unitAssert(one->hit == 2);
	unitAssert(two->hit == 0);
	unitAssert(three->hit == 1);
	unitAssert(four->hit == 0);
}
