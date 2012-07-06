// Copyright 2011 Nuffer Brothers Software LLC

#include "TruckTouchHandler.hpp"
#include "Truck.hpp"

namespace rr
{	
	TruckTouchHandler::TruckTouchHandler(TruckControllerPtr tc, const RaceScene& rs)
	: truckController(tc),
	  raceScene(rs)
	{}
	
	bool TruckTouchHandler::checkHit(float screenX, float screenY)
	{
		Point point = raceScene.translateScreenPointToRacePoint(Point(screenX,screenY));
		return truckController->getTruck()->containsPoint(point);
	}
	
	TruckControllerPtr TruckTouchHandler::getController()
	{
		return truckController;
	}
}
