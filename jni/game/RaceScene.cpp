// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "RaceScene.hpp"
#include "Truck.hpp"
#include "TrackBorder.hpp"
#include "PlayerTruck.hpp"
#include "OpponentTruck.hpp"
#include "Animal.hpp"
#include "Obstacle.hpp"
#include "miniblocxx/Format.hpp"
#include "miniblocxx/Enumerator.hpp"
#include "engine/MoveAction.hpp"
#include "engine/AccelerateAction.hpp"
#include "engine/Sprite.hpp"
#include "engine/TextureLibrary.hpp"
#include "engine/Animation.hpp"
#include "Globals.hpp"
#include "engine/RotateAction.hpp"

#include "boost/range/algorithm_ext/push_back.hpp"
#include "boost/range/irange.hpp"
#include "boost/range/algorithm/random_shuffle.hpp"
#include <map>

#include "engine/TexturedFont.hpp"
#include "engine/Label.hpp"
#include "FPSAction.hpp"
#include "engine/Rectangle.hpp"
#include "boost/foreach.hpp"
#define foreach BOOST_FOREACH

#include "boost/random.hpp"
#include "engine/Log.hpp"
#include "CivilCar.hpp"
#include "engine/ProgressBar.hpp"
#include "DrivingAI.hpp"
#include "OpponentAI.hpp"
#include "PoliceAI.hpp"
#include "MainMenuScene.hpp"
#include <fstream>
#include "boost/bind.hpp"
#include "TruckTouchHandler.hpp"
#include "graphlib/GenericVector2.hpp"
#include "RaceTracks.hpp"
#include "BestTimes.hpp"


namespace rr
{
	using namespace boost;

	namespace
	{
		const string trackGroup_STR = "track";

		void progress(float f)
		{
			//LOGD("We're making progress! Currently at %02.2f%%", f*100);
		}

		const float turnDelay = 0.25;
		const float turnAngle = 25.0;
		const int TruckCenterVerticalOffset = 133;

		rand48 randEng(::time(NULL));

		void extractNumbers(const String& text, Array<int>& numbers)
		{
			numbers.clear();

			StringArray tokens = text.tokenize("_.");
			for( size_t i = 0; i < tokens.size(); ++i )
			{
				try
				{
					numbers.append(tokens[i].toInt());
				}
				catch(const StringConversionException& e)
				{
					// expected for most of the string.  Yes, this is evil and java-style.
				}
			}
		}

		Rectangle leftBorderRect(int numBackgroundSections)
		{
			return Rectangle(
				DefaultScreenLeft - DefaultScaleWidth * 100,
				DefaultScreenLeft + 20,
				DefaultScreenTop + DefaultScaleHeight * numBackgroundSections,
				DefaultScreenBottom);
		}

		Rectangle rightBorderRect(int numBackgroundSections)
		{
			return Rectangle(
				DefaultScreenRight - 20,
				DefaultScreenRight + DefaultScaleWidth * 100,
				DefaultScreenTop + DefaultScaleHeight * numBackgroundSections,
				DefaultScreenBottom);
		}

		String getPositionDescription(int position)
		{
			switch (position)
			{
				case 1: return "1st";
				case 2: return "2nd";
				case 3: return "3rd";
				default: return String(position) + "th";
			}
		}
	}

	RaceScene::RaceScene(GameLibrary& gameLibrary, float& rollAngle, const BestTimesPtr& bestTimes)
	: gameLibrary(gameLibrary)
	, sections()
	, state(RS_WAIT_FOR_START)
	, rollAngle(rollAngle)
	, isRestarted(false)
	, raceFinishCount(0)
	, m_bestTimes(bestTimes)
	{
		gameLibrary.loadRaceAtlases();
		setRaceTrack(RaceTracks::YANKEESHOT);
	}
	
	void RaceScene::setRaceTrack(RaceTracks::Races raceTrack)
	{
		currentRaceTrack = raceTrack;
	}

	void RaceScene::restartRace()
	{
		// Stop sounds
		gameLibrary.truckStartupSound()->stop();
        gameLibrary.copsComingSound()->stop();
		foreach (PoliceTruckPtr t, policeTrucks)
			t->stopSounds();
		foreach (OpponentTruckPtr t, opponentTrucks)
			t->stopSounds();
		foreach (CivilCarPtr t, civilCars)
			t->stopSounds();
		foreach (ObstaclePtr o, obstacles)
			o->stopSounds();
		foreach (AnimalPtr a, animals)
			a->stopSounds();
		
		// CleanUp.
		backgrounds.clear();
		sections.clear();
		civilCars.clear();
		policeTrucks.clear();
		opponentTrucks.clear();
		obstacles.clear();
		animals.clear();

		trucksControllers.clear();
		truckTouchHandlers.clear();
		opponentsAi.clear();
		infoLabels.clear();

		state = RS_WAIT_FOR_START;

		removeAllChildren();

		foreach(TouchButtonPtr button, finishButtons)
		{
			touchButtons.erase(remove_if(touchButtons.begin(), touchButtons.end(), bind2nd(equal_to<TouchButtonPtr>(), button)), touchButtons.end());
		}
		initRaceScene();
		elapsedTime = TimeDuration();

		isRestarted = true;
	}

