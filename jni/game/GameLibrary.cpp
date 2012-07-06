// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "GameLibrary.hpp"
#include "PlayerTruck.hpp"
#include "OpponentTruck.hpp"
#include "Obstacle.hpp"
#include "Animal.hpp"
#include "miniblocxx/Array.hpp"
#include "engine/TexturedFont.hpp"
#include "engine/Log.hpp"
#include "CivilCar.hpp"
#include "boost/foreach.hpp"
#define foreach BOOST_FOREACH
#include "engine/Sound.hpp"

namespace rr
{
	namespace
	{
		template <typename T, size_t N>
		char ( &_ArraySizeHelper( T (&array)[N] ))[N];
		#define sizeof_array( array ) (sizeof( _ArraySizeHelper( array ) ))
		
		
		void progress(float f)
		{
			//LOGD("We're making progress! Currently at %02.2f%%", f*100);
		}
		
		const char* truckColorStr(GameLibrary::TruckColor color)
		{
			const char* names[] = {"Red", "Black", "Blue", "Brown", "Green", "Yellow"};
			LOGASSERT((size_t)color <= sizeof_array(names), "Invalid color or other bug: %zu", (size_t)color);
			return names[(size_t)color];
		}
		
		const char* carColorStr(GameLibrary::TruckColor color)
		{
			return truckColorStr(color);
		}
		Array<string> frameNames(const char* formatStr, int first, int last)
		{
			Array<string> names(last - first);
			for (int i = first; i <= last; ++i)
				names[i - first] = Format(formatStr, i);
			return names;
		}
		
		string truckStraightName(GameLibrary::TruckColor color, int frame)
		{
			return Format("%1TruckDrivesNorth%<2:04>", truckColorStr(color), frame);
		}
	}

    GameLibrary::GameLibrary(const TextureLibraryPtr& textureLibrary)
        : _textureLibrary(textureLibrary)
        , progressCallback(NULL)
    {}

    GameLibrary::~GameLibrary()
    {
    }
    
    void GameLibrary::setSoundGain(float gain)
    {
		_shotgunSound->setGain(gain);
		_gunPingSound->setGain(gain);
		_animalSound->setGain(gain);
		_truckStartupSound->setGain(gain);
		_truckRevFadeSound->setGain(gain);
		_copsComingSound->setGain(gain);
		_racersCrashSound->setGain(gain);

		foreach(const SoundPtr & sound, _rageSounds)
		{
			if(sound)
				sound->setGain(gain);
		}
		foreach(const SoundPtr & sound, _obstaclesSounds)
		{
			if(sound)
				sound->setGain(gain);
		}
	}
	
	void GameLibrary::setMusicGain(float gain)
    {
		_backgroundMusic->setGain(gain);
	}
	
	void GameLibrary::setProgressFunction(const std::tr1::function<void (float)>& progressCbck)
	{
		progressCallback = progressCbck;
	}

	CivilCarPtr GameLibrary::civilCar(TruckColor color) const
	{
		string name = Format("%1CarDrivesNorth0001", truckColorStr(color));
		return new CivilCar(_textureLibrary->texturedQuad(name),
				carAccelerating(color),
				carDrivingStraight(color),
				carTurningLeft(color),
				carDrivingLeft(color),
				carTurningLeftToStraight(color),
				carTurningRight(color),
				carDrivingRight(color),
				carTurningRightToStraight(color),
				name,
                _racersCrashSound);
	}

	TexturedQuadPtr GameLibrary::policeStopped() const
	{
		return _textureLibrary->texturedQuad("PoliceTruckDrivesNorth0001");
	}

	AnimationPtr GameLibrary::policeTurningLeft() const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads("PoliceTruckTurnsLeft"), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeTurningRight() const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads("PoliceTruckTurnsRight"), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeDrivingLeft() const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads("PoliceTruckDrivesLeft"), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeDrivingRight() const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads("PoliceTruckDrivesRight"), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeTurningLeftToStraight() const
	{
		StringArray frameNames = _textureLibrary->listQuads("PoliceTruckTurnsLeft");
		std::reverse(frameNames.begin(), frameNames.end());
		return _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeTurningRightToStraight() const
	{
		StringArray frameNames = _textureLibrary->listQuads("PoliceTruckTurnsRight");
		std::reverse(frameNames.begin(), frameNames.end());
		return _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::policeMovingAnim() const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads("PoliceTruckDrivesNorth"), Animation::E_LOOP, 15);
	}

