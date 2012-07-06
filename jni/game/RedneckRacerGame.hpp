// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef REDNECK_RACER_GAME_HPP
#define REDNECK_RACER_GAME_HPP

#include "RRConfig.hpp"
#include "GameLibrary.hpp"
#include "engine/Director.hpp"
#include "engine/TextureLibrary.hpp"
#include "engine/AndroidKeyboardInput.hpp"
#include "boost/noncopyable.hpp"
#include "engine/Resources.hpp"
#include "engine/SoundDevice.hpp"

#include "RaceTracks.hpp"

#define APP_SETTINGS_FILE_NAME "redneckracer.set"
#define CONTROL_SETTINGS_FILE_NAME "ControlSettings"
#define BEST_TIMES_FILE_NAME "BestTimes"

namespace rr
{
class RedneckRacerGame : private boost::noncopyable, public AndroidKeyboardInput
{
public:
	RedneckRacerGame();
	~RedneckRacerGame();
	void init(const char* apkPath);
	void resize(int width, int height);
	void pause();
	void resume();
	void handleTouchEvent(long downTime, long eventTime, int action, float x, float y, float pressure, float size,
		int metaState, float xPrecision, float yPrecision, int deviceId, int edgeFlags);
	void handleTrackballEvent(float deltaX);
	void handleOrientationChanged(float rollAngle, float pitchAngle, float headingAngle);
	void renderNextFrame();
	void renderNextFrameWithLoading();

	void activateLoadingScreenScene();
	void startRace(RaceTracks::Races selectedTrack);
	void activateRaceScene();
	void activateGarageScene();
	void activateTrophyRoomScene();
	void activateOptionsScene();
	void activateMainMenuScene();
	void activateTrackSelectScene();
	void setCameraPosition(engine::Point pos);
	void showLoadingScreen();
	float getScreenWidth() const;
	float getScreenHeight() const;

	const TextureLibraryPtr& textureLibrary() { return textureLibrary_; }
	GameLibrary& library() { return _gameLibrary; }
	Director& director() { return m_director; }

	bool runningOnEmulator() const;

	std::string getSettingsFile() const;
	std::string getControlsFile() const;
	void setSettingsFilePath(const char* filePath);

	// Stuff for feedback (redirecting to android market):
	void setMarketFeedbackFunction(std::tr1::function<void (void)> f);
	void activateMarketFeedback();

	ScenePtr raceScene;
	ScenePtr garageScene;
	ScenePtr trophyRoomScene;
	ScenePtr optionsScene;
	ScenePtr trackSelectScene;
	MainMenuScenePtr mainMenuScene;
	LoadingScreenScenePtr loadingScreen;
	
	enum Scenes
	{
		_Default = -1,
		_LoadingScene = 0,
		_MainMenuScene,
		_RaceScene,
		_GarageScene,
		_OptionsScene,
		_TrackSelectScene
	};
	Scenes activeScene;
	Scenes prevScene;
	int loadingSceneCounter;

	enum Control_Types
	{
		TILT = 0,
		D_PAD,
		D_PAD_ALT,
		TRACKBALL,
		KEYBOARD
	};
	const String controlsFile;
	Control_Types controlType;
	bool inverted;
	unsigned left;
	unsigned right;
	bool selectLeftKey;
	bool selectRightKey;
	bool music;
	bool sfx;

	float currentLoadingProgress;

private:

    SoundDevice soundDevice;
	bool onKeyDownCallback(unsigned key, unsigned modifiers);
	bool onKeyUpCallback(unsigned key, unsigned modifiersAtPress, unsigned modifiersAtRelease);

	Director m_director;
	TextureLibraryPtr textureLibrary_;
	GameLibrary _gameLibrary;

	float heightScaleFactor;
	float widthScaleFactor;
	float deviceScreenWidth;
	float deviceScreenHeight;

	// orientation sensor state
	float rollAngle_;
	float pitchAngle_;
	float headingAngle_;
	float isLoadingStarted;

	//keyboardState
	float keyRollAngle_;
	float orientationRollAngle_;

	String applicationDataFilesDir;
	const String settingsFile;

	// Stuff for feedback (redirecting to android market):
	std::tr1::function<void ()> m_marketFeedbackFunc;

	SoundPtr backgroundMusic;
	
	BestTimesPtr m_bestTimes;
};
}

#endif