	void RaceScene::initRaceScene()
	{

		racePosAction = new RacePosAction("Pos: ");;

		trucksControllers.reserve(12);

		StringArray backgroundQuads = gameLibrary.listBackgroundQuads();
		// Just make sure things are somewhat sorted.
		std::sort(backgroundQuads.begin(), backgroundQuads.end());

		// Load all of the textures for the quads matching the above filter.
		std::map<int, TrackSection> sectionMap;
		for( size_t quadIndex = 0; quadIndex < backgroundQuads.size(); ++quadIndex )
		{
			String name = backgroundQuads[quadIndex];
			LOGD("adding some sections/subsections for quad %s", name.c_str());

			Array<int> numbers;
			extractNumbers(name, numbers);
			// Assume for now that numbers[0] == section, numbers[1] = subsection.  This may need to be changed.
			assert(numbers.size() == 2);
			unsigned section = numbers[0];
			unsigned subSection = numbers[1];

			TexturedQuadPtr texture = gameLibrary.backgroundQuad(name);

			TrackSection& trackSection(sectionMap[section-1]);
			if( trackSection.subSections.size() < subSection )
			{
				trackSection.subSections.resize(subSection);
			}
			trackSection.subSections[subSection-1] = texture;
		}
		sections.reserve(sectionMap.size());
		for( std::map<int, TrackSection>::const_iterator iter = sectionMap.begin(); iter != sectionMap.end(); ++iter )
		{
			sections.push_back(iter->second);
		}

		if (currentRaceTrack == RaceTracks::WIJADIDJA) {
			raceTracks.generateRandomTrack(unsigned(sections.size()));
		}

		int yPosition = 0;
		LOGD("currentRaceTrack size %zu", raceTracks.getTrackSize(currentRaceTrack));

		// Prepare road bound
		roadBound = new RoadBound(DefaultScreenRight, DefaultScreenTop);
		roadBound->setName("RoadBound");

		vector<char> track_sequence = raceTracks.getRaceTrack(currentRaceTrack);
		for (size_t i = 0; i < track_sequence.size(); ++i)
		{
			const TrackSection& trackSection(sections[track_sequence[i]]);
			for (size_t subSection = 0; subSection < trackSection.subSections.size(); ++subSection)
			{
				// FIXME! Bad assumption about sprite names.
				SpritePtr sprite(new Sprite(trackSection.subSections[subSection], Format("background_grassy_%1_%2", track_sequence[i] + 1, subSection + 1)));
				roadBound->loadRoadSectionBorder(Format("background_grassy_%1_%2.road", track_sequence[i] + 1, subSection + 1));

				Point position = sprite->position();
				position.y() = yPosition;
				sprite->setPosition(position);
				yPosition += sprite->size().height();
				backgrounds.push_back(sprite);
				addChild(sprite, backgroundZOrder);
				LOGD("added child sprite %zu of section %d.%zu", i, track_sequence[i], subSection);
			}
		}
		// finish line
		SpritePtr finishLineBackground = new Sprite(sections[0].subSections[0], "background_grassy_0_0-finish");
		finishLineBackground->setPosition(Point(0, yPosition));
		addChild(finishLineBackground, backgroundZOrder);
		finishLine = gameLibrary.finishLine();
		finishLine->setPosition(Point(5, yPosition - 350));
		addChild(finishLine);

		TrackBorderPtr left = new TrackBorder(leftBorderRect(backgrounds.size()));
		TrackBorderPtr right = new TrackBorder(rightBorderRect(backgrounds.size()));
		left->setName("LeftBorder");
		left->setName("RightBorder");

		addCollidableChild(left);
		addCollidableChild(right);

		addCollidableChild(roadBound);

		assert(!roadBound->isPointOnRoad(40,yPosition-DefaultScreenTop+1));
		assert(roadBound->isPointOnRoad(40,yPosition-DefaultScreenTop));
		assert(roadBound->isPointOnRoad(40,yPosition-DefaultScreenTop-1));
#ifdef TEST_DRAWING_ROAD_BOUND
		// Temp for testing road border.
		addChild(roadBound);
#endif

		// truck starts out static
		playerTruck = gameLibrary.playerTruck(rollAngle);
		playerTruck->setPosition(Point(40, -133));
		playerTruck->setName("PlayerTruck");
		addCollidableChild(playerTruck);
		addChild(playerTruck, truckZOrder);

		GameLibrary::TruckColor truckColors[] = { GameLibrary::Black, GameLibrary::Blue, GameLibrary::Brown, GameLibrary::Green, GameLibrary::Yellow };
		Point truckPositions[] = {
									roadBound->getPointOnRoad(0),
									roadBound->getPointOnRoad(133),
									roadBound->getPointOnRoad(266),
									roadBound->getPointOnRoad(400),
									roadBound->getPointOnRoad(533)
								 };

		for( unsigned i = 0; i < sizeof(truckColors) / sizeof(*truckColors); ++i )
		{
			OpponentTruckPtr truckSprite = gameLibrary.opponentTruck(truckColors[i]);
			truckSprite->setName(Format("Opponent%1", (int)truckColors[i]));
			opponentTrucks.push_back(truckSprite);
			truckSprite->setPosition(truckPositions[i]);
			addChild(truckSprite, truckZOrder);
			addCollidableChild(truckSprite);
			// Update player truck race position at start.
			playerTruck->updateBoundingRect();
			racePosAction->updateRacePosition(playerTruck->getBoundingRect().top, truckSprite->getBoundingRect().top);
		}

		// Set number of trucks in race (+1 - player truck).
		racePosAction->setPostfix("/" + String(opponentTrucks.size() + 1));

		addCivilCars();

		PoliceTruckPtr policeSprite = gameLibrary.policeTruck();
		policeSprite->setName("Police");
		policeSprite->attachAnimation(Truck::ShotgunBlast, gameLibrary.effect(GameLibrary::ShotgunBlast), Point(-30,18));
		policeTrucks.push_back(policeSprite);
		policeSprite->setPosition(Point(0, -500));
		addChild(policeSprite, truckZOrder);
		addCollidableChild(policeSprite);


		#ifdef DEBUG
		LabelPtr label = new Label(gameLibrary.theMilkmanConspiracyFont());
		label->setText("00.0");
		label->addAction(new FPSAction(""));
		label->setPositionInterpretation(Drawable::E_SCREEN);
		label->setPosition(Point(0, -380));
		addChild(label, hudZOrder);
		#endif
		
		addAnimals(GameLibrary::Armadillo);
		addAnimals(GameLibrary::Snake);
		addAnimals(GameLibrary::Possum);
		addAnimals(GameLibrary::Raccoon);
		addAnimals(GameLibrary::Squirrel);

		addObstacles(GameLibrary::Puddle, OP_OnRoad);
		addObstacles(GameLibrary::Pothole, OP_OnRoad);
		addObstacles(GameLibrary::FatGuy, OP_OffRoad);
		addObstacles(GameLibrary::Outhouse, OP_OffRoad);
		addObstacles(GameLibrary::Shrub, OP_OffRoad);
		addObstacles(GameLibrary::Tree, OP_OffRoad/*, truckZOrder + 1*/);
		addObstacles(GameLibrary::Tractor, OP_Any);

		playerTruck->attachAnimation(Truck::ShotgunBlast, gameLibrary.effect(GameLibrary::ShotgunBlast), Point(-30, 18));
		// Hide blast for first loading by setting last(clear) frame.
		playerTruck->getAttachedAnimationSprite(Truck::ShotgunBlast)->update(DateTime(0), TimeDuration(15.0));
		// Attach shotgun animation to oppponents trucks.
		foreach(TruckPtr t, opponentTrucks)
		{
			t->attachAnimation(Truck::ShotgunBlast, gameLibrary.effect(GameLibrary::ShotgunBlast), Point(-30, 18));
			t->getAttachedAnimationSprite(Truck::ShotgunBlast)->update(DateTime(0), TimeDuration(15.0));
		}

		addTrucksControllers();

		addButtons();

		addMeters();

		addInfoLabels(1, 0);

		//infoLabels.at(0)->setText("Prepare to race!");

        gameLibrary.truckStartupSound()->play();
        gameLibrary.copsComingSound()->play();

		startScreen(TimeDuration(0.0f));

		isRestarted = true;

		// Checking previous feedback status (saved in internal memory)
		showFeedbackPromt = !checkUserFeedback();

	}
	
