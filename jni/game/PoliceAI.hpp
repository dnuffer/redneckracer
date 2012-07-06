// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __POLICEAI_HPP__
#define __POLICEAI_HPP__

#include "OpponentAI.hpp"

namespace rr
{
	class PoliceAI: public OpponentAI
	{
	public:
		PoliceAI(TruckControllerPtr controller, RoadBoundPtr road):
					  OpponentAI(controller, road)
		{shoot = true;}
		void update(const TimeDuration& deltaTime, vector<TruckControllerPtr>& trucksControllers);
	private:
        void attackFirstPlaceTruck(vector<TruckControllerPtr>& trucksControllers);
	};
}


#endif // __OPPONENTAI_HPP__
