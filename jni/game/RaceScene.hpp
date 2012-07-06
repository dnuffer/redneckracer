// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef RACE_SCENE_HPP_INCLUDED
#define RACE_SCENE_HPP_INCLUDED

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "GameLibrary.hpp"
#include "engine/EngineFwd.hpp"
#include "engine/Scene.hpp"
#include "miniblocxx/stl/vector"
#include <vector>
#include "TruckController.hpp"
#include "RoadBound.hpp"
#include "RacePosAction.hpp"
#include "RaceTracks.hpp"


#include "RedneckRacerGame.hpp"


namespace rr
{
	
	class RaceScene : public Scene
	{
	public:

		RaceScene(GameLibrary& gameLibrary, float& rollAngle, const BestTimesPtr& bestTimes);
		virtual ~RaceScene();
		
		virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
		
		virtual void handleTouchEvent(const TouchEvent& touchEvent);

		void setRaceTrack(RaceTracks::Races raceTrack);
		void restartRace();

		static const int backgroundZOrder = -1;
		static const int obstacleZOrder = 10;
		static const int animalZOrder = 20;
		static const int truckZOrder = 30;
		//should this be changed to effectsZOrder?
		static const int sparksZOrder = 40;
		static const int hudZOrder = 100;
		Point translateScreenPointToRacePoint(const Point& screenPoint) const;
		
	private:
		// Callback functions to interact with brake button.
		void brakeActionDown(const TouchHandler&);
		void brakeActionUp(const TouchHandler&);

		// Callback function to interact with menu button.
		void menuActionUp(const TouchHandler&);

		// Callback functions to interact with shot button.
		void shotActionDown(const TouchHandler&);
		void shotActionUp(const TouchHandler&);

		// Callback functions to interact with nitro button.
		void nitroActionDown(const TouchHandler&);

		// Callback functions to interact with feedback buttons.
		void feedbackRateActionUp(const TouchHandler&);
		void feedbackLaterActionUp(const TouchHandler&);
		void feedbackNeverActionUp(const TouchHandler&);

		size_t raceLength() const;
		void addAnimals(GameLibrary::AnimalSpecies);

		enum ObstaclePosition
		{
			OP_OnRoad = 0,
			OP_OffRoad,
			OP_Any
		};
		void addCivilCars();
		void addObstacles(GameLibrary::ObstacleType, ObstaclePosition position, int zOrder = obstacleZOrder);
		void addButtons();
		void addMeters();
		void addTrucksControllers();
		void startNearbyAnimalsMoving();
		void removeOffScreenStuff();
		
	private:

		void prepareButton(TouchButtonPtr button);
		void resetElapsedTime();
		void initRaceScene();
		void addTruckTouchHandler(TruckControllerPtr truck);

		void startScreen(const TimeDuration& deltaTime);
		void finishScreen(const TimeDuration& deltaTime);
		void feedbackScreen(const TimeDuration& deltaTime);
		void hideFeedbackScreen();
		void addInfoLabels(unsigned int labelsCount, float firstLabelYpos);

		// Return true if user give feedback or select never give feedback.
		bool checkUserFeedback() const;
		void saveFeedback() const;

		struct TrackSection
		{
			Array<TexturedQuadPtr> subSections;
		};
		GameLibrary& gameLibrary;
		std::vector<SpritePtr> backgrounds;
		std::vector<TrackSection> sections;
		
		std::vector<CivilCarPtr> civilCars;
		std::vector<PoliceTruckPtr> policeTrucks;
		PlayerTruckPtr playerTruck;
		std::vector<OpponentTruckPtr> opponentTrucks;
		//SpritePtr copSuv;
		std::vector<ObstaclePtr> obstacles;
		std::vector<AnimalPtr> animals;
		std::vector<TouchButtonPtr> touchButtons;
		SpritePtr healthAndRageMeter;
		SpritePtr finishLine;

		TruckControllerPtr playerTruckController;
		vector<TruckControllerPtr> trucksControllers; // For opponents, police and civil cars.
		vector<TouchHandlerPtr> truckTouchHandlers;
		vector<DrivingAIPtr> opponentsAi;

		ProgressBarPtr playerTruckDefense;
		ProgressBarPtr playerTruckArmor;
		ProgressBarPtr playerTruckRage;
		
		ProgressBarPtr infoLabelsBackground; // Use progressbar like a simple colored rectangle.
		vector<LabelPtr> infoLabels; // For showing information at start and at finish.
		vector<TouchButtonPtr> finishButtons; // For storing restart and back to menu buttons, appearing after finish.
		vector<TouchButtonPtr> feedbackButtons;
		TimeDuration infoLabelShowingTime;

		RoadBoundPtr roadBound;

		RacePosActionPtr racePosAction;


		enum RaceState
		{
			// Transition to RS_RACE_ACTIVE after 5 seconds.
			RS_WAIT_FOR_START,
			
			// Transition to RS_RACE_FINISHED after finish line.
			RS_RACE_ACTIVE,

			// No transitions
			RS_RACE_FINISHED
		};
		
		RaceTracks raceTracks;
		RaceTracks::Races currentRaceTrack;
		RaceState state;
		TimeDuration elapsedTime;
		float& rollAngle;
		bool isRestarted;
		unsigned int raceFinishCount;
		bool showFeedbackPromt;
		BestTimesPtr m_bestTimes;
	};
	
}

#endif