	void RaceScene::addTruckTouchHandler(TruckControllerPtr truck)
	{
		TruckTouchHandlerPtr tth = new TruckTouchHandler(truck,*this);
		tth->getTouchedEvent().connect(boost::bind(&RaceScene::shotActionDown, this, _1));
		tth->getUnTouchedEvent().connect(boost::bind(&RaceScene::shotActionUp, this, _1));
		truckTouchHandlers.push_back(tth);
	}


	void RaceScene::addCivilCars()
	{
		GameLibrary::TruckColor truckColors[] = { GameLibrary::Black, GameLibrary::Blue, GameLibrary::Brown, GameLibrary::Green, GameLibrary::Yellow };
		const int minNumCars = 2;
		const int maxNumCars = 5;
		uniform_int<int> carNumRD(minNumCars, maxNumCars);
		uniform_int<int> verticalRD(DefaultScaleHeight * 2, (raceLength() - DefaultScaleHeight) / 2);
		int num = carNumRD(randEng);
		for (int i = 0; i < num; ++i)
		{
			CivilCarPtr carSprite = gameLibrary.civilCar(truckColors[i]);
			civilCars.push_back(carSprite);
			carSprite->setPosition(roadBound->getPointOnRoad(verticalRD(randEng)));
			carSprite->setName(Format("NPC%1", (int)truckColors[i]));
			addCollidableChild(carSprite);
			addChild(carSprite, truckZOrder);
		}

	}

	void RaceScene::addAnimals(GameLibrary::AnimalSpecies species)
	{
		int minNumAnimals = raceTracks.getMinAnimals(currentRaceTrack);
		int maxNumAnimals = raceTracks.getMaxAnimals(currentRaceTrack);
		uniform_int<int> animalNumRD(minNumAnimals, maxNumAnimals);

		// 0 is E_LEFT, 1 is E_RIGHT
		uniform_int<int> directionRD(0, 1);

		// +/- 50 so some animals can be a little bit off the screen initially.
		uniform_int<int> leftSideRD(DefaultScreenLeft - 50, 0);
		uniform_int<int> rightSideRD(0, DefaultScreenRight + 50);

		// Restrict by DefaultScaleHeight on either side to prevent animals on the first or last screen.
		uniform_int<int> verticalRD(DefaultScaleHeight, raceLength() - DefaultScaleHeight);
		// temp: only put them on the third screen
		//uniform_int<int> verticalRD(DefaultScaleHeight * 1, DefaultScaleHeight * 2);

		int num = animalNumRD(randEng);
		for (int i = 0; i < num; ++i)
		{
			Animal::EMovementDirection direction(directionRD(randEng) == 0 ? Animal::E_LEFT : Animal::E_RIGHT);
			AnimalPtr animal = gameLibrary.animal(species, direction);
			animal->setPosition(Point(direction == Animal::E_RIGHT ? leftSideRD(randEng) : rightSideRD(randEng),
									  verticalRD(randEng)));
			animal->setName(Format("animal%1-%2", int(species), i));
			animals.push_back(animal);
			addChild(animal, animalZOrder);
			addCollidableChild(animal);
		}
	}

