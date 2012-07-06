// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "PoliceAI.hpp"
#include "boost/foreach.hpp"
#include "TruckController.hpp"
#include "Truck.hpp"
#include "engine/Log.hpp"
#include "CivilCar.hpp"
#include "OpponentTruck.hpp"
#include "ShotGun.hpp"

#define foreach BOOST_FOREACH


namespace rr
{
	void PoliceAI::update(const TimeDuration& deltaTime, vector<TruckControllerPtr>& trucksControllers)
	{
		const Rectangle& truckRect = truckController->getTruck()->getBoundingRect();
		stayOnRoad(deltaTime, truckRect);
		processOtherTrucks(truckRect, trucksControllers, deltaTime);
		attackFirstPlaceTruck(trucksControllers);
	}

	void PoliceAI::attackFirstPlaceTruck(vector<TruckControllerPtr>& trucksControllers)
	{
		TruckControllerPtr firstPlace = trucksControllers[0];

		foreach(TruckControllerPtr tc, trucksControllers)
		{
			if(tc->getTruck()->isRacer())
			{
				if(tc->getTruck()->position().y() > firstPlace->getTruck()->position().y())
					firstPlace = tc;
			}
		}

		Point firstPos = firstPlace->getTruck()->position();
		Point policePos = truckController->getTruck()->position();
		if (firstPos.y() - policePos.y() > 200)
		{
			accelerateTruck();
		}
		else if(firstPos.y() - policePos.y() < -120)
		{
			slowdownTruck();
		}
		else if (truckController->getShotGun().isObjectInRange(truckController->getTruck()->getBoundingRect(), firstPlace->getTruck()->getBoundingRect()))
		{
			if (truckController->getShotGun().isReadyToShoot())
			{
				//LOGD("KABLOOIEEEEEEEE!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
				truckController->getTruck()->activateAttachedAnimation(Truck::ShotgunBlast);
				truckController->getShotGun().shoot();
				truckController->getTruck()->activateAttachedAnimation(Truck::ShotgunBlast);
				firstPlace->getTruck()->setAttachedAnimationPosition(Truck::Sparks, Point(0, -(firstPlace->getTruck()->size().height()) / 2));
				firstPlace->getTruck()->activateAttachedAnimation(Truck::Sparks);
			}
		}
	}

} // namespace rr
