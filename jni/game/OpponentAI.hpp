// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __OPPONENTAI_HPP__
#define __OPPONENTAI_HPP__

#include "DrivingAI.hpp"
#include "miniblocxx/TimeDuration.hpp"


namespace rr
{
	class OpponentAI: public DrivingAI
	{
	public:
		OpponentAI(TruckControllerPtr controller, RoadBoundPtr road):
			  DrivingAI(controller, road)
            , shoot(false)
			, interacted(false)
		{}

	protected:
		virtual void processOtherTrucks(const Rectangle& truckRect, vector<TruckControllerPtr>& trucksControllers, 
										const TimeDuration& deltaTime);
		virtual void interactWithOtherTruck(const Rectangle& truckRect, const Rectangle& opponentTruckRect,
											const TimeDuration& deltaTime);
		bool isInteracted() const { return interacted; }
		void setInteracted(bool val) { interacted = val; }

		static bool isOpponentBelow(const Rectangle& truckRect, const Rectangle& opponentTruckRect)
		{
			if (truckRect.bottom > opponentTruckRect.top) return true;
			else return false;
		}

		static bool isOpponentUpper(const Rectangle& truckRect, const Rectangle& opponentTruckRect)
		{
			if (truckRect.top < opponentTruckRect.bottom) return true;
			else return false;
		}

		static bool isOpponentLeft(const Rectangle& truckRect, const Rectangle& opponentTruckRect)
		{
			if (truckRect.left > opponentTruckRect.right) return true;
			else return false;
		}

		static bool isOpponentRight(const Rectangle& truckRect, const Rectangle& opponentTruckRect)
		{
			if (truckRect.right < opponentTruckRect.left) return true;
			else return false;
		}

		static const float hitRange = 40.0f;
		bool shoot;
	private:
		bool interacted;

	};
}


#endif // __OPPONENTAI_HPP__
