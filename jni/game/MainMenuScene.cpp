// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "engine/ProgressBar.hpp"
#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "MainMenuScene.hpp"
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
#include <sys/stat.h>
#include <fstream>

#include "boost/range/algorithm_ext/push_back.hpp"
#include "boost/range/irange.hpp"
#include "boost/range/algorithm/random_shuffle.hpp"
#include <map>

#include "engine/TexturedFont.hpp"
#include "engine/Rectangle.hpp"
#include "boost/foreach.hpp"
#define foreach BOOST_FOREACH

#include "boost/random.hpp"

#include "engine/Log.hpp"
#include "CivilCar.hpp"
#include "engine/DrawableRectangle.hpp"
#include "engine/DrawableLine.hpp"
#include "engine/Size.hpp"
#include "miniblocxx/String.hpp"
#include "boost/bind.hpp"


namespace rr
{
	using namespace boost;
	
	namespace 
	{
		class PointsBuilder
		{
		public:
			PointsBuilder(float xCoord, float yCoord)
			{
				nextPoint(xCoord, yCoord);
			}		
			PointsBuilder & nextPoint(float xCoord, float yCoord)
			{
				points.push_back(Point(DefaultScreenLeft + xCoord, DefaultScreenTop - yCoord));
				return *this;
			}
			operator std::vector<Point>() const{ return points; }
		private:
			std::vector<Point> points;
		};
	}

	MainMenuScene::MainMenuScene(GameLibrary& gameLibrary)
	: gameLibrary(gameLibrary)
	, racePaused(false)
    , state(E_MAIN_MENU)
	{
		gameLibrary.loadMenuAtlases();
		drawBackground();
		loadButtons();
		loadControls();
		addMainMenuButtons();
	}
	
	MainMenuScene::~MainMenuScene()
	{

	}
	
	void MainMenuScene::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		Scene::update(thisFrameStartTime, deltaTime);
		elapsedTime+=deltaTime;
		