	void RaceScene::addObstacles(GameLibrary::ObstacleType type, ObstaclePosition positionInterpretation, int zOrder)
	{
		int minNumObstacles = raceTracks.getMinObstacles(currentRaceTrack);
		int maxNumObstacles = raceTracks.getMaxObstacles(currentRaceTrack);
		uniform_int<int> obstacleNumRD(minNumObstacles, maxNumObstacles);
		// +/- 50 so nothing is half off the screen
		uniform_int<int> horizontalRD;//(DefaultScreenLeft + 50, DefaultScreenRight - 50);

		// Restrict by DefaultScaleHeight on either side to prevent animals on the first or last screen.
		uniform_int<int> verticalRD(DefaultScaleHeight, raceLength() - DefaultScaleHeight);
		// temp: only put them on the second screen
		//uniform_int<int> verticalRD(DefaultScaleHeight * 2, DefaultScaleHeight * 3);

		int num = obstacleNumRD(randEng);
		float yPosition(0.0f);
		Coord bound(0,0);
		for (int i = 0; i < num; ++i)
		{
			ObstaclePtr obstacle = gameLibrary.obstacle(type);
			float width = obstacle->size().width();
			yPosition = verticalRD(randEng);
			switch (positionInterpretation)
			{
				case OP_OnRoad:
				{
					bound = roadBound->getBoundCoordinates(yPosition);
					horizontalRD = uniform_int<int>(
						std::max<int>(0, bound.x() + 10),
						std::max<int>(0, bound.y() - 10)); // !Road width need to be minimum 20 pixels.
					break;
				}
				case OP_OffRoad:
				{
					bound = roadBound->getBoundCoordinates(yPosition);
					uniform_int<int> leftOrRight(0,1);
					int left = DefaultScreenLeft + 50;
					int right = bound.x() - width / 2;
					if (type == GameLibrary::FatGuy)// FIXME!!!  Should not hard code solutions.
					{// Adjustment for FatGuy
						left += width / 4;
						right += width / 2;
					}
					if (leftOrRight(randEng) == 0)
					{
						left = bound.y() + width / 2;
						right = DefaultScreenRight - 50;
						if (type == GameLibrary::FatGuy)
						{// Adjustment for FatGuy
							left += width / 6;
							right += 50;
						}
					}
					if ((left >= right) || (right > DefaultScreenRight))
					{
						--i;
						continue;
					}
					horizontalRD = uniform_int<int>(right - 1, right);
					break;
				}
				case OP_Any:
				default:
				{
					horizontalRD = uniform_int<int>(DefaultScreenLeft + 50, DefaultScreenRight - 50);
					break;
				}
			}
			obstacle->setPosition(Point(horizontalRD(randEng), yPosition));
			obstacle->setName(Format("obstacle%1-%2", int(type), i));
			obstacles.push_back(obstacle);
			addChild(obstacle, zOrder);
			addCollidableChild(obstacle);
		}
	}


	void RaceScene::prepareButton(TouchButtonPtr button)
	{
		button->setPositionInterpretation(Drawable::E_SCREEN);
		touchButtons.push_back(button);
		addChild(button, hudZOrder);
	}

