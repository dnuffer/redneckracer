// Copyright 2011 Nuffer Brothers Software LLC

#ifndef _game_TRUCK_TOUCH_HANDLER_HPP_
#define _game_TRUCK_TOUCH_HANDLER_HPP_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "TruckController.hpp"
#include "GameLibrary.hpp"
#include "RaceScene.hpp"

#include "engine/TouchHandler.hpp"
#include <map>
#include <tr1/functional>


namespace rr
{	
	class TruckTouchHandler: public TouchHandler
	{
	public:
		TruckTouchHandler(TruckControllerPtr tc, const RaceScene& rs);
		~TruckTouchHandler(){};
		
		bool checkHit(float screenX, float screenY);
		
		TruckControllerPtr getController();
		
	private:
		TouchState touchState;
		TruckControllerPtr truckController;
		const RaceScene& raceScene;
	};

}

#endif // _game_TRUCK_TOUCH_HANDLER_HPP_
