// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_GameLibrary_hpp_INCLUDED_
#define game_GameLibrary_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "engine/TextureLibrary.hpp"
#include "Animal.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Array.hpp"
#include "engine/TouchButton.hpp"
#include <tr1/functional>


namespace rr
{
	class GameLibrary
	{
	public:
		GameLibrary(const TextureLibraryPtr& textureLibrary);
		
        ~GameLibrary();

		// should be called to load cache of TexturedQuadPtrs/Animations.
		void load();
		void loadMenuButtons();
		void loadOptionsButtons();
		void loadTrackSelectButtons();
		
		void setSoundGain(float gain);
		void setMusicGain(float gain);
		
		void setProgressFunction(const std::tr1::function<void (float)>& progressCallback);

		// sprites
		PlayerTruckPtr playerTruck(float& rollAngle) const;
		
		enum TruckColor
		{
			Red, Black, Blue, Brown, Green, Yellow, TruckColorMax
		};

		PoliceTruckPtr policeTruck() const;
		TexturedQuadPtr policeStopped() const;
		AnimationPtr policeMovingAnim() const;
		AnimationPtr policeTurningLeft() const;
		AnimationPtr policeTurningRight() const;
		AnimationPtr policeDrivingLeft() const;
		AnimationPtr policeDrivingRight() const;
		AnimationPtr policeTurningLeftToStraight() const;
		AnimationPtr policeTurningRightToStraight() const;

		OpponentTruckPtr opponentTruck(TruckColor color) const;
		TexturedQuadPtr truckStopped(TruckColor color) const;
		AnimationPtr truckAccelerating(TruckColor color) const;
		AnimationPtr truckDrivingStraight(TruckColor color) const;
		AnimationPtr truckTurningLeft(TruckColor color) const;
		AnimationPtr truckTurningRight(TruckColor color) const;
		AnimationPtr truckDrivingLeft(TruckColor color) const;
		AnimationPtr truckDrivingRight(TruckColor color) const;
		AnimationPtr truckTurningLeftToStraight(TruckColor color) const;
		AnimationPtr truckTurningRightToStraight(TruckColor color) const;

		CivilCarPtr civilCar(TruckColor color) const;
		AnimationPtr carAccelerating(TruckColor color) const;
		AnimationPtr carDrivingStraight(TruckColor color) const;
		AnimationPtr carTurningLeft(TruckColor color) const;
		AnimationPtr carTurningRight(TruckColor color) const;
		AnimationPtr carDrivingLeft(TruckColor color) const;
		AnimationPtr carDrivingRight(TruckColor color) const;
		AnimationPtr carTurningLeftToStraight(TruckColor color) const;
		AnimationPtr carTurningRightToStraight(TruckColor color) const;

		enum AnimalSpecies
		{
			Armadillo, Snake, Possum, Raccoon, Squirrel
		};
		AnimalPtr animal(AnimalSpecies species, Animal::EMovementDirection direction) const;
		
		enum ObstacleType
		{
			Puddle = 0,
			Pothole,
			FatGuy,
			Outhouse,
			Shrub,
			Tree,
			Tractor
		};
		ObstaclePtr obstacle(ObstacleType type) const;
		
		enum ButtonType
		{
			Brake = 0,
			Shot,
			Menu,
			BackToMenu,
			RateIt,
			Later,
			Never
		};

		enum MenuButtonType
		{
			Race,
			Options, 
			Tips
		};
		
		enum OptionsButtonType
		{
			E_OPTION_AUDIO,
			E_OPTION_CONTROLS,
			E_OPTION_ON,
			E_OPTION_OFF,
			E_OPTION_TILT,
			E_OPTION_DPAD,
			E_OPTION_DPAD_ALT,
			E_OPTION_TRACKBALL,
			E_OPTION_KEYBOARD
		};
		
		enum TrackSelectButtonType {
			E_YANKEE_SHOT,
			E_EVERWHICHAWAYS,
			E_UHMURKIN,
			E_DADGUM_TRAKTURS,
			E_REAL_BAMMER,
			E_WIJADIDJA
		};

		TouchButtonPtr button(ButtonType type) const;
		TouchButtonPtr menuButton(MenuButtonType type) const;
		TouchButtonPtr optionsButton(OptionsButtonType type) const;
		TouchButtonPtr trackSelectButton(TrackSelectButtonType type) const;

		/*
		 * These types of animations can share frames between trucks
		 * b/c they do not flip TexturedQuads
		 */ 
		enum EffectType
		{
			ShotgunBlast = 0,
			Sparks,
			FireAndSmoke
		};
		
		AnimationPtr effect(EffectType type) const;
		
		/*
		 * These types of animations flip TexturedQuads horizontally
		 * and need to be truck specific --- trucks should not share frames
		 */
		
		enum ExhaustType
		{
			E_FLAMES_DRIVE_LEFT,
			E_FLAMES_DRIVE_RIGHT,
			E_FLAMES_MAX
		};
		
		SpritePtr exhaustFlamesDrivingStraight() const;
		SpritePtr exhaustFlamesDrivingLeft() const;
		SpritePtr exhaustFlamesDrivingRight() const;
		
		// fonts
		TexturedFontPtr theMilkmanConspiracyFont() const;
		
		// backgrounds
		StringArray listBackgroundQuads() const;
		TexturedQuadPtr backgroundQuad(const std::string& name) const;
		SpritePtr finishLine() const;
		SpritePtr loadingSprite(const std::string& name) const;
		
		// misc
		void loadRaceAtlases() const;
		void loadMenuAtlases() const;
		void unloadRaceAtlases() const;

        // music
        SoundPtr backgroundMusic() const;
        
        // sounds
        SoundPtr shotgunSound() const;
        SoundPtr gunPingSound() const;
        SoundPtr truckStartupSound() const;
        SoundPtr truckRevFadeSound() const;
        SoundPtr copsComingSound() const;

	private:
		const TextureLibraryPtr _textureLibrary;
		
		std::vector<std::vector<TexturedQuadPtr> > _obstaclesFrames;
        std::vector<SoundPtr> _obstaclesSounds;
		std::vector<std::vector<TexturedQuadPtr> > _menuButtonsFrames;
		std::vector<std::vector<TexturedQuadPtr> > _optionsButtonsFrames;
		std::vector<std::vector<TexturedQuadPtr> > _trackSelectButtonsFrames;
		std::vector<std::vector<TexturedQuadPtr> > _buttonsFrames;
		std::vector<std::vector<TexturedQuadPtr> > _effectsFrames;
		
		//  EXHAUST FLAME ANIMATIONS FOR EACH TRUCK (except driving straight b/c the trucks share ExhaustStraight Quads)
		//   1)TruckColor  2)ExhaustType  3)Frames of Each Animation
		std::vector<std::vector<TexturedQuadPtr> >  _exhaustFrames;
		StringArray _exhaustStraightFrameNames;
		
		std::vector<StringArray> _animalFrameNames;
		std::tr1::function<void (float)> progressCallback;
		SoundPtr _backgroundMusic;
        SoundPtr _shotgunSound;
        SoundPtr _gunPingSound;
        SoundPtr _animalSound;
        std::vector<SoundPtr> _rageSounds;
        SoundPtr _truckStartupSound;
        SoundPtr _truckRevFadeSound;
        SoundPtr _copsComingSound;
        SoundPtr _racersCrashSound;
        /* WARNING!!! MAKE SURE YOU ADD NEW SOUND EFFECTS
         *  INTO FUNCTION setSoundGain(); OTHERWISE, THE
         *  SFX ON/OFF FUNCTIONALITY IS BROKEN!!!
         */
	};
}

#endif
