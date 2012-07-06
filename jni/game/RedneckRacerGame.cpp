// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "RedneckRacerGame.hpp"
#include "engine/Scene.hpp"
#include "MainMenuScene.hpp"
#include "RaceScene.hpp"
#include "GarageScene.hpp"
#include "TrophyRoomScene.hpp"
#include "OptionsScene.hpp"
#include "TrackSelectScene.hpp"
#include "RaceTracks.hpp"
#include "engine/TextureLoader.hpp"
#include "engine/TouchEvent.hpp"
#include "engine/Log.hpp"
#include "Globals.hpp"
#include "LoadingScreenScene.hpp"
#include "engine/TexturedFont.hpp"
#include "engine/ProgressBar.hpp"
#include "engine/Sound.hpp"
#include "BestTimes.hpp"

namespace rr
{

using namespace boost;

unsigned int resourcesForLoadingCount = 70;
void imageLoadingProgress(float f)
{
	++(game().currentLoadingProgress);
	// If add new resources, uncomment to find new resources count
	/*++resourcesForLoadingCount;
	LOGE("Resources for Loading Count = %i", resourcesForLoadingCount);*/
}

void progress(float f)
{

}

std::string RedneckRacerGame::getSettingsFile() const
{
	return std::string(applicationDataFilesDir) + '/' + settingsFile;
}

std::string RedneckRacerGame::getControlsFile() const
{
        return std::string(applicationDataFilesDir) + '/' + controlsFile;
}


void RedneckRacerGame::setSettingsFilePath(const char* filePath)
{
	applicationDataFilesDir = String(filePath);
	m_bestTimes.reset(new BestTimes(applicationDataFilesDir + '/' + BEST_TIMES_FILE_NAME));
}



RedneckRacerGame::RedneckRacerGame()
	: loadingSceneCounter(0)
	, selectLeftKey(false)
	, selectRightKey(false)
	, currentLoadingProgress(0)
	, textureLibrary_(new TextureLibrary)
	, _gameLibrary(textureLibrary_)
	, rollAngle_(0)
	, pitchAngle_(0)
	, headingAngle_(0)
	, isLoadingStarted(false)
	, keyRollAngle_(0)
	, orientationRollAngle_(0)
	, settingsFile(APP_SETTINGS_FILE_NAME)
{
	_gameLibrary.setProgressFunction(progress);
}

RedneckRacerGame::~RedneckRacerGame() {}

void RedneckRacerGame::init(const char* apkPath)
{
	LOGD("GLVendor=%s", gl::getVendor().c_str());
	LOGD("GLVersion=%s", gl::getVersion().c_str());
	LOGD("GLRenderer=%s", gl::getRenderer().c_str());
	LOGD("GLExtensions=%s", gl::getExtensions().c_str());
	m_director.init(apkPath);

	KeyboardInput::disableRepeat();
	// These can be used to handle single events for up/down.  When repeat is
	// disabled (above) the down will only be called once, no matter how many
	// times the emulator or keyboard sends down a repeat press.
	KeyboardInput::key_pressed.connect(memfun(this, &RedneckRacerGame::onKeyDownCallback));
	KeyboardInput::key_released.connect(memfun(this, &RedneckRacerGame::onKeyUpCallback));
	activeScene =  _LoadingScene;
	prevScene =  _Default;
}

void RedneckRacerGame::resize(int width, int height)
{
	deviceScreenWidth = (float)width;
	deviceScreenHeight = (float)height;
	heightScaleFactor = (float)DefaultScaleHeight/deviceScreenHeight;
	widthScaleFactor = (float)DefaultScaleWidth/deviceScreenWidth;

	m_director.setSize(width, height, DefaultScaleWidth, DefaultScaleHeight);

	showLoadingScreen();
}
void RedneckRacerGame::showLoadingScreen()
{
	textureLibrary_->loadAtlasData("loading1.atlas");
	textureLibrary_->loadAtlasTexture("loading1.atlas");
	textureLibrary_->loadAtlasData("fonts/TheMilkmanConspiracy.atlas");
	textureLibrary_->loadAtlasTexture("fonts/TheMilkmanConspiracy.atlas");

	loadingScreen = new LoadingScreenScene(resourcesForLoadingCount, new TexturedFont(textureLibrary_, "TheMilkmanConspiracy"),
						     	      Point(0,300),
						     	      "LOADING");

	currentLoadingProgress = 0;
	loadingScreen->getProgressBar()->connectVariable(currentLoadingProgress);
	m_director.runScene(loadingScreen);
	loadingSceneCounter = 1;
	renderNextFrameWithLoading();
}

void RedneckRacerGame::setCameraPosition(Point pos)
{
	m_director.setCameraPosition(pos);
}

void RedneckRacerGame::pause()
{

}
	
void RedneckRacerGame::resume()
{
}

bool loadingIsOver = false;

void* loadRaceResources(void*)
{
	loadingIsOver = false;
	game().textureLibrary()->loadAllAtlases(imageLoadingProgress);
	game().textureLibrary()->preloadTexImages(imageLoadingProgress);
	game().library().load();
	loadingIsOver = true;
	LOGI("Resources loading finished!");
	return NULL;
}

void RedneckRacerGame::handleTouchEvent(long downTime, long eventTime, int action, float x, float y, float pressure,
		float size, int metaState, float xPrecision, float yPrecision, int deviceId, int edgeFlags)
{
	// Translate android coordinates ( x = 0, y = 0 - top left corner) to
	// director screen coordinates(x=0, y=0 - screen center).
	x -= deviceScreenWidth / 2;
	y = (deviceScreenHeight / 2) - y;
	x *= widthScaleFactor;
	y *= heightScaleFactor;

	m_director.handleTouchEvent(TouchEvent(downTime, eventTime, action, x, y, pressure, size, metaState, xPrecision,
			yPrecision, deviceId, edgeFlags));
}

void RedneckRacerGame::handleTrackballEvent(float deltaX)
{
        if((activeScene == _RaceScene) && (controlType == TRACKBALL))
        {
                rollAngle_ = deltaX * 8;//Conversion to have numbers similar to the keypad.
        }
}

void RedneckRacerGame::handleOrientationChanged(float rollAngle, float pitchAngle, float headingAngle)
{
	//LOGD("handleOrientationChanged(rollAngle: %f, pitchAngle: %f, headingAngle: %f", rollAngle, pitchAngle, headingAngle);
	if(controlType == TILT)
	{
		rollAngle_ = orientationRollAngle_ = rollAngle;
		pitchAngle_ = pitchAngle;
		headingAngle_ = headingAngle;
	}
}

void RedneckRacerGame::renderNextFrameWithLoading()
{
	pthread_t loadingThread = 0;

	switch(activeScene)
	{
		case _LoadingScene:
			if (activeScene != prevScene)
			{
				if (loadingSceneCounter > 0)
				{
					loadingSceneCounter--;
				}
				else
				{
					if (game().raceScene == NULL && !isLoadingStarted)
					{
						m_director.runScene(loadingScreen);
						isLoadingStarted = true;
						pthread_create(&loadingThread, NULL, loadRaceResources, NULL);
					}
					else if (loadingIsOver)
					{
						LOGI("Start creating race scene.");
						game().raceScene = new RaceScene(game()._gameLibrary, game().rollAngle_, m_bestTimes);
						LOGI("Creating race scene end!");
						prevScene = activeScene;
						activeScene = _MainMenuScene;
					}
				}
			}

			m_director.renderNextFrame();
			break;

		case _MainMenuScene:
			if (activeScene != prevScene)
			{
				//reset camera
				setCameraPosition(Point(0.0,0.0));
				if (mainMenuScene == NULL)
				{
					mainMenuScene = new MainMenuScene(_gameLibrary);

                    if (!backgroundMusic)
                    {
                        backgroundMusic = _gameLibrary.backgroundMusic();
                    }
                    backgroundMusic->play();
				}
				activateMainMenuScene();
				prevScene = activeScene;
			}
			m_director.renderNextFrame();
			break;
			
		case _TrackSelectScene:
			if (activeScene != prevScene) {
				//reset camera
				setCameraPosition(Point(0.0,0.0));
				if (trackSelectScene == NULL) {
					trackSelectScene = new TrackSelectScene(_gameLibrary, m_bestTimes);
				}
				activateTrackSelectScene();
				prevScene = activeScene;
			}
			m_director.renderNextFrame();
			break;


		case _RaceScene:
			if (loadingIsOver && prevScene != _RaceScene)
			{
				boost::intrusive_ptr<MainMenuScene> mainMenu = dynamic_pointer_cast<MainMenuScene>(mainMenuScene);
				if (mainMenu)
				{
					if (!mainMenu->isRacePaused())
					{
						boost::intrusive_ptr<RaceScene> race = dynamic_pointer_cast<RaceScene>(raceScene);
						if (race)
						{
							race->restartRace();
						}
					}
				}
				game().activateRaceScene();
				prevScene = activeScene;
			}

			m_director.renderNextFrame();
			break;
		
		case _Default: break;
		case _GarageScene: break;
		case _OptionsScene: break;
	}
}
void RedneckRacerGame::renderNextFrame()
{
	renderNextFrameWithLoading();
}

void RedneckRacerGame::activateLoadingScreenScene()
{
	m_director.runScene(loadingScreen);
}
	
void RedneckRacerGame::startRace(RaceTracks::Races selectedTrack)
{
	dynamic_pointer_cast<RaceScene>(raceScene)->setRaceTrack(selectedTrack);
	activeScene = _RaceScene;
	_gameLibrary.setSoundGain(game().sfx);
	loadingSceneCounter = 1;
	renderNextFrameWithLoading();
	setCameraPosition(Point(0,0));
}

void RedneckRacerGame::activateRaceScene()
{
	m_director.runScene(raceScene);
}

void RedneckRacerGame::activateGarageScene()
{
	m_director.runScene(garageScene);
}
void RedneckRacerGame::activateTrophyRoomScene()
{
	m_director.runScene(trophyRoomScene);
}
void RedneckRacerGame::activateOptionsScene()
{
	m_director.runScene(optionsScene);
}

void RedneckRacerGame::activateMainMenuScene()
{
	m_director.runScene(mainMenuScene);
}
	
void RedneckRacerGame::activateTrackSelectScene()
{
	m_director.runScene(trackSelectScene);
}

bool RedneckRacerGame::runningOnEmulator() const
{
	std::string vendor = gl::getVendor();
	std::string renderer = gl::getRenderer();

	if( renderer.find("PixelFlinger") != std::string::npos )
	{
		return vendor == "Android";
	}
	return false;
}

bool RedneckRacerGame::onKeyDownCallback(unsigned key, unsigned modifiers)
{
	LOGD("KeyDown: State: %s", KeyboardInput::keyname(key, modifiers, false).c_str());
	
	if(controlType == KEYBOARD)
	{
		if( key == left )
		{
			rollAngle_ = -30;
		}
		if( key == right )
		{
			rollAngle_ = 30;

		}

		 //= keyRollAngle_;
	}
	if(controlType == D_PAD)
	{
		if( KeyboardInput::key_down(KeyboardInput::KEY_keypad_up) ||
			KeyboardInput::key_down(KeyboardInput::KEY_keypad_left) )
		{
			rollAngle_ = -30;
		}
		if( KeyboardInput::key_down(KeyboardInput::KEY_keypad_down) ||
			KeyboardInput::key_down(KeyboardInput::KEY_keypad_right) )
		{
			rollAngle_ = 30;
		}
		
		//rollAngle_ = keyRollAngle_;
	}
	
	if (key == KeyboardInput::KEY_escape)
    {
        switch (activeScene) 
        {
			case _RaceScene:
				game()._gameLibrary.setSoundGain(false);
				activeScene = _MainMenuScene;
				break;
				
			case _TrackSelectScene:
				activeScene = _MainMenuScene;
				break;

				
            case _LoadingScene:
                exit(0);
                break;
                
            case _MainMenuScene:
                mainMenuScene->handleBackInput();
                break;
                
            default:		
                activeScene = _MainMenuScene;
                break;
        }
    }

	LOGASSERT(!(selectLeftKey == true && selectRightKey == true),
		"Variables selectLeftKey and selectRightKey should never simultaneously be true.");
	
	if(selectLeftKey)
	{
		left = key;
		selectLeftKey = false;
		mainMenuScene->selectKeyScreenFinished();
	}
	else if(selectRightKey)
	{
		right = key;
		selectRightKey = false;
		mainMenuScene->selectKeyScreenFinished();
	}
	

        
    
	return true;
}
bool RedneckRacerGame::onKeyUpCallback(unsigned key, unsigned modifiersAtPress, unsigned modifiersAtRelease)
{
	LOGD("KeyUp: State: %s", KeyboardInput::keyname(key, modifiersAtPress, false).c_str());
	
	LOGD("Resetting pitch and roll angle to 0");
	
	keyRollAngle_ = 0;
	rollAngle_ = 0;
	pitchAngle_ = 0;
	
	return true;
}

float RedneckRacerGame::getScreenWidth() const
{
	return deviceScreenWidth * widthScaleFactor;
}

float RedneckRacerGame::getScreenHeight() const
{
	return deviceScreenHeight * heightScaleFactor;
}

void RedneckRacerGame::setMarketFeedbackFunction(std::tr1::function<void (void)> f)
{
	m_marketFeedbackFunc = f;
}

void RedneckRacerGame::activateMarketFeedback()
{
	m_marketFeedbackFunc();
}

}