	PoliceTruckPtr GameLibrary::policeTruck() const
	{
	  string name = "PoliceTruckDrivesNorth";
	  return new PoliceTruck(policeStopped(),
			  	  	  	  	 policeMovingAnim(),
			  	  	  	  	 policeMovingAnim(),
			  	  	  	  	 policeTurningLeft(),
			  	  	  	  	 policeDrivingLeft(),
			  	  	  	  	 policeTurningLeftToStraight(),
			  	  	  	  	 policeTurningRight(),
			  	  	  	  	 policeDrivingRight(),
			  	  	  	  	 policeTurningRightToStraight(),
			  	  	  	  	 name,
                             _racersCrashSound);
	}
	PlayerTruckPtr GameLibrary::playerTruck(float& rollAngle) const
	{ 
		return new PlayerTruck(rollAngle, 
							   _textureLibrary->texturedQuad(truckStraightName(Red, 1)), 
							   truckAccelerating(Red),
							   truckDrivingStraight(Red),
							   truckTurningLeft(Red),
							   truckDrivingLeft(Red),
							   truckTurningLeftToStraight(Red),
							   truckTurningRight(Red),
							   truckDrivingRight(Red),
							   truckTurningRightToStraight(Red),
							   truckStraightName(Red, 1),
                               _rageSounds,
                               _racersCrashSound); 
	}
	
	OpponentTruckPtr GameLibrary::opponentTruck(TruckColor color) const
	{
		string name = truckStraightName(color, 1);
		LOGD("TRUCK NAME: %s", name.c_str());
		
		return new OpponentTruck(_textureLibrary->texturedQuad(name), 
								 truckAccelerating(color),
								 truckDrivingStraight(color),
								 truckTurningLeft(color),
								 truckDrivingLeft(color),
								 truckTurningLeftToStraight(color),
								 truckTurningRight(color),
								 truckDrivingRight(color),
								 truckTurningRightToStraight(color),
								 name,
                                 _racersCrashSound);
	}
	
	TexturedQuadPtr GameLibrary::truckStopped(TruckColor color) const
	{
		//LOGD("GameLibrary::truckStopped");
		return _textureLibrary->texturedQuad(Format("%1TruckDrivesNorth0001", truckColorStr(color)));
	}
	
	AnimationPtr GameLibrary::truckAccelerating(TruckColor color) const
	{
		//LOGD("GameLibrary::truckAccelerating");

		// truck plays accelerating animation (5 frames/1 sec)
		vector<string> truckFrameNames(5);
		for (int i = 1; i <= 5; ++i)
			truckFrameNames[i - 1] = truckStraightName(color, i);
		return _textureLibrary->animationFromAtlasKeys(truckFrameNames, Animation::E_PLAY_ONCE, 5);
	}
	
	AnimationPtr GameLibrary::truckDrivingStraight(TruckColor color) const
	{
		//LOGD("GameLibrary::truckDrivingStraight");

		vector<string> truckFrameNames;
		for (int i = 6; i <= 10; ++i)
			truckFrameNames.push_back(Format("%1TruckDrivesNorth%<2:04>", truckColorStr(color), i));
		return _textureLibrary->animationFromAtlasKeys(truckFrameNames, Animation::E_LOOP, 15);
		
		
	}
	
