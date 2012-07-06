// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "DrivingAI.hpp"
#include "Truck.hpp"
#include "engine/Log.hpp"
#include "boost/random.hpp"

namespace rr
{

	static boost::mt19937 rng(::time(NULL));

	DrivingAI::DrivingAI(TruckControllerPtr controller, RoadBoundPtr road):
		  truckController(controller)
		, roadBound(road)
		, currentIndexInRoadBound(0)
		, posOnRoad(P_Random)
	{
		const Rectangle& truckRect = controller->getTruck()->getBoundingRect();
		truckWidth = (truckRect.right - truckRect.left) * 1.6f;

		// Random init turnAccuracy
		 boost::uniform_real<float> accur(0, 0.3f);
		 boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > gen(rng, accur);
		 turningAccuracy = gen();

		 LOGI("Turning accuracy set to %f", turningAccuracy);
	}

	void DrivingAI::update(const TimeDuration& deltaTime, vector<TruckControllerPtr>& trucksControllers)
	{
		const Rectangle& truckRect = truckController->getTruck()->getBoundingRect();
		stayOnRoad(deltaTime, truckRect);
		processOtherTrucks(truckRect, trucksControllers, deltaTime);
	}

	void DrivingAI::stayOnRoad(const TimeDuration& deltaTime, const Rectangle& truckRect)
	{
		// Get road near coordinates.
		while (roadBound->getLeftBoundByIndex(currentIndexInRoadBound).y() < truckRect.top && (roadBound->getBoundIndexSize() > currentIndexInRoadBound))
		{
			++currentIndexInRoadBound;
		}

		slowdownCoeff =  fabs(roadBound->getLeftBoundByIndex(currentIndexInRoadBound+1).x() -
				roadBound->getLeftBoundByIndex(currentIndexInRoadBound).x()) / 15;
		if (slowdownCoeff < 0.1f) slowdownCoeff = 0.1f;

		// If civilian cars are getting off track, the only thing I can find (at the moment) that
		//  might solve the problem is the use of only x() in the following switch statement.
		//  I'm inclined to think that x() == y(), but that's just my understanding.
		switch (posOnRoad)
		{
			case P_Left:
			{
				leftBorder = roadBound->getLeftBoundByIndex(currentIndexInRoadBound+1).x();
				rightBorder = (leftBorder + roadBound->getRightBoundByIndex(currentIndexInRoadBound+1).x()) / 2;
				if ((rightBorder - leftBorder) < truckWidth)
				{
					rightBorder = leftBorder + truckWidth;
				}
				break;
			}
			case P_Right:
			{
				rightBorder = roadBound->getRightBoundByIndex(currentIndexInRoadBound+1).x();
				leftBorder = (rightBorder + roadBound->getLeftBoundByIndex(currentIndexInRoadBound+1).x() ) / 2;
				if ((rightBorder - leftBorder) < truckWidth)
				{
					leftBorder = rightBorder - truckWidth;
				}
				break;
			}
			case P_Random:
			default:
			{
				rightBorder = roadBound->getRightBoundByIndex(currentIndexInRoadBound+1).x();
				leftBorder = roadBound->getLeftBoundByIndex(currentIndexInRoadBound+1).x();
				break;
			}
		}

		// Slowdown/accelerate truck, when it needed.
		if (truckController->getTruck()->truckParams.truckSpeed > TruckController::defaultMaxSpeed / slowdownCoeff)
		{
			slowdownTruck();
		}
		else
		{
			accelerateTruck();
		}

		turningTime += deltaTime;
		// Turn truck, when it needed.
		if (truckRect.left <= leftBorder)
		{
			turningTime = TimeDuration();
			truckController->getTruck()->setInputRight(true); // Driving right back to road.
		}
		else if (truckRect.right >= rightBorder)
		{
			turningTime = TimeDuration();
			truckController->getTruck()->setInputLeft(true); // Driving left back to road.
		}
		else
		{
			if (turningTime.realSeconds() > turningAccuracy)
			{
				truckController->getTruck()->setInputLeft(false);  //Reset all turning actions.
				accelerateTruck();
			}
		}
	}


	void DrivingAI::slowdownTruck()
	{
		if (!truckController->isBraking()) truckController->brakeActionDown();
	}
	void DrivingAI::accelerateTruck()
	{
		if (truckController->isBraking()) truckController->brakeActionUp();
	}

} // namespace rr
