// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "TrackSelectScene.hpp"
#include "RaceTracks.hpp"
#include "RaceScene.hpp"
#include "Globals.hpp"
#include "BestTimes.hpp"

#include "boost/foreach.hpp"
#define foreach BOOST_FOREACH

#include "engine/Rectangle.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/Log.hpp"
#include "engine/Label.hpp"

namespace rr
{
	
	TrackSelectScene::TrackSelectScene(GameLibrary& gameLibrary, const BestTimesPtr& bestTimes)
	: gameLibrary(gameLibrary)
	, m_bestTimes(bestTimes)
	{
		drawBackground();
		loadButtons();
	}
	
	TrackSelectScene::~TrackSelectScene() {}
	
	void TrackSelectScene::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		Scene::update(thisFrameStartTime, deltaTime);
	}
	
	void TrackSelectScene::handleTouchEvent(const TouchEvent& touchEvent)
	{
		foreach(TouchButtonPtr button, touchButtons)
		{
			if (button->checkHit(touchEvent.x, touchEvent.y) &&
				(touchEvent.action == 0 || touchEvent.action == 2))
			{
				button->setState(TouchHandler::Touched);
			}
			else if (button->getState() == TouchHandler::Touched)
			{
				button->setState(TouchHandler::Untouched);
			}
		}
	}
	
	void TrackSelectScene::handleActivated()
	{
		removeAllChildren();
		drawBackground();
		loadButtons();
	}
	
	
	void TrackSelectScene::drawBackground()
	{
		//background
		SpritePtr background = gameLibrary.loadingSprite("MainBackground");
		addChild(background, backgroundZOrder);
		background->setPositionInterpretation(Drawable::E_SCREEN);
		//faded background mask
		dimmedBackground = new DrawableRectangle(480.0, 800.0);
		dimmedBackground->setColor(0.1, 0.1, 0.1, 0.7);
		dimmedBackground->setPosition(Point(DefaultScreenLeft, DefaultScreenBottom));
		dimmedBackground->setPositionInterpretation(Drawable::E_SCREEN);
		addChild(dimmedBackground, backgroundZOrder);
	}
	
	void TrackSelectScene::loadButtons()
	{
		gameLibrary.loadTrackSelectButtons();
		
		// Title
		title = gameLibrary.loadingSprite("TrackSelectMenuTitle");
		title->setPosition(Point(0, DefaultScreenTop - (title->size().height() / 2) - 15));
		addChild(title, backgroundZOrder);
		title->setPositionInterpretation(Drawable::E_SCREEN);
		
		// Yankee Shot
		yankeeShotButton = gameLibrary.trackSelectButton(GameLibrary::E_YANKEE_SHOT);	
		Rectangle rect = yankeeShotButton->getBoundingRect();
		float leftPos = -(rect.right - rect.left) / 2;
		yankeeShotButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, DefaultScreenTop/2));
		yankeeShotButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::yankeeShotButtonActionUp, this, _1));
		prepareButton(yankeeShotButton);
		addBestTimeLabel(RaceTracks::YANKEESHOT, yankeeShotButton->getBoundingRect().bottom);
		
		// Everwhichaways
		everwhichawaysButton = gameLibrary.trackSelectButton(GameLibrary::E_EVERWHICHAWAYS);
		rect = everwhichawaysButton->getBoundingRect();
		everwhichawaysButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, yankeeShotButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 55));
		everwhichawaysButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::everwhichawaysButtonActionUp, this, _1));
		prepareButton(everwhichawaysButton);
		addBestTimeLabel(RaceTracks::EVERWHICHAWAYS, everwhichawaysButton->getBoundingRect().bottom);
		
		// Uhmurkin
		uhmurkinButton = gameLibrary.trackSelectButton(GameLibrary::E_UHMURKIN);
		rect = uhmurkinButton->getBoundingRect();
		uhmurkinButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, everwhichawaysButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 55));
		uhmurkinButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::uhmurkinButtonActionUp, this, _1));
		prepareButton(uhmurkinButton);
		addBestTimeLabel(RaceTracks::UHMURKIN, uhmurkinButton->getBoundingRect().bottom);
		
		// Dadgum Trakturs
		dadgumTraktursButton = gameLibrary.trackSelectButton(GameLibrary::E_DADGUM_TRAKTURS);
		rect = dadgumTraktursButton->getBoundingRect();
		dadgumTraktursButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, uhmurkinButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 55));
		dadgumTraktursButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::dadgumTraktursButtonActionUp, this, _1));
		prepareButton(dadgumTraktursButton);
		addBestTimeLabel(RaceTracks::DADGUMTRAKTURS, dadgumTraktursButton->getBoundingRect().bottom);
		
		// Real Bammer
		realBammerButton = gameLibrary.trackSelectButton(GameLibrary::E_REAL_BAMMER);
		rect = realBammerButton->getBoundingRect();
		realBammerButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, dadgumTraktursButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 55));
		realBammerButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::realBammerButtonActionUp, this, _1));
		prepareButton(realBammerButton);
		addBestTimeLabel(RaceTracks::REALBAMMER, realBammerButton->getBoundingRect().bottom);
		
		// Wijadidja
		wijadidjaButton = gameLibrary.trackSelectButton(GameLibrary::E_WIJADIDJA);
		rect = wijadidjaButton->getBoundingRect();
		wijadidjaButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, realBammerButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 55));
		wijadidjaButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::wijadidjaButtonActionUp, this, _1));
		prepareButton(wijadidjaButton);
		addBestTimeLabel(RaceTracks::WIJADIDJA, wijadidjaButton->getBoundingRect().bottom);
		
		// Back Button
		backButton = gameLibrary.button(GameLibrary::Menu);
		rect = backButton->getBoundingRect();
		backButton->setPosition(Point(DefaultScreenRightPad - rect.width() / 2, DefaultScreenTopPad - rect.height() / 2));
		backButton->getUnTouchedEvent().connect(boost::bind(&TrackSelectScene::backButtonActionUp, this, _1));
		prepareButton(backButton);
	}
	
	void TrackSelectScene::prepareButton(TouchButtonPtr button)
	{
		button->setPositionInterpretation(Drawable::E_SCREEN);
		touchButtons.push_back(button);
		addChild(button, hudZOrder);
	}
	
	void TrackSelectScene::addBestTimeLabel(RaceTracks::Races trackType, float yUpper)
	{
		if (m_bestTimes->getTime(trackType) != INT_MAX)
		{
			LabelPtr bestTime(new Label(gameLibrary.theMilkmanConspiracyFont()));
			bestTime->setText(Format("Best Time: %1 seconds", m_bestTimes->getTime(trackType)));
			Size textSize = bestTime->size();
			bestTime->setPosition(Point(-150 + textSize.width() / 2, yUpper - textSize.height() / 2 - 10));
			addChild(bestTime, hudZOrder);
		}
	}
	
	// Action Buttons
	void TrackSelectScene::backButtonActionUp(const TouchHandler&)
	{
		game().activeScene = game()._MainMenuScene;
	}
	
	void TrackSelectScene::yankeeShotButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::YANKEESHOT);
	}
	
	void TrackSelectScene::everwhichawaysButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::EVERWHICHAWAYS);
	}
	
	void TrackSelectScene::uhmurkinButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::UHMURKIN);
	}
	
	void TrackSelectScene::dadgumTraktursButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::DADGUMTRAKTURS);
	}
	
	void TrackSelectScene::realBammerButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::REALBAMMER);
	}
	
	void TrackSelectScene::wijadidjaButtonActionUp(const TouchHandler&)
	{
		game().startRace(RaceTracks::WIJADIDJA);
	}
}