	AnimationPtr GameLibrary::truckTurningLeft(TruckColor color) const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1TruckTurnsLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
	}
	
	AnimationPtr GameLibrary::truckTurningRight(TruckColor color) const
	{
		AnimationPtr truckTurningLeft = _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1TruckTurnsLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
		truckTurningLeft->setFlippedHorizontal(true); //Sprite resets animation flip, otherwise this would work
		return truckTurningLeft;
	}
		
	AnimationPtr GameLibrary::truckDrivingLeft(TruckColor color) const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1TruckDrivesLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
	}
		
	AnimationPtr GameLibrary::truckDrivingRight(TruckColor color) const
	{
		AnimationPtr truckDrivingLeft = _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1TruckDrivesLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
		truckDrivingLeft->setFlippedHorizontal(true); //Sprite resets animation flip, otherwise this would work
		return truckDrivingLeft;
	}
	
	AnimationPtr GameLibrary::truckTurningLeftToStraight(TruckColor color) const
	{
		StringArray frameNames = _textureLibrary->listQuads(Format("%1TruckTurnsLeft", truckColorStr(color)).c_str());
		std::reverse(frameNames.begin(), frameNames.end());
		return _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
	}
	
	AnimationPtr GameLibrary::truckTurningRightToStraight(TruckColor color) const
	{
		StringArray frameNames = _textureLibrary->listQuads(Format("%1TruckTurnsLeft", truckColorStr(color)).c_str());
		std::reverse(frameNames.begin(), frameNames.end());
		AnimationPtr truckTurnsLeftToStraight = _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
		truckTurnsLeftToStraight->setFlippedHorizontal(true);
		return truckTurnsLeftToStraight;
	}
	SpritePtr GameLibrary::exhaustFlamesDrivingStraight() const
	{
		//LOGD("GameLibrary::exhaustFlamesDrivingStraight()");
		AnimationPtr exhaustAnim =_textureLibrary->animationFromAtlasKeys(_exhaustStraightFrameNames, Animation::E_LOOP, 15.0);
		exhaustAnim->setScaleY(0.7);
		SpritePtr exhaustFlamesSprite = new Sprite(exhaustAnim, "ExhaustFlameStraight");
		exhaustFlamesSprite->setPosition(Point(0.0, -53.0));
		return exhaustFlamesSprite;
	}
	SpritePtr GameLibrary::exhaustFlamesDrivingLeft() const
	{	
		AnimationPtr exhaustAnim = _textureLibrary->animationFromAtlasKeys(_exhaustStraightFrameNames, Animation::E_LOOP, 15.0);
		exhaustAnim->setScale(0.6);
		SpritePtr exhaustFlamesSprite = new Sprite(exhaustAnim, "ExhaustFlamesLeft");
		exhaustFlamesSprite->setPosition(Point(24.0, -48.0));
		exhaustFlamesSprite->setRotation(-24.0);
		return exhaustFlamesSprite;
	}
	SpritePtr GameLibrary::exhaustFlamesDrivingRight() const
	{
		AnimationPtr exhaustAnim = _textureLibrary->animationFromAtlasKeys(_exhaustStraightFrameNames, Animation::E_LOOP, 15.0);
		exhaustAnim->setScale(0.6);
		exhaustAnim->setFlippedHorizontal(true);
		SpritePtr exhaustFlamesSprite = new Sprite(exhaustAnim, "ExhaustFlamesRight");
		exhaustFlamesSprite->setPosition(Point(-24.0,-48.0));
		exhaustFlamesSprite->setRotation(24.0);
		return exhaustFlamesSprite;
	}
	
	AnimationPtr GameLibrary::carAccelerating(TruckColor color) const
	{
		return carDrivingStraight(color);
	}

	AnimationPtr GameLibrary::carDrivingStraight(TruckColor color) const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1CarDrivesNorth", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::carTurningLeft(TruckColor color) const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1CarTurnsLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::carTurningRight(TruckColor color) const
	{
		AnimationPtr carTurnsLeft = _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1CarTurnsLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
		carTurnsLeft->setFlippedHorizontal(true);
		return carTurnsLeft;
	}

	AnimationPtr GameLibrary::carDrivingLeft(TruckColor color) const
	{
		return _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1CarDrivesLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::carDrivingRight(TruckColor color) const
	{
		AnimationPtr carDrivesLeft = _textureLibrary->animationFromAtlasKeys(_textureLibrary->listQuads(Format("%1CarDrivesLeft", truckColorStr(color)).c_str()), Animation::E_LOOP, 15);
		carDrivesLeft->setFlippedHorizontal(true);
		return carDrivesLeft;
	}

	AnimationPtr GameLibrary::carTurningLeftToStraight(TruckColor color) const
	{
		StringArray frameNames = _textureLibrary->listQuads(Format("%1CarTurnsLeft", truckColorStr(color)).c_str());
		std::reverse(frameNames.begin(), frameNames.end());
		return _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
	}

	AnimationPtr GameLibrary::carTurningRightToStraight(TruckColor color) const
	{
		StringArray frameNames = _textureLibrary->listQuads(Format("%1CarTurnsLeft", truckColorStr(color)).c_str());
		std::reverse(frameNames.begin(), frameNames.end());
		AnimationPtr carTurnsRightToStraight = _textureLibrary->animationFromAtlasKeys(frameNames, Animation::E_LOOP, 15);
		carTurnsRightToStraight->setFlippedHorizontal(true);
		return carTurnsRightToStraight;
	}

	
	AnimalPtr GameLibrary::animal(AnimalSpecies species, Animal::EMovementDirection direction) const
	{
		LOGD("GameLibrary::animal");

		const char* deadNames[] = { "ArmadilloDead", "SnakeDead", "PossumDead", "RaccoonDead", "SquirrelDead" };
        const char* names[] = { "Armadillo", "Snake", "Possum", "Raccoon", "Squirrel" };
		LOGASSERT((size_t)species <= sizeof_array(deadNames), "Invalid animal species or other bug: %zu", (size_t)species);
		LOGASSERT((size_t)species <= _animalFrameNames.size(), "Invalid animal species or other bug: %zu", (size_t)species);
		
		return new Animal(_textureLibrary->animationFromAtlasKeys(_animalFrameNames[(size_t)species], Animation::E_LOOP), 
						  _textureLibrary->texturedQuad(deadNames[(size_t)species]),
						  direction, names[(size_t)species], _animalSound);
	}
	
	ObstaclePtr GameLibrary::obstacle(ObstacleType type) const
	{
		LOGASSERT((size_t)type <= _obstaclesFrames.size(), "Invalid obstacle type: %zu", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_obstaclesFrames[(size_t)type]);
		ObstaclePtr op = new Obstacle(frames[0], new Animation(frames), _obstaclesSounds[(size_t)type]);
		if (type == Pothole || type == Puddle) {
			op->makeInvincible();
		}
		return op;
	}
	TouchButtonPtr GameLibrary::button(ButtonType type) const
	{
		LOGASSERT((size_t)type <= _buttonsFrames.size(), "Invalid button type: %zu", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_buttonsFrames[(size_t)type]);
		return new TouchButton(frames);
	}
	
	AnimationPtr GameLibrary::effect(EffectType type) const
	{
		LOGASSERT((size_t)type <= _effectsFrames.size(), "Invalid effect type: %zu", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_effectsFrames[(size_t)type]);
		return new Animation(frames);
	}

	TouchButtonPtr GameLibrary::menuButton(MenuButtonType type) const
	{
		LOGASSERT((size_t)type <= _menuButtonsFrames.size(), "Invalid button type: %zu", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_menuButtonsFrames[(size_t)type]);
		return new TouchButton(frames);
	}

	TouchButtonPtr GameLibrary::optionsButton(OptionsButtonType type) const
	{
		LOGASSERT((size_t)type <= _optionsButtonsFrames.size(), "Invalid options button type: %d", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_optionsButtonsFrames[(size_t)type]);
		return new TouchButton(frames);
	}
	
	TouchButtonPtr GameLibrary::trackSelectButton(TrackSelectButtonType type) const
	{
		LOGASSERT((size_t)type <= _trackSelectButtonsFrames.size(), "Invalid track select button type: %d", (size_t)type);
		const vector<TexturedQuadPtr>& frames(_trackSelectButtonsFrames[(size_t)type]);
		return new TouchButton(frames);
	}

	TexturedFontPtr GameLibrary::theMilkmanConspiracyFont() const
	{
		//LOGD("GameLibrary::theMilkmanConspiracyFont");

		const char* name = "TheMilkmanConspiracy";
		_textureLibrary->loadGroup(name, /*progress*/progressCallback);
		return new TexturedFont(_textureLibrary, name);
	}
	
	StringArray GameLibrary::listBackgroundQuads() const
	{
		return _textureLibrary->listQuads("background_grassy_");
	}
	
	TexturedQuadPtr GameLibrary::backgroundQuad(const std::string& name) const
	{
		//LOGD("GameLibrary::backgroundQuad");

		return _textureLibrary->texturedQuad(name);
	}
	
	SpritePtr GameLibrary::finishLine() const
	{
		return new Sprite(_textureLibrary->texturedQuad("FinishLine"), "FinishLine");
	}

	SpritePtr GameLibrary::loadingSprite(const std::string& name) const
	{
		return new Sprite(_textureLibrary->texturedQuad(name), name);
	}
	
	void GameLibrary::loadRaceAtlases() const
	{
		_textureLibrary->loadGroup("race", /*progress*/progressCallback);
		
		_textureLibrary->loadGroup("background_grassy", /*progress*/progressCallback);
	
	}
	
	void GameLibrary::loadMenuAtlases() const
	{
		_textureLibrary->loadGroup("loading", /*progress*/progressCallback);
	}
	
	void GameLibrary::unloadRaceAtlases() const
	{
		_textureLibrary->unloadGroup("race");
	}

	void GameLibrary::loadMenuButtons()
	{
		_menuButtonsFrames.resize(3);
		_textureLibrary->animationFrames("Race", _menuButtonsFrames[(size_t)Race]);
		_textureLibrary->animationFrames("Options", _menuButtonsFrames[(size_t)Options]);
		_textureLibrary->animationFrames("Tips", _menuButtonsFrames[(size_t)Tips]);
		//_textureLibrary->animationFrames("Resume", _menuButtonsFrames[(size_t)Resume]);
	}
	
	void GameLibrary::loadOptionsButtons()
	{
		_optionsButtonsFrames.resize(9);
		_textureLibrary->animationFrames("On", _optionsButtonsFrames[(size_t)E_OPTION_ON]);
		_textureLibrary->animationFrames("Off", _optionsButtonsFrames[(size_t)E_OPTION_OFF]);
		_textureLibrary->animationFrames("DPadAlt", _optionsButtonsFrames[(size_t)E_OPTION_DPAD_ALT]);
		_textureLibrary->animationFrames("DPad0", _optionsButtonsFrames[(size_t)E_OPTION_DPAD]);
		_textureLibrary->animationFrames("Keyboard", _optionsButtonsFrames[(size_t)E_OPTION_KEYBOARD]);
		_textureLibrary->animationFrames("Trackball", _optionsButtonsFrames[(size_t)E_OPTION_TRACKBALL]);
		_textureLibrary->animationFrames("Tilt", _optionsButtonsFrames[(size_t)E_OPTION_TILT]);
		_textureLibrary->animationFrames("Audio", _optionsButtonsFrames[(size_t)E_OPTION_AUDIO]);
		_textureLibrary->animationFrames("Controls", _optionsButtonsFrames[(size_t)E_OPTION_CONTROLS]);
	}
	
	void GameLibrary::loadTrackSelectButtons()
	{
		_trackSelectButtonsFrames.resize(6); // TODO - Fix
		_textureLibrary->animationFrames("YankeeShot", _trackSelectButtonsFrames[(size_t)E_YANKEE_SHOT]);
		_textureLibrary->animationFrames("Everwhichaways", _trackSelectButtonsFrames[(size_t)E_EVERWHICHAWAYS]);
		_textureLibrary->animationFrames("Uhmurkin", _trackSelectButtonsFrames[(size_t)E_UHMURKIN]);
		_textureLibrary->animationFrames("DadgumTrakturs", _trackSelectButtonsFrames[(size_t)E_DADGUM_TRAKTURS]);
		_textureLibrary->animationFrames("RealBammer", _trackSelectButtonsFrames[(size_t)E_REAL_BAMMER]);
		_textureLibrary->animationFrames("Wijadidja", _trackSelectButtonsFrames[(size_t)E_WIJADIDJA]);
	}

	void GameLibrary::load()
	{
		LOGD("GameLibrary::load");
		// Loading real bounding rectangles.
		_textureLibrary->loadRealBounds("realrect.bound");

		_buttonsFrames.resize(7);
		_textureLibrary->animationFrames("Brake", _buttonsFrames[(size_t)Brake]);
		_textureLibrary->animationFrames("Shotgun", _buttonsFrames[(size_t)Shot]);
		_textureLibrary->animationFrames("BackArrow", _buttonsFrames[(size_t)Menu]);
		_textureLibrary->animationFrames("BackToMenuText", _buttonsFrames[(size_t)BackToMenu]);
		_textureLibrary->animationFrames("RateIt", _buttonsFrames[(size_t)RateIt]);
		_textureLibrary->animationFrames("Later", _buttonsFrames[(size_t)Later]);
		_textureLibrary->animationFrames("Never", _buttonsFrames[(size_t)Never]);

		//LOGD("Loading Truck Effect Animations");
		
		_effectsFrames.resize(3);
		_textureLibrary->animationFrames("Blast", _effectsFrames[(size_t)ShotgunBlast]);
		_textureLibrary->animationFrames("Sparks", _effectsFrames[(size_t)Sparks]);
		_textureLibrary->animationFrames("FireAndSmoke", _effectsFrames[(size_t)FireAndSmoke]);

		//LOGD("Loading Truck Exhaust Animations");

		_exhaustStraightFrameNames = _textureLibrary->listQuads("ExhaustFlamesDriveNorth");
        
        const size_t obstacleCount = 7;
		_obstaclesFrames.resize(obstacleCount);
		_textureLibrary->animationFrames("Puddle", _obstaclesFrames[(size_t)Puddle]);
		_textureLibrary->animationFrames("Pothole", _obstaclesFrames[(size_t)Pothole]);
		_textureLibrary->animationFrames("FatGuy", _obstaclesFrames[(size_t)FatGuy]);
		_textureLibrary->animationFrames("Outhouse", _obstaclesFrames[(size_t)Outhouse]);
		_textureLibrary->animationFrames("Shrub", _obstaclesFrames[(size_t)Shrub]);
		_textureLibrary->animationFrames("Tree", _obstaclesFrames[(size_t)Tree]);
		_textureLibrary->animationFrames("Tractor", _obstaclesFrames[(size_t)Tractor]);
        
        _obstaclesSounds.resize(obstacleCount);
        _obstaclesSounds[(size_t)FatGuy] = new Sound(Resources::loadResourceFromAssets("sounds/fat_guy_splash.wav"), Sound::E_PLAY_ONCE);
        _obstaclesSounds[(size_t)Puddle] = new Sound(Resources::loadResourceFromAssets("sounds/puddle_splash.wav"), Sound::E_PLAY_ONCE);
        _obstaclesSounds[(size_t)Outhouse] = new Sound(Resources::loadResourceFromAssets("sounds/outhouse_smash.wav"), Sound::E_PLAY_ONCE);
        _obstaclesSounds[(size_t)Shrub] = new Sound(Resources::loadResourceFromAssets("sounds/hit_bush.wav"), Sound::E_PLAY_ONCE);
        _obstaclesSounds[(size_t)Tree] = new Sound(Resources::loadResourceFromAssets("sounds/hit_tree.wav"), Sound::E_PLAY_ONCE);
        _obstaclesSounds[(size_t)Tractor] = new Sound(Resources::loadResourceFromAssets("sounds/tractor_crash.wav"), Sound::E_PLAY_ONCE);

		_animalFrameNames.resize(5);
		_animalFrameNames[(size_t)Armadillo] = _textureLibrary->listQuads("ArmadilloAlive");
		_animalFrameNames[(size_t)Snake] = _textureLibrary->listQuads("SnakeAlive");
		_animalFrameNames[(size_t)Possum] = _textureLibrary->listQuads("PossumAlive");
		_animalFrameNames[(size_t)Raccoon] = _textureLibrary->listQuads("RaccoonAlive");
		_animalFrameNames[(size_t)Squirrel] = _textureLibrary->listQuads("SquirrelAlive");
        _animalSound = new Sound(Resources::loadResourceFromAssets("sounds/roadkill_hit.wav"), Sound::E_PLAY_ONCE);

        _backgroundMusic = new Sound(Resources::loadResourceFromAssets("sounds/Edits_Intro4.wav"), Sound::E_LOOP);
        _shotgunSound = new Sound(Resources::loadResourceFromAssets("sounds/Shotgun.wav"), Sound::E_PLAY_ONCE);
        _gunPingSound = new Sound(Resources::loadResourceFromAssets("sounds/GunPing.wav"), Sound::E_PLAY_ONCE);
        _copsComingSound = new Sound(Resources::loadResourceFromAssets("sounds/cops_coming.wav"), Sound::E_PLAY_ONCE);
        _racersCrashSound = new Sound(Resources::loadResourceFromAssets("sounds/racers_crash.wav"), Sound::E_PLAY_ONCE);

        for (int i = 1; i <= 6; i++)
        {
            _rageSounds.push_back(new Sound(Resources::loadResourceFromAssets(Format("sounds/rage%1.wav", i).c_str()), Sound::E_PLAY_ONCE));
        }

        _truckStartupSound = new Sound(Resources::loadResourceFromAssets("sounds/truck_startup.wav"), Sound::E_PLAY_ONCE);
        _truckRevFadeSound = new Sound(Resources::loadResourceFromAssets("sounds/truck_rev_fade.wav"), Sound::E_PLAY_ONCE);
        
		LOGD("GameLibrary::load finished");

	}

    SoundPtr GameLibrary::backgroundMusic() const
    {
        return _backgroundMusic;
    }
    
    SoundPtr GameLibrary::shotgunSound() const
    {
        return _shotgunSound;
    }
    
    SoundPtr GameLibrary::gunPingSound() const
    {
        return _gunPingSound;
    }

    SoundPtr GameLibrary::truckStartupSound() const
    {
        return _truckStartupSound;
    }
    
    SoundPtr GameLibrary::truckRevFadeSound() const
    {
        return _truckRevFadeSound;
    }
    
    SoundPtr GameLibrary::copsComingSound() const
    {
        return _copsComingSound;
    }
}
