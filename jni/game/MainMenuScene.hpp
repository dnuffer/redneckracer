// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef MAIN_MENU_SCENE_HPP_INCLUDED
#define MAIN_MENU_SCENE_HPP_INCLUDED

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "GameLibrary.hpp"
#include "engine/EngineFwd.hpp"
#include "engine/Scene.hpp"
#include "engine/TextureLibrary.hpp"
#include "miniblocxx/stl/vector"
#include "miniblocxx/Array.hpp"
#include "TruckController.hpp"
#include "engine/Label.hpp"
#include "engine/ProgressBar.hpp"
#include "engine/DrawableRectangle.hpp"
#include "boost/signals2.hpp"

namespace rr
{
	class MainMenuScene : public Scene
	{
	public:
		MainMenuScene(GameLibrary& gameLibrary);
		virtual ~MainMenuScene();

		virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);

		virtual void handleTouchEvent(const TouchEvent& touchEvent);

		void setRacePaused();
		bool isRacePaused() const;

		void selectKeyScreenFinished();

        void handleBackInput();

		static const int backgroundZOrder = -1;
		static const int obstacleZOrder = 10;
		static const int animalZOrder = 20;
		static const int truckZOrder = 30;
		static const int sparksZOrder = 40;
		static const int hudZOrder = 100;

		static const float scaleFactor = 1.7;

	private:

		// Callback function to interact with menu button.
		void resumeActionUp(const TouchHandler&);
		void raceActionUp(const TouchHandler&);
		void optionsActionUp(const TouchHandler&);
		void tipsActionUp(const TouchHandler&);
		
		// Tip Screen demo buttons
		void brakeActionDown(const TouchHandler&);
		void shotActionDown(const TouchHandler&);
		
		// Tip Handling Functions
		void loadTipLine(std::vector<Point> points);
		void loadTipLabel(String tipLine, Point tipLocation);
		void loadTipBox(Size tipBoxSize, Point boxLocation);
		
		void addTipsButtons();
		void addTipsLabels();
		void addTipLines();
		void addTipBoxes();
		
		void removeMainMenuButtons();
		void removeTipsLabels();
		void removeTipLines();
		void removeTipBoxes();

		// Callback functions to interact with options buttons.
		
		// Options menu buttons
		void soundActionUp(const TouchHandler&);
		void controlsActionUp(const TouchHandler&);

		// Sound menu buttons
		void musicActionUp(const TouchHandler&);
		void sfxActionUp(const TouchHandler&);

		// Controls buttons
		void steeringActionUp(const TouchHandler&);
		void inversionActionUp(const TouchHandler&);
		void selectLeftKeyActionUp(const TouchHandler&);
		void selectRightKeyActionUp(const TouchHandler&);

		// Steering buttons
		void tiltActionUp(const TouchHandler&);
		void dPadActionUp(const TouchHandler&);
		void dPadAltActionUp(const TouchHandler&);
		void trackballActionUp(const TouchHandler&);
		void keyboardActionUp(const TouchHandler&);

		// Back buttons
		void backFromOptionsActionUp(const TouchHandler&);	// Return to MainMenu from Options
		void backFromSoundActionUp(const TouchHandler&);	// Return to Options from Sound
		void backFromControlsActionUp(const TouchHandler&);// Return to Options from Controls
		void backFromSteeringActionUp(const TouchHandler&);// Return to Controls from Steering
		void backFromSelectKeyScreenActionUp(const TouchHandler&);
		void backFromTipsActionUp(const TouchHandler&);	// Return to MainMenu from Tips

	// Additional options-related methods
		// Button Preparations
		void loadButtons();
		void loadMainMenuButtons();
		void loadOptionsButtons();
		void loadTipsButtons();

		// Button Loads (add/remove)
		void addMainMenuButtons();
		void addOptionsButtons();
		void addSoundButtons();
		void addControlsButtons();
		void addSteeringButtons();
		
		void removeOptionsButtons();
		void removeSoundButtons();
		void removeControlsButtons();
		void removeSteeringButtons();
		void removeSelectKeyScreen();

		// Control Settings
		void loadControls();
		void saveControls();

		
		void restoreButton();
		void steeringButton();

		void drawBackground();
		void addButtons();
		void prepareButton(TouchButtonPtr button);
		void resetElapsedTime();
		void startRace();

	private:
	
		GameLibrary& gameLibrary;
		
		SpritePtr title;
		
		TouchButtonPtr raceButton;
		TouchButtonPtr optionsButton;
		TouchButtonPtr tipsButton;
		TouchButtonPtr backButton;
		boost::signals2::connection backButtonConnection;
		
		boost::signals2::connection tiltButtonConnection;
		boost::signals2::connection dPadButtonConnection;
		boost::signals2::connection dPadAltButtonConnection;
		boost::signals2::connection trackballButtonConnection;
		boost::signals2::connection keyboardButtonConnection;
		
		bool racePaused;

		enum MenuState
		{
			E_MAIN_MENU,
			E_OPTIONS,
			E_TIPS
		};
		
		MenuState state;
		
		TimeDuration elapsedTime;
		std::vector<TouchButtonPtr> touchButtons;
		
		// Darkens background for options and tips
		DrawableRectanglePtr dimmedBackground;
		DrawableRectanglePtr dimmedTipFrame;
		
		// Tip Assets
		
		std::vector<LabelPtr> tipLabels;
		std::vector<DrawableLinePtr> tipLines;
		std::vector<DrawableRectanglePtr> tipBoxes;
		std::vector<SpritePtr> tipTextures;
		std::vector<TouchButtonPtr> tipButtons;
		
		TouchButtonPtr shotButton;

		ProgressBarPtr tipDefenseDemoBar;
		ProgressBarPtr tipArmorDemoBar;
		ProgressBarPtr tipRageDemoBar;
			
		SpritePtr policeTruckTip;
		SpritePtr engineBlowTip;
		SpritePtr redTruckTip;
		SpritePtr optionsTitle;
		SpritePtr optionsStereo;
		SpritePtr optionsWheel;
		SpritePtr tipTitle;
		SpritePtr shotgunBackground;
		
		// Options Assets
		
		TouchButtonPtr soundButton;
		TouchButtonPtr controlsButton;
		TouchButtonPtr onButton1;
		TouchButtonPtr offButton1;
		TouchButtonPtr onButton2;
		TouchButtonPtr offButton2;

		LabelPtr optionsLabel1;
		LabelPtr optionsLabel2;
		LabelPtr optionsLabel3;
		LabelPtr left;
		LabelPtr right;
		
		DrawableRectanglePtr leftKeyBox;
		DrawableRectanglePtr rightKeyBox;
		DrawableRectanglePtr steeringBox;

		TouchButtonPtr tiltButton;
		TouchButtonPtr dPadButton;
		TouchButtonPtr dPadAltButton;
		TouchButtonPtr trackballButton;
		TouchButtonPtr keyboardButton;
		TouchButtonPtr selectLeftKeyButton;
		TouchButtonPtr selectRightKeyButton;
	};

}

#endif
