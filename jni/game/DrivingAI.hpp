// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __DRIVINGAI_HPP__
#define __DRIVINGAI_HPP__

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "TruckController.hpp"
#include "RoadBound.hpp"

namespace rr
{
	class DrivingAI: public virtual IntrusiveCountableBase
	{
	public:
		DrivingAI(TruckControllerPtr controller, RoadBoundPtr road);
		enum PositionOnRoad
		{
			P_Left = 0,
			P_Right,
			P_Random
		};

		virtual ~DrivingAI(){}
		virtual void update(const TimeDuration& deltaTime, vector<TruckControllerPtr>& trucksControllers);
		virtual void setPositionOnRoad(PositionOnRoad position) { posOnRoad = position; }
		TruckControllerPtr& getTruckController() { return truckController; }

	protected:
		virtual void processOtherTrucks(const Rectangle& truckRect, vector<TruckControllerPtr>& trucksControllers, 
										const TimeDuration& deltaTime) {}
		virtual void slowdownTruck();
		virtual void accelerateTruck();
		virtual void stayOnRoad(const TimeDuration& deltaTime, const Rectangle& truckRect);

	protected:
		TruckControllerPtr truckController;

	private:
		RoadBoundPtr roadBound;
		unsigned int currentIndexInRoadBound;
		PositionOnRoad posOnRoad;
		float slowdownCoeff;
		float leftBorder;
		float rightBorder;
		float truckWidth;
		float turningAccuracy;
		TimeDuration turningTime;

	};
} // namespace rr

#endif// __DRIVINGAI_HPP__