	void RaceScene::addButtons()
	{
		if (touchButtons.size() == 0) // Don't create after race restarting.
		{
			/*
			// Brake pedal.
			TouchButtonPtr brakeButton = gameLibrary.button(GameLibrary::Brake);
			Rectangle rect = brakeButton->getBoundingRect();
			brakeButton->setPosition(Point(
					DefaultScreenRightPad - rect.width() / 2,
					DefaultScreenBottomPad + rect.height() / 2));

			brakeButton->addStateAction(TouchHandler::Touched, boost::bind(&RaceScene::brakeActionDown, this, _1));
			brakeButton->addStateAction(TouchHandler::Untouched, boost::bind(&RaceScene::brakeActionUp, this, _1));
			brakeButton->getTouchedEvent().connect(boost::bind(&RaceScene::brakeActionDown, this, _1));
			brakeButton->getUnTouchedEvent().connect(boost::bind(&RaceScene::brakeActionUp, this, _1));
			prepareButton(brakeButton);
			*/
			
			// Menu button.
			TouchButtonPtr menuButton = gameLibrary.button(GameLibrary::Menu);
			Rectangle rect = menuButton->getBoundingRect();
			menuButton->setPosition(Point(
					DefaultScreenRightPad - rect.width() / 2,
					DefaultScreenTopPad - rect.height() / 2));
			menuButton->getUnTouchedEvent().connect(boost::bind(&RaceScene::menuActionUp, this, _1));
			prepareButton(menuButton);

			// Adding additional button (back to menu) - appear after finish.

			TouchButtonPtr backtomenuButton = gameLibrary.button(GameLibrary::BackToMenu);
			Rectangle rect2 = backtomenuButton->getBoundingRect();
			backtomenuButton->setPosition(Point(0, DefaultScreenBottom +(rect.top-rect.bottom)/2 +(rect2.top-rect2.bottom)/2 + 300));
			backtomenuButton->setPositionInterpretation(Drawable::E_SCREEN);
			backtomenuButton->getUnTouchedEvent().connect(boost::bind(&RaceScene::menuActionUp, this, _1));
			finishButtons.push_back(backtomenuButton);

			// Adding feedback buttons
			TouchButtonPtr rateit = gameLibrary.button(GameLibrary::RateIt);
			rateit->setPosition(Point(0, DefaultScreenTop - 300));
			rateit->getUnTouchedEvent().connect(boost::bind(&RaceScene::feedbackRateActionUp, this, _1));
			rateit->setPositionInterpretation(Drawable::E_SCREEN);
			feedbackButtons.push_back(rateit);

			TouchButtonPtr later = gameLibrary.button(GameLibrary::Later);
			rect = later->getBoundingRect();
			later->setPosition(Point(0, rateit->getBoundingRect().bottom - (rect.top-rect.bottom) - 20));
			later->getUnTouchedEvent().connect(boost::bind(&RaceScene::feedbackLaterActionUp, this, _1));
			later->setPositionInterpretation(Drawable::E_SCREEN);
			feedbackButtons.push_back(later);

			TouchButtonPtr never = gameLibrary.button(GameLibrary::Never);
			rect = never->getBoundingRect();
			never->setPosition(Point(0, later->getBoundingRect().bottom -(rect.top-rect.bottom) - 20));
			never->getUnTouchedEvent().connect(boost::bind(&RaceScene::feedbackNeverActionUp, this, _1));
			never->setPositionInterpretation(Drawable::E_SCREEN);
			feedbackButtons.push_back(never);
		}
		else
		{
			for (unsigned int i = 0; i < touchButtons.size(); ++i)
			{
				addChild(touchButtons.at(i), hudZOrder);
			}
		}

		// Race position label.
		//Rectangle rect = touchButtons.at(1)->getBoundingRect();
		LabelPtr racePosLabel = new Label(gameLibrary.theMilkmanConspiracyFont());
		racePosLabel->setText(" ");
		racePosLabel->addAction(racePosAction);
		racePosLabel->setPositionInterpretation(Drawable::E_SCREEN);
		racePosLabel->setPosition(Point( DefaultScreenRightPad - 60, 
										DefaultScreenBottom + 660));
		addChild(racePosLabel, hudZOrder);

	}

	void RaceScene::addMeters()
	{
		playerTruckDefense = new ProgressBar(Truck::defaultTruckDefense, 200, 30);
		playerTruckDefense->connectVariable(playerTruck->truckParams.truckDefense);
		playerTruckDefense->setColor(1.0, 0.7, 0.0, 0.6);
		playerTruckDefense->setBorderWidth(2.0);
		playerTruckDefense->setBorderColor(0.3, 0.3, 0.3, 0.3);
		playerTruckDefense->setPosition(Point(DefaultScreenLeft + 5, DefaultScreenTop - 40));
		playerTruckDefense->setPositionInterpretation(Drawable::E_SCREEN);
		addChild(playerTruckDefense, hudZOrder);

		// Adding armor/ health label
		LabelPtr label = new Label(gameLibrary.theMilkmanConspiracyFont());
		label->setText("armor/health");
		label->setPositionInterpretation(Drawable::E_SCREEN);
		label->setPosition(Point(DefaultScreenLeft + 102, DefaultScreenTop - 22));
		addChild(label, hudZOrder);

		playerTruckArmor = new ProgressBar(Truck::defaultTruckArmor, 200, 30);
		playerTruckArmor->connectVariable(playerTruck->truckParams.truckArmor);
		playerTruckArmor->setColor(1.0, 0.0, 0.0, 0.5);
		playerTruckArmor->setBorderWidth(1.0);
		playerTruckArmor->setBorderColor(0.3, 0.3, 0.3, 0.5);
		playerTruckArmor->setPosition(Point(DefaultScreenLeft + 5, DefaultScreenTop - 40));
		playerTruckArmor->setPositionInterpretation(Drawable::E_SCREEN);
		addChild(playerTruckArmor, hudZOrder);

		// Adding rage label
		LabelPtr label2 = new Label(gameLibrary.theMilkmanConspiracyFont());
		label2->setText("rage");
		label2->setPositionInterpretation(Drawable::E_SCREEN);
		label2->setPosition(Point(DefaultScreenLeft + 102, DefaultScreenTop - 62));
		addChild(label2, hudZOrder);

		playerTruckRage = new ProgressBar(Truck::defaultTruckRage, 200, 20);
		playerTruckRage->connectVariable(playerTruck->truckParams.truckRage);
		playerTruckRage->setColor(0.0, 0.7, 1.0, 0.5);
		playerTruckRage->setBorderWidth(2.0);
		playerTruckRage->setBorderColor(0.3, 0.3, 0.3, 0.5);
		playerTruckRage->setPosition(Point(DefaultScreenLeft + 5, DefaultScreenTop - 75));
		playerTruckRage->setPositionInterpretation(Drawable::E_SCREEN);
		addChild(playerTruckRage, hudZOrder);
	}

