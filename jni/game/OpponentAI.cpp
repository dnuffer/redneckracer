// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "OpponentAI.hpp"
#include "boost/foreach.hpp"
#include "TruckController.hpp"
#include "Truck.hpp"
#include "engine/Log.hpp"
#include "CivilCar.hpp"
#include "ShotGun.hpp"
#include "boost/random.hpp"


#define foreach BOOST_FOREACH

namespace rr
{
	static boost::rand48 rng(::time(NULL));
	static boost::uniform_real<> chance(0.0,5.0);
	
	void OpponentAI::processOtherTrucks(const Rectangle& truckRect, vector<TruckControllerPtr>& trucksControllers, 
										const TimeDuration& deltaTime)
	{		
		setInteracted(false);
		shoot = false;
		foreach(TruckControllerPtr tc, trucksControllers)
		{
			if (NULL == dynamic_cast<CivilCar*>(&(*tc->getTruck()))) // Don't attack civil cars.
			{
				interactWithOtherTruck(truckRect, tc->getTruck()->getBoundingRect(), deltaTime);
			}

			if (isInteracted())
			{
				if (shoot)
				{
					// calculate damage
					Point p1 = getTruckController()->getTruck()->position();
					Point p2 = tc->getTruck()->position();
					int damage = getTruckController()->getShotGun().calculateDamage(p1,p2);
					// handle the hit
					if (damage > 0) {
						// TODO: if we want, play an opponent hit sound
						tc->getTruck()->decrementHealth(damage);
						tc->getTruck()->setAttachedAnimationPosition(Truck::Sparks, Point(0, -(tc->getTruck()->size().height()) / 2));
						tc->getTruck()->activateAttachedAnimation(Truck::Sparks);
					} else {
						// TODO: if we want, play an opponent miss sound
					}
				}
				break; // Interact only with one opponent.
			}
		}

		if (!isInteracted()) // Road is free.
		{
			getTruckController()->nitroActionDown();
		}
	}

	void OpponentAI::interactWithOtherTruck(const Rectangle& truckRect, const Rectangle& opRect, const TimeDuration& deltaTime)
	{
		if (getTruckController()->isDestroyed()) {
			return;
		}
        // if we are out of armor, avoid collisions, but still shoot.
        if(getTruckController()->getTruck()->truckParams.truckArmor <= 0)
        {
            if (getTruckController()->getShotGun().isObjectInRange(truckRect, opRect))
            {
                if (getTruckController()->getShotGun().isReadyToShoot() && chance(rng) < deltaTime.realSeconds())
                {
                    setInteracted(true);
                    shoot = true;
                    getTruckController()->getShotGun().shoot();
                    getTruckController()->getTruck()->activateAttachedAnimation(Truck::ShotgunBlast);
					// TODO: If we want, this is where an opponent shotgun sound would go
                }
            }
            return;
        }
        // else we have armor, so try to attack
		if (isOpponentLeft(truckRect, opRect) && !isOpponentUpper(truckRect, opRect) && !isOpponentBelow(truckRect, opRect))
		{
			if (truckRect.left - opRect.right < hitRange)
			{
				getTruckController()->getTruck()->setInputLeft(true);
				setInteracted(true);
				//LOGI("Hit left");
			}
		}
		else if (isOpponentRight(truckRect, opRect) && !isOpponentUpper(truckRect, opRect) && !isOpponentBelow(truckRect, opRect))
		{
			if (opRect.left - truckRect.right < hitRange)
			{
				getTruckController()->getTruck()->setInputRight(true);
				setInteracted(true);
				//LOGI("Hit right");
			}
		}
		else if (getTruckController()->getShotGun().isObjectInRange(truckRect, opRect))
		{
			if (getTruckController()->getShotGun().isReadyToShoot() && chance(rng) < deltaTime.realSeconds())
			{
				setInteracted(true);
				shoot = true;
				getTruckController()->getShotGun().shoot();
				getTruckController()->getTruck()->activateAttachedAnimation(Truck::ShotgunBlast);
				// TODO: If we want to, this is where an opponent shotgun sound would go.
			}
		}
	}
} // namespace rr
