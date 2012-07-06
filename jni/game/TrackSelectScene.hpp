// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef _RACE_SELECT_SCENE_HPP_INCLUDED_
#define _RACE_SELECT_SCENE_HPP_INCLUDED_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "GameLibrary.hpp"
#include "engine/EngineFwd.hpp"
#include "engine/Scene.hpp"
#include "boost/signals2.hpp"
#include "RaceTracks.hpp"

namespace rr 
{
	class TrackSelectScene : public Scene {
	public:
		TrackSelectScene(GameLibrary& gameLibrary, const BestTimesPtr& bestTimes);
		virtual ~TrackSelectScene();
		
		virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
		virtual void handleTouchEvent(const TouchEvent& touchEvent);
		virtual void handleActivated();
	
		static const int backgroundZOrder = -1;
		static const int obstacleZOrder = 10;
		static const int animalZOrder = 20;
		static const int truckZOrder = 30;
		static const int sparksZOrder = 40;
		static const int hudZOrder = 100;
		
	private:
		// Callback function to interact with menu buttons.
		void backButtonActionUp(const TouchHandler&);
		void yankeeShotButtonActionUp(const TouchHandler&);
		void everwhichawaysButtonActionUp(const TouchHandler&);
		void uhmurkinButtonActionUp(const TouchHandler&);
		void dadgumTraktursButtonActionUp(const TouchHandler&);
		void realBammerButtonActionUp(const TouchHandler&);
		void wijadidjaButtonActionUp(const TouchHandler&);
		
		
		void loadButtons();
	
		void drawBackground();
		void prepareButton(TouchButtonPtr button);
		void addBestTimeLabel(RaceTracks::Races trackType, float yUpper);
		
	private:
		GameLibrary& gameLibrary;
		
		TouchButtonPtr yankeeShotButton;
		TouchButtonPtr everwhichawaysButton;
		TouchButtonPtr uhmurkinButton;
		TouchButtonPtr dadgumTraktursButton;
		TouchButtonPtr realBammerButton;
		TouchButtonPtr wijadidjaButton;
		TouchButtonPtr backButton;
		std::vector<TouchButtonPtr> touchButtons;
		
		DrawableRectanglePtr dimmedBackground;
		SpritePtr title;
		BestTimesPtr m_bestTimes;
	};
}

#endif