	void RaceScene::addTrucksControllers()
	{
		//playerTruckController = new TruckController(playerTruck, gameLibrary);
        playerTruckController = new TruckController(playerTruck, gameLibrary, rr::TruckController::defaultMaxSpeed+20, 
                                                    true, rr::TruckController::defaultRageMaxSpeed+10);
		trucksControllers.push_back(playerTruckController);

		foreach(TruckPtr t, opponentTrucks)
		{
			TruckControllerPtr tc = new TruckController(t, gameLibrary);
            //TruckControllerPtr tc = new TruckController(t, gameLibrary, rr::TruckController::defaultMaxSpeed, false);
			trucksControllers.push_back(tc);
			addTruckTouchHandler(tc);
			DrivingAIPtr ai = new OpponentAI(tc, roadBound);
			opponentsAi.push_back(ai);
		}

		foreach(TruckPtr p, policeTrucks)
		{
			TruckControllerPtr tc = new TruckController(p, gameLibrary, 650, false);
			trucksControllers.push_back(tc);
			addTruckTouchHandler(tc);
			DrivingAIPtr ai = new PoliceAI(tc, roadBound);
			opponentsAi.push_back(ai);
		}

		foreach(CivilCarPtr c, civilCars)
		{
			TruckControllerPtr tc= new TruckController(c, gameLibrary, TruckController::defaultMaxSpeed / 2, false );
			trucksControllers.push_back(tc);
			DrivingAIPtr ai = new DrivingAI(tc, roadBound);
			ai->setPositionOnRoad(DrivingAI::P_Right);
			opponentsAi.push_back(ai);
		}
	}

	RaceScene::~RaceScene()
	{

	}

	void RaceScene::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{

		Scene::update(thisFrameStartTime, deltaTime);
		elapsedTime += deltaTime;

		switch(state)
		{
			case RS_WAIT_FOR_START:
			{
				if (isRestarted)
				{
					elapsedTime -= deltaTime;
					isRestarted = false;
				}
				else
				{
					startScreen(deltaTime);
				}
				// Start race after 3 seconds.
				if (elapsedTime.realSeconds() >= 3)
				{
					// Set initial accleration for all trucks.
					foreach(TruckControllerPtr t, trucksControllers)
					{
						t->startRace();
					}
					foreach(LabelPtr l, infoLabels)
					{
						removeChild(l);
					}
					infoLabels.clear();
					removeChild(infoLabelsBackground);

					state = RS_RACE_ACTIVE;
				}
				break;
			}
			case RS_RACE_ACTIVE:
			{
				if ((playerTruck->position().y() > finishLine->position().y() ) || playerTruckController->isFinished())
				{
					LOGI("Race finished!");

					if (!playerTruckController->isDestroyed()) ++raceFinishCount;
					if (raceFinishCount == 2 && showFeedbackPromt ) // Show feedback after 2 race finish.
					{
						addInfoLabels(2, 200);
						feedbackScreen(TimeDuration(0.0f));
					}
					else
					{
						showFeedbackPromt = false;
						addInfoLabels(3, 200);
						finishScreen(TimeDuration(0.0f));
					}

					playerTruckController->setFinished();
					playerTruckController->update(deltaTime);

					foreach(TruckControllerPtr t, trucksControllers)
					{
						t->setFinished();
						t->update(deltaTime);
					}


					state = RS_RACE_FINISHED;
				}
				else
				{
					startNearbyAnimalsMoving();
					// Update AI
					foreach(DrivingAIPtr ai, opponentsAi)
					{
						ai->update(deltaTime, trucksControllers);
					}
					foreach(TruckControllerPtr t, trucksControllers)
					{
						t->update(deltaTime);
					}
					// Update player truck race position.
					racePosAction->resetRacePosition();
					foreach(SpritePtr t, opponentTrucks)
					{
						racePosAction->updateRacePosition(playerTruck->getBoundingRect().top, t->getBoundingRect().top);
					}
				}

				break;
			}
			case RS_RACE_FINISHED:
			{
				if (!showFeedbackPromt) finishScreen(deltaTime);
				else feedbackScreen(deltaTime);
				break;
			}
		}

		//LOGD("RaceScene::update(deltaTime: %f) elapsedTime = %f, fps = %f", deltaTime.realSeconds(), elapsedTime.realSeconds(), 1.0/deltaTime.realSeconds());
		//LOGI("Active rect x: %f y: %f", cameraTruck->getBoundingRect().left, cameraTruck->getBoundingRect().top);

		game().director().setCameraPosition(translateScreenPointToRacePoint(Point(0, 0)));
		removeOffScreenStuff();
	}
	
	Point RaceScene::translateScreenPointToRacePoint(const Point& screenPoint) const
	{
		return Point(screenPoint.x(), screenPoint.y() + playerTruck->position().y() + TruckCenterVerticalOffset);
	}


	size_t RaceScene::raceLength() const
	{
		return backgrounds.size() * DefaultScaleHeight;
	}

	void RaceScene::startNearbyAnimalsMoving()
	{
		const Point& truckPos(playerTruck->position());
		foreach (AnimalPtr& animal, animals)
		{
			if (animal->position().y() <= truckPos.y() + DefaultScaleHeight && !animal->moving())
			{
				animal->startMoving();
				//LOGD("started an animal moving");
			}
		}
	}

