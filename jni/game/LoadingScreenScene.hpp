// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef LOADING_SCREEN_HPP_INCLUDED
#define LOADING_SCREEN_HPP_INCLUDED

#include "engine/EngineConfig.hpp"
#include "engine/Drawable.hpp"
#include "engine/Sprite.hpp"
#include "engine/TexturedFont.hpp"
#include "engine/Label.hpp"
#include "miniblocxx/String.hpp"
#include "engine/TexturedQuad.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Scene.hpp"
#include "engine/ProgressBar.hpp"
#include "Globals.hpp"
#include "RRFwd.hpp"
#include <vector>
#include "miniblocxx/TimeDuration.hpp"

namespace rr
{

	class LoadingScreenScene: public Scene
	{
		public:

		LoadingScreenScene(float maxProgressVal, const TexturedFontPtr& font,const Point& textPos, const String& text);
		ProgressBarPtr& getProgressBar();
		virtual void draw(const Rectangle& screen);
		void setText(const String& text);
		void setTextPosition(const Point& textPos);
		void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
		
		
		private:
		

		void addCreditsLine(String str);

		const TexturedFontPtr font;
		Label loadingText;
		ProgressBarPtr loadingProgress;
		vector<LabelPtr> credits;
		static const float creditsMoveSpeed = 50; // per second
		static const float creditsInterval = 25;
		float firstLinePosition;
		float maxProgressValue;
	};

}

#endif
