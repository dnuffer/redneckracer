// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "LoadingScreenScene.hpp"
#include "RRFwd.hpp"
#include "RRConfig.hpp"
#include "Globals.hpp"
#include "engine/Rectangle.hpp"
#include "miniblocxx/String.hpp"
#include "boost/foreach.hpp"
#include "engine/Log.hpp"
#define foreach BOOST_FOREACH

namespace rr
{

void LoadingScreenScene::setText(const String& text)
{
	loadingText.setText(text);
}

void LoadingScreenScene::setTextPosition(const Point& textPos)
{
	loadingText.setPosition(textPos);
}

void LoadingScreenScene::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	firstLinePosition += deltaTime.realSeconds() *creditsMoveSpeed;
	float yPos = firstLinePosition;
	
	foreach(LabelPtr label, credits)
	{
		label->setPosition(Point(-12, DefaultScreenBottom + yPos));
		yPos -= creditsInterval;
	}

	if (yPos > DefaultScreenTop )
	{
		yPos = DefaultScreenBottom;
		firstLinePosition = 70;
	}

	Scene::update(thisFrameStartTime, deltaTime);
}

void LoadingScreenScene::draw(const Rectangle& screen)
{
	Scene::draw(screen);
}

LoadingScreenScene::LoadingScreenScene(float maxProgressVal, const TexturedFontPtr& font,const Point& textPos, const String& text)
	: font(font)
	, loadingText(font)
	, firstLinePosition(70)
	, maxProgressValue(maxProgressVal)

	{
	
	//loading progress bar
	loadingProgress = new ProgressBar(maxProgressVal, DefaultScaleWidth, 20);
	loadingProgress->setColor(0.5, 0.5, 0.5, 1);
	loadingProgress->setBorderWidth(1.0);
	loadingProgress->setBorderColor(0, 0, 0, 1);
	loadingProgress->setPosition(Point(DefaultScreenLeft, DefaultScreenBottom));
	loadingProgress->setPositionInterpretation(Drawable::E_SCREEN);
	addChild(loadingProgress, 10);
	
	//Loading Progress Bar label
	loadingText.setPosition(Point(0,  DefaultScreenBottom + 10));
	loadingText.setText(text);
	addChild(&loadingText,30);
	
	//Loading Background
	SpritePtr woodenBackground = game().library().loadingSprite("LoadingScreenWooden");
	woodenBackground->setPosition(Point(0.0,0.0));
	addChild(woodenBackground, 3);
	woodenBackground->setPositionInterpretation(Drawable::E_SCREEN);
	
	//Loading scrolling Game Credits.
	addCreditsLine("dan nuffer");
	addCreditsLine("dave nuffer");
	addCreditsLine("sonya summers");
	addCreditsLine("kevin harris");
	addCreditsLine("garry garrett");
	addCreditsLine("alexander");
	addCreditsLine("feldman");
	addCreditsLine("cambell");
	addCreditsLine("christensen");
	addCreditsLine("matthew ricks");
	addCreditsLine("michael landes");

	LOGD("Loading Screen Finished Loading");
}


void LoadingScreenScene::addCreditsLine(String str)
{
	LabelPtr label = new Label(font);
	label->setPosition(Point(-12, DefaultScreenBottom - 100));
	label->setText(str);
	credits.push_back(label);
	addChild(label, 2);
}

ProgressBarPtr& LoadingScreenScene::getProgressBar()
{
	return loadingProgress;
}


}