	void RaceScene::removeOffScreenStuff()
	{
		const Point& truckPos(playerTruck->position());
		for (vector<AnimalPtr>::iterator animalIt = animals.begin(); animalIt != animals.end(); ++animalIt)
		{
			AnimalPtr& animal(*animalIt);
			if (animal->position().y() <= truckPos.y() - DefaultScaleHeight)
			{
				animal->stopMoving();
				//LOGD("stopped an animal moving");
				removeChild(animal);
				animalIt = animals.erase(animalIt);
				--animalIt;
			}
		}

		for (vector<ObstaclePtr>::iterator obstacleIt = obstacles.begin(); obstacleIt != obstacles.end(); ++obstacleIt)
		{
			ObstaclePtr& obstacle(*obstacleIt);
			if (obstacle->position().y() <= truckPos.y() - DefaultScaleHeight)
			{
				removeChild(obstacle);
				obstacleIt = obstacles.erase(obstacleIt);
				--obstacleIt;
			}
		}
	}

	void RaceScene::brakeActionDown(const TouchHandler&)
	{
		if (state != RS_RACE_FINISHED) playerTruckController->brakeActionDown();
	}

	void RaceScene::brakeActionUp(const TouchHandler&)
	{
		if (state != RS_RACE_FINISHED) playerTruckController->brakeActionUp();
	}

	void RaceScene::shotActionDown(const TouchHandler& handle)
	{
		// do some casting so that we can actually use the enemy that we are trying to shoot.
		TruckTouchHandler* enemyHandle;
		if ((enemyHandle = dynamic_cast<TruckTouchHandler*> (const_cast<TouchHandler*>(&handle))) == NULL) {
			LOGD("MICHAEL >>>>> epic casting fail for shot action down by touching an enemy truck");
			return;
		}
		//LOGD("MICHAEL >>>>> Touched Sprite %s",enemyHandle->getController()->getTruck()->name().c_str());
		if (state != RS_RACE_FINISHED && !playerTruckController->isDestroyed())
		{
			if (playerTruckController->getShotGun().isReadyToShoot())
			{
				// shoot the gun
				playerTruckController->getShotGun().shoot();
				playerTruck->activateAttachedAnimation(Truck::ShotgunBlast);
                gameLibrary.shotgunSound()->play();
				// calculate damage
				Point player = playerTruckController->getTruck()->position();
				Point enemy = enemyHandle->getController()->getTruck()->position();
				int damage = playerTruckController->getShotGun().calculateDamage(player, enemy);
				// handle the hit
				if (damage > 0) {
					gameLibrary.gunPingSound()->play();
					enemyHandle->getController()->getTruck()->decrementHealth(damage);
					enemyHandle->getController()->getTruck()->setAttachedAnimationPosition(Truck::Sparks, 
							Point(0, -(enemyHandle->getController()->getTruck()->size().height()) / 2));
					enemyHandle->getController()->getTruck()->activateAttachedAnimation(Truck::Sparks);
				} else {
					// TODO: when there is a gunshot missed sound, play it here
				}
			}
		}
	}

	void RaceScene::shotActionUp(const TouchHandler&)
	{
		if (state != RS_RACE_FINISHED)
		{
			// nothing happens on shot action up
		}
	}

	void RaceScene::menuActionUp(const TouchHandler&)
	{
		game().activeScene = game()._MainMenuScene;
		gameLibrary.setSoundGain(false);
		if (state != RS_RACE_FINISHED)
		{
			boost::intrusive_ptr<MainMenuScene> mainMenu = dynamic_pointer_cast<MainMenuScene>(game().mainMenuScene);
			if (mainMenu) mainMenu->setRacePaused();
		}
	}

	void RaceScene::nitroActionDown(const TouchHandler&)
	{
		if (state != RS_RACE_FINISHED) playerTruckController->nitroActionDown();
	}

	void RaceScene::addInfoLabels(unsigned int labelsCount, float firstLabelYpos)
	{
		infoLabelsBackground = NULL;
		infoLabelsBackground = new ProgressBar(10, DefaultScreenRight * 2 + 4, DefaultScreenTop * 2 + 4);
		infoLabelsBackground->setCurrentValue(10);
		infoLabelsBackground->setPosition(Point(DefaultScreenLeft -2, DefaultScreenBottom - 2));
		infoLabelsBackground->setPositionInterpretation(Drawable::E_SCREEN);
		infoLabelsBackground->setColor(0.1, 0.1, 0.1, 0.2);
		addChild(infoLabelsBackground, hudZOrder);
		float yPos = firstLabelYpos;
		for (unsigned int i = 0; i < labelsCount; ++i)
		{
			LabelPtr label = new Label(gameLibrary.theMilkmanConspiracyFont());
			label->setPosition(Point(0, yPos));
			yPos -= 20;
			label->setPositionInterpretation(Drawable::E_SCREEN);
			addChild(label, hudZOrder);
			infoLabels.push_back(label);
		}
		infoLabelShowingTime = TimeDuration();
	}

	void RaceScene::startScreen(const TimeDuration& deltaTime)
	{
		infoLabelShowingTime += deltaTime;
		float alpha = 1.0 - infoLabelShowingTime.realSeconds() / 2.0;
		if (alpha < 0) alpha = 0;
		infoLabelsBackground->setColor(0.1, 0.1, 0.1, alpha);
		if (infoLabelShowingTime.realSeconds() < 3) infoLabels.at(0)->setText("READY!");
	}