		switch(state)
		{
			case E_TIPS:
			break;
			case E_OPTIONS:
			break;
			case E_MAIN_MENU:
			break;
		}
	}
	
	void MainMenuScene::handleTouchEvent(const TouchEvent& touchEvent)
	{
		unsigned int startSize = 0;
		foreach(TouchButtonPtr button, touchButtons)
		{
			startSize = touchButtons.size();
			if (button->checkHit(touchEvent.x, touchEvent.y) &&
					(touchEvent.action == 0 || touchEvent.action == 2))
			{
				button->setState(TouchHandler::Touched);
			}
			else if (button->getState() == TouchHandler::Touched){
				button->setState(TouchHandler::Untouched);
			}
			if (startSize != touchButtons.size())
			{
				break;
			}
		}
	}
	
	void MainMenuScene::startRace()
	{
		game().activeScene = game()._TrackSelectScene;
	}
	
	void MainMenuScene::drawBackground()
	{
		SpritePtr background = gameLibrary.loadingSprite("MainBackground");
		addChild(background, backgroundZOrder);
		background->setPositionInterpretation(Drawable::E_SCREEN);
	}

	
	/* MAIN MENU, OPTIONS & TIPS
	 * LOADING FUNCTIONS
	 */
	 
	void MainMenuScene::loadButtons()
	{
		loadMainMenuButtons();
		loadOptionsButtons();
		loadTipsButtons();
	}
	
	void MainMenuScene::prepareButton(TouchButtonPtr button)
	{
		button->setPositionInterpretation(Drawable::E_SCREEN);
		touchButtons.push_back(button);
		addChild(button, hudZOrder);
	}
	
	void MainMenuScene::setRacePaused()
	{
		//racePaused = true;
		//prepareButton(resumeButton);
	}

	bool MainMenuScene::isRacePaused() const
	{
		return racePaused;
	}

	void MainMenuScene::loadMainMenuButtons()
	{
		gameLibrary.loadMenuButtons();

		title = gameLibrary.loadingSprite("MainMenuTitle");
		title->setPosition(Point(0, DefaultScreenTop - (title->size().height() / 2) - 15));
		addChild(title, backgroundZOrder);
		title->setPositionInterpretation(Drawable::E_SCREEN);
		
		// Race button
		raceButton = gameLibrary.menuButton(GameLibrary::Race);
		Rectangle rect = raceButton->getBoundingRect();
		raceButton->setPosition(Point(-24, DefaultScreenTop/2));
		raceButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::raceActionUp, this, _1));
		prepareButton(raceButton);
		float leftPos = -(rect.right - rect.left) / 2;

		//Options button
		optionsButton = gameLibrary.menuButton(GameLibrary::Options);
		rect = optionsButton->getBoundingRect();
		optionsButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 20, raceButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 18));
		optionsButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::optionsActionUp, this, _1));
		prepareButton(optionsButton);
		
		//Tips button
		tipsButton = gameLibrary.menuButton(GameLibrary::Tips);
		rect = tipsButton->getBoundingRect();
		tipsButton->setPosition(Point(leftPos + (rect.right - rect.left) / 2 - 40, optionsButton->getBoundingRect().bottom-(rect.top-rect.bottom)/2 - 18));
		tipsButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::tipsActionUp, this, _1));
		prepareButton(tipsButton);
		
		//Return to Main Menu from tips or options screen
		backButton = gameLibrary.button(GameLibrary::Menu);
		rect = backButton->getBoundingRect();
		backButton->setPosition(Point(
				DefaultScreenRightPad - rect.width() / 2,
				DefaultScreenTopPad - rect.height() / 2));
	}
	
	void MainMenuScene::loadTipsButtons()
	{
		//faded background mask
		dimmedBackground = new DrawableRectangle(480.0, 800.0);
		dimmedBackground->setColor(0.1, 0.1, 0.1, 0.7);
		dimmedBackground->setPosition(Point(DefaultScreenLeft, DefaultScreenBottom));
		dimmedBackground->setPositionInterpretation(Drawable::E_SCREEN);
		
		dimmedTipFrame = new DrawableRectangle(410.0, 657.0);
		dimmedTipFrame->setColor(0.1, 0.1, 0.1, 0.7);
		dimmedTipFrame->setPosition(Point(DefaultScreenLeft+30, DefaultScreenBottom+25));
		dimmedTipFrame->setPositionInterpretation(Drawable::E_SCREEN);
		
		tipTitle = gameLibrary.loadingSprite("Tips01");
		Rectangle titleBounds = tipTitle->getBoundingRect();
		tipTitle->setPosition(Point(DefaultScreenLeft + 215, DefaultScreenBottom + 736));
		tipTitle->setPositionInterpretation(Drawable::E_SCREEN);
		
		//tip labels reference from bottom left of screen

		loadTipLabel("FORGET THE PAINT JOB.", Point(100, 662));
		loadTipLabel("RUN INTO ANYTHING IN YER PATH", Point(50, 630));
		loadTipLabel("TO BOOST YER REDNECK RAGE!", Point(50, 610));
		
		loadTipBox(Size(210.0, 40.0),  Point(135, 540.0));   //Rage Tip box
		
		tipRageDemoBar = new ProgressBar(200, 200, 30);
		tipRageDemoBar->setColor(0.0, 0.7, 1.0, 0.5);
		tipRageDemoBar->setBorderWidth(2.0);
		tipRageDemoBar->setBorderColor(0.3, 0.3, 0.3, 0.5);
		tipRageDemoBar->setPosition(Point(DefaultScreenLeft + 140, DefaultScreenBottom + 545));
		tipRageDemoBar->setCurrentValue(155);
		tipRageDemoBar->setPositionInterpretation(Drawable::E_SCREEN);
		
		loadTipLabel("rage", Point(218, 565));
		
		loadTipLabel("WHEN YOUR RAGE BAR FILLS UP,", Point(50, 520));
		loadTipLabel("YOU'LL LEAVE 'EM IN THE DUST!", Point(50, 500));
		
		loadTipLabel("BUT WATCH OUT, CUZ", Point(180, 460));
		loadTipLabel("YARN'T INVINCIBLE.", Point(180, 440));
		loadTipLabel("YER TRUCK CAN ONLY", Point(180, 400));
		loadTipLabel("TAKE SO MUCH.", Point(180, 380));

		loadTipBox(Size(100.0, 120.0), Point(60.0, 347.0)); //EngineBlow Tip box
				
		engineBlowTip = gameLibrary.loadingSprite("FireAndSmoke0001");
		engineBlowTip->setPosition(Point(DefaultScreenLeft + 90, DefaultScreenBottom + 465));
		engineBlowTip->setPositionInterpretation(Drawable::E_SCREEN);
		
		redTruckTip = gameLibrary.loadingSprite("RedTruckDrivesLeft0001");
		redTruckTip->setPosition(Point(DefaultScreenLeft + 110, DefaultScreenBottom + 407));
		redTruckTip->setPositionInterpretation(Drawable::E_SCREEN);
		
		loadTipLabel("TOUCH ENEMY TRUCKS", Point(50, 320));
		loadTipLabel("AND COPS TO BLAST", Point(50, 300));
		loadTipLabel("'EM WITH YER SHOTGUN.", Point(50, 280));
		
		loadTipLabel("THE CLOSER YA ARE,", Point(50, 240));
		loadTipLabel("THE MORE DAMAGE", Point(50, 220));
		loadTipLabel("YOU'LL INFLICT.", Point(50, 200));
		
		
		loadTipLabel("NOW GO GET 'EM", Point(140, 150));
		loadTipLabel("LIKE THE REDNECK YA ARE!", Point(90, 130));
		loadTipLabel("YEEHAW!", Point(180, 110));
		
		LOGD("Loading Tip Boxes");
		loadTipBox(Size(80.0, 120.0), Point(330.0, 205));  //Police Tip box
		
		//tip vehicle graphic icons in boxes
		policeTruckTip = gameLibrary.loadingSprite("PoliceTruckDrivesNorth0001");
		policeTruckTip->setPosition(Point(DefaultScreenLeft + 370, DefaultScreenBottom + 265));
		policeTruckTip->setPositionInterpretation(Drawable::E_SCREEN);
		
		//shotgun button for tip screen
		shotButton = gameLibrary.button(GameLibrary::Shot);
		Rectangle shotgunBounds = shotButton->getBoundingRect();
		shotButton->setPosition(Point( 0,
				DefaultScreenBottomPad + shotgunBounds.height() / 2 + 20));
		shotButton->getTouchedEvent().connect(boost::bind(&MainMenuScene::shotActionDown, this, _1));
		
		shotgunBackground = gameLibrary.loadingSprite("Shotgun01");
		shotgunBackground->setPosition(Point( 0,
				DefaultScreenBottomPad + shotgunBounds.height() / 2 + 20));
		shotgunBackground->setPositionInterpretation(Drawable::E_SCREEN);
	}
	
	void MainMenuScene::loadOptionsButtons()
	{
		gameLibrary.loadOptionsButtons();

		// These options labels are reused throughout the options menu with different text
		optionsLabel1 = new Label(gameLibrary.theMilkmanConspiracyFont());
		optionsLabel1->setPosition(Point(ColumnOne, RowTwo));
		optionsLabel1->setScale(scaleFactor);
		
		optionsLabel2 = new Label(gameLibrary.theMilkmanConspiracyFont());
		optionsLabel2->setPosition(Point(ColumnOne, RowThree));
		optionsLabel2->setScale(scaleFactor);
		
		optionsLabel3 = new Label(gameLibrary.theMilkmanConspiracyFont());
		optionsLabel3->setPosition(Point(ColumnOne, RowFour));
		optionsLabel3->setScale(scaleFactor);
		
		left = new Label(gameLibrary.theMilkmanConspiracyFont());
		left->setPosition(Point(ColumnTwo, RowThree));
		left->setScale(scaleFactor);
		
		right = new Label(gameLibrary.theMilkmanConspiracyFont());
		right->setPosition(Point(ColumnTwo, RowFour));
		right->setScale(scaleFactor);
		
		// options menu header graphics
		optionsTitle = gameLibrary.loadingSprite("Outhouse01");
		optionsTitle->setPosition(Point(0, DefaultScreenTopPad - (optionsTitle->size().height()/2 + 70)));
		optionsTitle->setPositionInterpretation(Drawable::E_SCREEN);
		
		optionsStereo = gameLibrary.loadingSprite("Stereo");
		optionsStereo->setPosition(Point(0, DefaultScreenTopPad - (optionsStereo->size().height()/2 + 110)));
		optionsStereo->setPositionInterpretation(Drawable::E_SCREEN);
		
		optionsWheel = gameLibrary.loadingSprite("SteeringWheel");
		optionsWheel->setPosition(Point(0, DefaultScreenTopPad - (optionsWheel->size().height()/2 + 70)));
		optionsWheel->setPositionInterpretation(Drawable::E_SCREEN);
		
		// options menu select boxes
		steeringBox = new DrawableRectangle(210.0, 35.0);
		steeringBox->setColor(1.0, 0.0, 0.0, 0.4);
		steeringBox->setPosition( Point( DefaultScreenLeft + 246, DefaultScreenBottom + 517));
		steeringBox->setPositionInterpretation(Drawable::E_SCREEN);
		
		leftKeyBox = new DrawableRectangle(40.0, 40.0);
		leftKeyBox->setColor(1.0, 0.0, 0.0, 0.4);
		leftKeyBox->setPosition( Point(DefaultScreenLeft + 334, DefaultScreenBottom + 440));
		leftKeyBox->setPositionInterpretation(Drawable::E_SCREEN);
		
		rightKeyBox = new DrawableRectangle(40.0, 40.0);
		rightKeyBox->setColor(1.0, 0.0, 0.0, 0.4);
		rightKeyBox->setPosition( Point(DefaultScreenLeft + 334, DefaultScreenBottom + 374));
		rightKeyBox->setPositionInterpretation(Drawable::E_SCREEN);
		
		// Sound button
		soundButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_AUDIO);
		Rectangle rect = soundButton->getBoundingRect();
		soundButton->setPosition(Point(DefaultScreenLeft + 100 + rect.width()/2, RowTwo - 10));
		soundButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::soundActionUp, this, _1));

		// Controls button
		controlsButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_CONTROLS);
		rect = controlsButton->getBoundingRect();
		controlsButton->setPosition(Point(DefaultScreenLeft + 100 + rect.width()/2, RowThree - 10));
		controlsButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::controlsActionUp, this, _1));

		// On button 1
		onButton1 = gameLibrary.optionsButton(GameLibrary::E_OPTION_ON);
		onButton1->setPosition(Point(ColumnTwo, RowTwo));
		onButton1->getUnTouchedEvent().connect(
											   boost::bind(&MainMenuScene::musicActionUp, this, _1));
		// Off button 1
		offButton1 = gameLibrary.optionsButton(GameLibrary::E_OPTION_OFF);
		offButton1->setPosition(Point(ColumnTwo, RowTwo));
		offButton1->getUnTouchedEvent().connect(
												boost::bind(&MainMenuScene::musicActionUp, this, _1));
		// On button 2
		onButton2 = gameLibrary.optionsButton(GameLibrary::E_OPTION_ON);
		onButton2->setPosition(Point(ColumnTwo, RowThree));
		onButton2->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::sfxActionUp, this, _1));

		// Off button 2
		offButton2 = gameLibrary.optionsButton(GameLibrary::E_OPTION_OFF);
		offButton2->setPosition(Point(ColumnTwo, RowThree));
		offButton2->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::sfxActionUp, this, _1));

		// Tilt button
		tiltButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_TILT);
		tiltButtonConnection = tiltButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::tiltActionUp, this, _1));

		// D-Pad button
		dPadButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_DPAD);
		dPadButtonConnection = dPadButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::dPadActionUp, this, _1));

		// D-Pad Alt button
		dPadAltButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_DPAD_ALT);
		dPadAltButtonConnection = dPadAltButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::dPadAltActionUp, this, _1));

		// Trackball button
		trackballButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_TRACKBALL);
		trackballButtonConnection = trackballButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::trackballActionUp, this, _1));

		// Keyboard button
		keyboardButton = gameLibrary.optionsButton(GameLibrary::E_OPTION_KEYBOARD);
		keyboardButtonConnection = keyboardButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::keyboardActionUp, this, _1));
		
		// Select Left Key button
		selectLeftKeyButton = gameLibrary.menuButton(GameLibrary::Race);
		selectLeftKeyButton->setPosition(Point(ColumnTwo, RowThree));
		selectLeftKeyButton->getUnTouchedEvent().connect(
										boost::bind(&MainMenuScene::selectLeftKeyActionUp, this, _1));

		// Select Right Key button
		selectRightKeyButton = gameLibrary.menuButton(GameLibrary::Race);
		selectRightKeyButton->setPosition(Point(ColumnTwo, RowFour));
		selectRightKeyButton->getUnTouchedEvent().connect(
											boost::bind(&MainMenuScene::selectRightKeyActionUp, this, _1));
	}
	
	void MainMenuScene::loadTipBox(Size tipBoxSize, Point boxLocation)
	{
		DrawableRectanglePtr tipBox = new DrawableRectangle(tipBoxSize.width(), tipBoxSize.height());
		tipBox->setColor(0.9, 0.9, 0.9, 0.5);
		tipBox->setPosition( Point( DefaultScreenLeft + boxLocation.x(), 
										DefaultScreenBottom + boxLocation.y()));
		tipBox->setPositionInterpretation(Drawable::E_SCREEN);
		tipBoxes.push_back(tipBox);
	}
	
	void MainMenuScene::loadTipLabel(String tipLine, Point tipLocation)
	{
		LabelPtr tipLabel = new Label(gameLibrary.theMilkmanConspiracyFont());
		tipLabel->setText(tipLine);
		tipLabel->setPositionInterpretation(Drawable::E_SCREEN);
		tipLabel->setPosition(Point(DefaultScreenLeft + tipLocation.x() + tipLabel->size().width()/2, 
									DefaultScreenBottom + tipLocation.y()));
		tipLabels.push_back(tipLabel);
	}
	
	void MainMenuScene::loadTipLine(std::vector<Point> points)
	{	
		DrawableLinePtr tipConnectorLine = new DrawableLine(points);
		tipConnectorLine->setPositionInterpretation(Drawable::E_SCREEN);
		tipLines.push_back(tipConnectorLine);
	}
	
	/* MAIN MENU, OPTIONS & TIPS
	 * USER BUTTON ACTION HANDLING FUNCTIONS
	 */
	
	//return to Main Menu from Tips or Options screen
	void MainMenuScene::backFromTipsActionUp(const TouchHandler&)
	{
		removeTipBoxes();
		removeTipLines();
		removeTipsLabels();
		
		removeChild(shotButton);
		removeChild(backButton);
		backButtonConnection.disconnect();

		removeChild(tipRageDemoBar);
		
		removeChild(dimmedBackground);
		removeChild(dimmedTipFrame);
		removeChild(shotgunBackground);
		removeChild(tipTitle);

		removeChild(policeTruckTip);
		removeChild(redTruckTip);
		removeChild(engineBlowTip);
		
		touchButtons.clear();
		
		addMainMenuButtons();
		
        state = E_MAIN_MENU;
	}
	
	void MainMenuScene::resumeActionUp(const TouchHandler&)
	{
		//startRace();
		//racePaused = false;
	}

	void MainMenuScene::raceActionUp(const TouchHandler&)
	{
		racePaused = false;
		startRace();
	}
	
	void MainMenuScene::optionsActionUp(const TouchHandler&)
	{
		removeMainMenuButtons();
		addChild(dimmedBackground, backgroundZOrder);
		addOptionsButtons();
        state = E_OPTIONS;
	}
	
	void MainMenuScene::tipsActionUp(const TouchHandler&)
	{
		removeMainMenuButtons();

		addChild(dimmedBackground, backgroundZOrder);
		addChild(dimmedTipFrame, backgroundZOrder);
		addChild(shotgunBackground, hudZOrder);
		addChild(tipTitle, hudZOrder);
		
		addTipBoxes();

		addChild(tipRageDemoBar, hudZOrder);
		
		addChild(policeTruckTip, hudZOrder);
		addChild(redTruckTip,hudZOrder);
		addChild(engineBlowTip, hudZOrder);
		
		addTipsLabels();
		addTipsButtons();
		addTipLines();
        state = E_TIPS;
	}
	
	//tips demo button
	void MainMenuScene::brakeActionDown(const TouchHandler&)
	{
		//add squeeling tires SFX
	}

	//tips demo button
	void MainMenuScene::shotActionDown(const TouchHandler&)
	{
		//add shotgun fire and reload SFX
		gameLibrary.shotgunSound()->play();
	}

	void MainMenuScene::soundActionUp(const TouchHandler&)
	{
		removeOptionsButtons();
		addSoundButtons();
	}

	void MainMenuScene::controlsActionUp(const TouchHandler&)
	{
		removeOptionsButtons();
		addControlsButtons();
	}

	void MainMenuScene::steeringActionUp(const TouchHandler&)
	{
		removeControlsButtons();
		addSteeringButtons();
	}

	void MainMenuScene::tiltActionUp(const TouchHandler&)
	{
		game().controlType = game().TILT;
		tiltButton->setPosition(Point(ColumnTwo, RowOne));
		saveControls();
		removeSteeringButtons();
		addControlsButtons();
	}

	void MainMenuScene::dPadActionUp(const TouchHandler&)
	{
		game().controlType = game().D_PAD;
		dPadButton->setPosition(Point(ColumnTwo, RowOne));
		saveControls();
		removeSteeringButtons();
		addControlsButtons();
	}

	void MainMenuScene::dPadAltActionUp(const TouchHandler&)
	{
		game().controlType = game().D_PAD_ALT;
		dPadAltButton->setPosition(Point(ColumnTwo, RowOne));
		saveControls();
		removeSteeringButtons();
		addControlsButtons();
	}

	void MainMenuScene::trackballActionUp(const TouchHandler&)
	{
		game().controlType = game().TRACKBALL;
		trackballButton->setPosition(Point(ColumnTwo, RowOne));
		saveControls();
		removeSteeringButtons();
		addControlsButtons();
	}

	void MainMenuScene::keyboardActionUp(const TouchHandler&)
	{
		game().controlType = game().KEYBOARD;
		keyboardButton->setPosition(Point(ColumnTwo, RowOne));
		saveControls();
		removeSteeringButtons();
		addControlsButtons();
	}

	void MainMenuScene::inversionActionUp(const TouchHandler&)
	{
		game().inverted = !game().inverted;
		saveControls();
		removeControlsButtons();
		addControlsButtons();
	}

	void MainMenuScene::selectLeftKeyActionUp(const TouchHandler&)
	{
		game().selectLeftKey = true;
		game().selectRightKey = false;
		removeControlsButtons();
		optionsLabel1->setPosition(Point(0, RowTwo));
		optionsLabel1->setText("SELECT LEFT BUTTON");
		addChild(optionsLabel1);
		backButtonConnection = backButton->getUnTouchedEvent().connect(
				boost::bind(&MainMenuScene::backFromSelectKeyScreenActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::selectRightKeyActionUp(const TouchHandler&)
	{
		game().selectRightKey = true;
		game().selectLeftKey= false;
		removeControlsButtons();
		optionsLabel1->setPosition(Point(0, RowTwo));
		optionsLabel1->setText("SELECT RIGHT BUTTON");
		addChild(optionsLabel1);
		backButtonConnection = backButton->getUnTouchedEvent().connect(
				boost::bind(&MainMenuScene::backFromSelectKeyScreenActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::musicActionUp(const TouchHandler&)
	{
		game().music = !game().music;
		gameLibrary.setMusicGain(game().music);
		saveControls();
		removeSoundButtons();
		addSoundButtons();
	}

	void MainMenuScene::sfxActionUp(const TouchHandler&)
	{
		game().sfx = !game().sfx;
		gameLibrary.setSoundGain(game().sfx);
		saveControls();
		removeSoundButtons();
		addSoundButtons();
	}

	void MainMenuScene::backFromOptionsActionUp(const TouchHandler&)
	{
		removeOptionsButtons();
		removeChild(dimmedBackground);
		
		addMainMenuButtons();
        state = E_MAIN_MENU;
	}
	void MainMenuScene::backFromSoundActionUp(const TouchHandler&)
	{	
		removeSoundButtons();
		addOptionsButtons();
	}
	void MainMenuScene::backFromControlsActionUp(const TouchHandler&)
	{
		removeControlsButtons();
		addOptionsButtons();
	}
	void MainMenuScene::backFromSteeringActionUp(const TouchHandler&)
	{
		removeSteeringButtons();
		steeringButton();
		addControlsButtons();
	}
	void MainMenuScene::backFromSelectKeyScreenActionUp(const TouchHandler&)
	{
		removeSelectKeyScreen();
		game().selectLeftKey = false;
		game().selectRightKey = false;
		addControlsButtons();
	}
	
	/* MAIN MENU, OPTIONS & TIPS
	 * CONTENT UPDATING FUNCTIONS
	 * based on user button actions
	 */ 
	void MainMenuScene::addMainMenuButtons()
	{	
		addChild(title);
		prepareButton(raceButton);
		prepareButton(optionsButton);
		prepareButton(tipsButton);
		//if(racePaused) { prepareButton(resumeButton); }
	}
	
	void MainMenuScene::removeMainMenuButtons()
	{
		removeChild(title);
		removeChild(raceButton);
		removeChild(optionsButton);
		removeChild(tipsButton);
		//if(racePaused) { removeChild(resumeButton); }
		
		touchButtons.clear();
	}

	void MainMenuScene::addTipLines()
	{
		foreach(DrawableLinePtr line, tipLines)
		{
			addChild(line, hudZOrder);
		}
	}
	
	void MainMenuScene::removeTipLines()
	{
		foreach(DrawableLinePtr line, tipLines)
		{
			removeChild(line);
		}
	}
	
	void MainMenuScene::addTipBoxes()
	{
		foreach(DrawableRectanglePtr box, tipBoxes)
		{
			addChild(box, hudZOrder);
		}
	}
	
	void MainMenuScene::removeTipBoxes()
	{
		foreach(DrawableRectanglePtr box, tipBoxes)
		{
			removeChild(box);
		}
	}

	void MainMenuScene::addTipsLabels()
	{
		foreach(LabelPtr tip, tipLabels)
		{
			addChild(tip, hudZOrder);
		}
	}
	
	void MainMenuScene::removeTipsLabels()
	{
		foreach(LabelPtr tip, tipLabels)
		{
			removeChild(tip);
		}
	}
	
	void MainMenuScene::addTipsButtons()
	{
		backButtonConnection = backButton->getUnTouchedEvent().connect(
									boost::bind(&MainMenuScene::backFromTipsActionUp, this, _1));
		prepareButton(backButton);
		prepareButton(shotButton);
		//prepareButton(brakeButton);
	}

	void MainMenuScene::addOptionsButtons()
	{	
		addChild(optionsTitle, hudZOrder);
		prepareButton(soundButton);
		prepareButton(controlsButton);
		backButtonConnection = backButton->getUnTouchedEvent().connect(
									boost::bind(&MainMenuScene::backFromOptionsActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::addSoundButtons()
	{
		addChild(optionsStereo, hudZOrder);
		
		optionsLabel1->setText("MUSIC:");
		addChild(optionsLabel1, hudZOrder);
		
		optionsLabel2->setText("FX:");
		addChild(optionsLabel2, hudZOrder);
		
		game().music ? prepareButton(onButton1) : prepareButton(offButton1);
		game().sfx ? prepareButton(onButton2) : prepareButton(offButton2);
		
		backButtonConnection = backButton->getUnTouchedEvent().connect(
									boost::bind(&MainMenuScene::backFromSoundActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::addControlsButtons()
	{
		optionsLabel1->setText("STEERING:");
		optionsLabel1->setPosition(Point(ColumnOne, RowTwo));
		
		addChild(optionsLabel1, hudZOrder);
		addChild(optionsWheel, hudZOrder);
		addChild(steeringBox, hudZOrder);
		steeringButton();
		
		switch(game().controlType)
		{
			case 0:
				prepareButton(tiltButton);
				break;
			case 1:
				prepareButton(dPadButton);
				break;
			case 2:
				prepareButton(dPadAltButton);
				optionsLabel2->setText("INVERSION:");
				addChild(optionsLabel2, hudZOrder);
				onButton1->setPosition(Point(ColumnTwo, RowThree));
				onButton1->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::inversionActionUp, this, _1));
				offButton1->setPosition(Point(ColumnTwo, RowThree));
				offButton1->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::inversionActionUp, this, _1));
				game().inverted ? prepareButton(onButton1) : prepareButton(offButton1);
				break;
			case 3:
				prepareButton(trackballButton);
				break;
			case 4:
				prepareButton(keyboardButton);
				optionsLabel2->setText("LEFT KEY:");
				addChild(optionsLabel2, hudZOrder);
				prepareButton(selectLeftKeyButton);
				removeChild(selectLeftKeyButton);
				optionsLabel3->setText("RIGHT KEY:");
				addChild(optionsLabel3, hudZOrder);
				prepareButton(selectRightKeyButton);
				removeChild(selectRightKeyButton);
				left->setText(string(1, char(game().left)));
				addChild(leftKeyBox, hudZOrder-1);
				addChild(left, hudZOrder);
				right->setText(string(1, char(game().right)));
				addChild(rightKeyBox, hudZOrder-1);
				addChild(right, hudZOrder);
				break;
		}
		backButtonConnection = backButton->getUnTouchedEvent().connect(
			boost::bind(&MainMenuScene::backFromControlsActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::addSteeringButtons()
	{
		optionsLabel1->setPosition(Point(0, RowTwo));
		optionsLabel1->setText("SELECT STEERING");
		
		addChild(optionsLabel1);
		addChild(optionsWheel, hudZOrder);
		
		switch(game().controlType)
		{
			case 0:
				tiltButtonConnection.disconnect();
				tiltButtonConnection = tiltButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::tiltActionUp, this, _1));
				break;
			case 1:
				dPadButtonConnection.disconnect();
				dPadButtonConnection = dPadButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::dPadActionUp, this, _1));
				break;
			case 2:
				dPadAltButtonConnection.disconnect();
				dPadAltButtonConnection = dPadAltButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::dPadAltActionUp, this, _1));
				break;
			case 3:
				trackballButtonConnection.disconnect();
				trackballButtonConnection = trackballButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::trackballActionUp, this, _1));
				break;
			case 4:
				keyboardButtonConnection.disconnect();
				keyboardButtonConnection = keyboardButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::keyboardActionUp, this, _1));
				break;
		}
		
		Rectangle rect = tiltButton->getBoundingRect();
		tiltButton->setPosition(Point(ColumnTwoLeftJustified + rect.width()/2, RowThree));
		prepareButton(tiltButton);
		
		rect = dPadButton->getBoundingRect();
		dPadButton->setPosition(Point(ColumnTwoLeftJustified + rect.width()/2, RowFour));
		prepareButton(dPadButton);
		/*
		rect = dPadAltButton->getBoundingRect();
		dPadAltButton->setPosition(Point(ColumnOneLeftJustified + rect.width()/2, RowFive));
		prepareButton(dPadAltButton);
		*/
		rect = trackballButton->getBoundingRect();
		trackballButton->setPosition(Point(ColumnOneLeftJustified + rect.width()/2, RowThree));
		prepareButton(trackballButton);
		
		rect = keyboardButton->getBoundingRect();
		keyboardButton->setPosition(Point(ColumnOneLeftJustified + rect.width()/2, RowFour));
		prepareButton(keyboardButton);
		
		backButtonConnection = backButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::backFromSteeringActionUp, this, _1));
		prepareButton(backButton);
	}

	void MainMenuScene::removeOptionsButtons(){
		
		removeChild(soundButton);
		removeChild(controlsButton);
		removeChild(backButton);
		backButtonConnection.disconnect();
		removeChild(optionsTitle);
		
		touchButtons.clear();
	}

	void MainMenuScene::removeSoundButtons(){
		
		removeChild(optionsStereo);
		removeChild(optionsLabel1);
		removeChild(onButton1);
		removeChild(offButton1);
		removeChild(optionsLabel2);
		removeChild(onButton2);
		removeChild(offButton2);
		removeChild(backButton);
		backButtonConnection.disconnect();
		
		touchButtons.clear();
	}

	void MainMenuScene::removeControlsButtons(){
		
		removeChild(optionsLabel1);
		removeChild(optionsWheel);
		
		switch(game().controlType)
		{
			case 0:
				removeChild(tiltButton);
				break;
			case 1:
				removeChild(dPadButton);
				break;
			case 2:
				/*
				removeChild(dPadAltButton);
				removeChild(optionsLabel2);
				removeChild(onButton1);
				removeChild(offButton1);
				*/
				break;
			case 3:
				removeChild(trackballButton);
				break;
			case 4:
				removeChild(keyboardButton);
				removeChild(optionsLabel2);
				removeChild(optionsLabel3);
				removeChild(left);
				removeChild(right);
				removeChild(leftKeyBox);
				removeChild(rightKeyBox);
				break;
			default:
				LOGE("Error: State: %d", game().controlType);
		}
		removeChild(backButton);
		backButtonConnection.disconnect();
		removeChild(steeringBox);
		touchButtons.clear();
	}

	void MainMenuScene::removeSteeringButtons()
	{
		removeChild(optionsWheel);
		removeChild(optionsLabel1);
		optionsLabel1->setPosition(Point(ColumnOne, RowOne));
		removeChild(tiltButton);
		removeChild(dPadButton);
		//removeChild(dPadAltButton);
		removeChild(trackballButton);
		removeChild(keyboardButton);
		removeChild(backButton);
		backButtonConnection.disconnect();

		touchButtons.clear();
	}

	void MainMenuScene::removeSelectKeyScreen()
	{
		removeChild(optionsLabel1);
		optionsLabel1->setPosition(Point(ColumnOne, RowTwo));
		removeChild(backButton);
		backButtonConnection.disconnect();

		touchButtons.clear();
	}

	void MainMenuScene::selectKeyScreenFinished()
	{
		saveControls();
		removeSelectKeyScreen();
		game().selectLeftKey = false;
		game().selectRightKey = false;
		addControlsButtons();
	}

	void MainMenuScene::loadControls()
	{
		
		LOGI("Opening user input settings file for read.");
		String contFile(game().getControlsFile());
		LOGI("User input settings file: %s", contFile.c_str());
		struct stat info;
		int state = -1;
		state = stat(contFile.c_str(), &info);
		if(state == 0)
		{
			std::ifstream file(contFile.c_str());
			string buffer;
			getline(file, buffer);//file >> buffer;

			LOGI("Buffer: %s", buffer.c_str());
			state = atoi(buffer.c_str());

			switch (state)
			{
				case 1:
					game().controlType = game().D_PAD;
					break;
				case 2:
					game().controlType = game().D_PAD_ALT;
					break;
				case 3:
					game().controlType = game().TRACKBALL;
					break;
				case 4:
					game().controlType = game().KEYBOARD;
					break;
				default:
					LOGASSERT(0 == state, "This should not be a control state: %d", state);
					game().controlType = game().TILT;
					break;
			}

			getline(file, buffer);
			LOGI("Buffer - inverted: %s", buffer.c_str());
			game().inverted = (buffer == "0") ? false : true;
			getline(file, buffer);
			state = atoi(buffer.c_str());
			game().left = (char) state;
			LOGI("Buffer - left: %d", game().left);
			getline(file, buffer);
			state = atoi(buffer.c_str());
			game().right = (char) state;
			LOGI("Buffer - right: %d", game().right);
			getline(file, buffer);
			LOGI("Buffer - music: %s", buffer.c_str());
			game().music = (buffer == "0") ? false : true;
			getline(file, buffer);
			LOGI("Buffer - sfx: %s", buffer.c_str());
			game().sfx = (buffer == "0") ? false : true;
			file.close();
		}
		else
		{
			game().controlType = game().TILT;
			game().inverted = false;
			game().left = 'a';
			game().right = 'd';
			game().music = true;
			game().sfx = true;
		}

		gameLibrary.setSoundGain(game().sfx ? 1.0 : 0.0);
		gameLibrary.setMusicGain(game().music ? 1.0 : 0.0);

		LOGD("Successfully loaded the user settings.");
		
	}

	void MainMenuScene::saveControls()
	{
		
		LOGI("Opening user control settings file for write.");
		String contFile(game().getControlsFile());
		LOGI("User control settings file: %s", contFile.c_str());
		std::ofstream file(contFile.c_str());
		file << game().controlType;
		file << '\n';
		file << game().inverted;
		file << '\n';
		file << game().left;
		file << '\n';
		file << game().right;
		file << '\n';
		file << game().music;
		file << '\n';
		file << game().sfx;
		file << '\n';
		file.close();
		LOGD("Successfully saved user settings.");
		
	}

	void MainMenuScene::steeringButton()
	{
		switch(game().controlType)
		{
			case 0:
				tiltButton->setPosition(Point(ColumnTwo, RowTwo));
				tiltButtonConnection.disconnect();
				tiltButtonConnection = tiltButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::steeringActionUp, this, _1));
				break;
			case 1:
				dPadButton->setPosition(Point(ColumnTwo, RowTwo));
				dPadButtonConnection.disconnect();
				dPadButtonConnection = dPadButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::steeringActionUp, this, _1));
				break;
			case 2:
				dPadAltButton->setPosition(Point(ColumnTwo, RowTwo));
				dPadAltButtonConnection.disconnect();
				dPadAltButtonConnection = dPadAltButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::steeringActionUp, this, _1));
				break;
			case 3:
				trackballButton->setPosition(Point(ColumnTwo, RowTwo));
				trackballButtonConnection.disconnect();
				trackballButtonConnection = trackballButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::steeringActionUp, this, _1));
				break;
			case 4:
				keyboardButton->setPosition(Point(ColumnTwo, RowTwo));
				keyboardButtonConnection.disconnect();
				keyboardButtonConnection = keyboardButton->getUnTouchedEvent().connect(boost::bind(&MainMenuScene::steeringActionUp, this, _1));
				break;
			default:
				LOGE("Error: State: %d", game().controlType);
		}
	}

    void MainMenuScene::handleBackInput()
    {
        switch (state) {
            case E_MAIN_MENU:
                exit(0);
                break;
                
            default:
				backButton->setState(TouchHandler::Touched);
				backButton->setState(TouchHandler::Untouched);
                break;
        }
    }
}

