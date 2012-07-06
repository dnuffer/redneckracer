// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef rr_game_RRFwd_hpp_INCLUDED_
#define rr_game_RRFwd_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "engine/EngineFwd.hpp"

namespace rr
{
	class Truck;
	typedef boost::intrusive_ptr<Truck> TruckPtr;

	class PlayerTruck;
	typedef boost::intrusive_ptr<PlayerTruck> PlayerTruckPtr;

	class OpponentTruck;
	typedef boost::intrusive_ptr<OpponentTruck> OpponentTruckPtr;

	class Animal;
	typedef boost::intrusive_ptr<Animal> AnimalPtr;

	class Obstacle;
	typedef boost::intrusive_ptr<Obstacle> ObstaclePtr;

	class GameLibrary;

	class CivilCar;
	typedef boost::intrusive_ptr<CivilCar> CivilCarPtr;

	class PoliceTruck;
	typedef boost::intrusive_ptr<PoliceTruck> PoliceTruckPtr;

	class TruckController;
	typedef boost::intrusive_ptr<TruckController> TruckControllerPtr;

	class RoadBound;
	typedef boost::intrusive_ptr<RoadBound> RoadBoundPtr;

	class DrivingAI;
	typedef boost::intrusive_ptr<DrivingAI> DrivingAIPtr;

	class RacePosAction;
	typedef boost::intrusive_ptr<RacePosAction> RacePosActionPtr;

	class LoadingScreenScene;
	typedef boost::intrusive_ptr<LoadingScreenScene> LoadingScreenScenePtr;
    
    class MainMenuScene;
    typedef boost::intrusive_ptr<MainMenuScene> MainMenuScenePtr;
	
	class RaceScene;
	typedef boost::intrusive_ptr<RaceScene> RaceScenePtr;
	
	class TruckTouchHandler;
	typedef boost::intrusive_ptr<TruckTouchHandler> TruckTouchHandlerPtr;
	
	class BestTimes;
	typedef boost::intrusive_ptr<BestTimes> BestTimesPtr;
}

#endif