	void RaceScene::finishScreen(const TimeDuration& deltaTime)
	{

		infoLabelShowingTime += deltaTime;
		float alpha = infoLabelShowingTime.realSeconds() / 2.0;
		if (alpha < 0.7)
		{
			infoLabelsBackground->setColor(0.1, 0.1, 0.1, alpha);
		}
		else if (infoLabels.at(0)->text() == "")
		{
			infoLabelsBackground->setColor(0.1, 0.1, 0.1, 0.7);
			if (playerTruckController->isDestroyed())
			{
				infoLabels.at(0)->setText("YOU LOSE...");
				infoLabels.at(1)->setText("Your truck was destroyed");
			}
			else if (racePosAction->getRacePosition() == 1)
			{
				infoLabels.at(0)->setText("YOU WIN!");
				infoLabels.at(1)->setText("You came in first place!");
			}
			else
			{
				infoLabels.at(0)->setText("Race over");
				infoLabels.at(1)->setText("You came in " + getPositionDescription(racePosAction->getRacePosition()) + " place");
			}

			String sec(elapsedTime.realSeconds());
			if (m_bestTimes->reportTime(currentRaceTrack, elapsedTime.completeSeconds()))
				infoLabels.at(2)->setText("Your race time of " + sec.substring(0, sec.indexOf('.')) + " seconds is the new best time!");
			else
				infoLabels.at(2)->setText("Your race time was " + sec.substring(0, sec.indexOf('.')) + " seconds.");


			// Show additional buttons:
			foreach(TouchButtonPtr button, finishButtons)
			{
				addChild(button, hudZOrder);
				touchButtons.push_back(button);
			}
		}
	}

	void RaceScene::feedbackScreen(const TimeDuration& deltaTime)
	{
		infoLabelShowingTime += deltaTime;
		float alpha = infoLabelShowingTime.realSeconds() / 2.0;
		if (alpha < 0.7)
		{
			infoLabelsBackground->setColor(0.1, 0.1, 0.1, alpha);
		}
		else if (infoLabels.at(0)->text() == "")
		{
			infoLabelsBackground->setColor(0.1, 0.1, 0.1, 0.7);
			infoLabels.at(0)->setText("IF YOU LIKE MY GAME");
			infoLabels.at(1)->setText("PLEASE RATE IT.");
			// Show additional buttons:
			foreach(TouchButtonPtr button, feedbackButtons)
			{
				addChild(button, hudZOrder);
				touchButtons.push_back(button);
			}
		}
	}

	void RaceScene::hideFeedbackScreen()
	{
		foreach(LabelPtr l, infoLabels)
		{
			removeChild(l);
		}
		foreach(TouchButtonPtr button, feedbackButtons)
		{
			removeChild(button);
		}
		infoLabels.clear();
		removeChild(infoLabelsBackground);
		showFeedbackPromt = false;

		// Delete feedback buttons.
		foreach(TouchButtonPtr button, feedbackButtons)
		{
			touchButtons.erase(remove_if(touchButtons.begin(), touchButtons.end(), bind2nd(equal_to<TouchButtonPtr>(), button)), touchButtons.end());
		}
	}


	void RaceScene::feedbackRateActionUp(const TouchHandler&)
	{
		saveFeedback(); // Never show feedback prompt again.

		// Redirect to market app.
		game().activateMarketFeedback();
	}


	bool RaceScene::checkUserFeedback() const
	{
		bool ret = false;
		std::ifstream settingsFile(game().getSettingsFile().c_str());
		if (!settingsFile.fail())
		{
			if (!settingsFile.eof())
			{
				int feedback;
				settingsFile>>feedback;
				if (feedback == 1) ret = true;
			}
			settingsFile.close();
		}
		return ret;
	}

	void RaceScene::saveFeedback() const
	{
		LOGI("Opening settings file for write.");
		String setFile(game().getSettingsFile());
		LOGI("settings file: %s", setFile.c_str());
		std::ofstream settingsFile(setFile.c_str());
		settingsFile<<1;
		settingsFile.close();
	}

	void RaceScene::feedbackLaterActionUp(const TouchHandler&)
	{
		hideFeedbackScreen();
		raceFinishCount = 0;
		addInfoLabels(3, 200);
		finishScreen(TimeDuration(0.0f));
	}

	void RaceScene::feedbackNeverActionUp(const TouchHandler&)
	{
		saveFeedback();
		hideFeedbackScreen();
		raceFinishCount = 0;
		addInfoLabels(3, 200);
		finishScreen(TimeDuration(0.0f));
	}

	void RaceScene::handleTouchEvent(const TouchEvent& touchEvent)
	{
		// As a reference, these are the different return values for touchEvent.action()
		// 0-down 1-up 2-move (3-cancel) 4-outside 5-down sec 6-up sec (7-hover)
		
		// Put the truck's into the touch list
		vector<TouchHandlerPtr> touchesCopy;
		foreach(TouchHandlerPtr thp, truckTouchHandlers)
		{
			touchesCopy.push_back(thp);
		}
		// Put the other buttons into the touch list
		foreach(TouchHandlerPtr thp, touchButtons)
		{
			touchesCopy.push_back(thp);
		}
		// Iterate through touch list and handle touches
		foreach(TouchHandlerPtr touch, touchesCopy)
		{
			// TODO in the future, allow for multi-touch, by removing the code that
			//  makes the touching of one button release the other buttons.
			if (touch->checkHit(touchEvent.x, touchEvent.y) &&
				(touchEvent.action == 0 || touchEvent.action == 2))
			{
				touch->setState(TouchHandler::Touched);
			}
			else if (touch->getState() == TouchHandler::Touched)
			{
				touch->setState(TouchHandler::Untouched);
			}
		}
	}
}
