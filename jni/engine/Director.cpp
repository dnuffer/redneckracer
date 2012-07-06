// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EngineConfig.hpp"
#include "Director.hpp"
#include "Log.hpp"
#include "GL.hpp"
#include "Resources.hpp"
#include "miniblocxx/DateTime.hpp"

#include "libzip/zip.h"

#include <stdlib.h>

namespace engine
{

	Rectangle Director::screen() const
	{
		return Rectangle::makeCenteredOn(m_cameraPosition, m_scaleSize);
	}

	void Director::init(const char* apkPath)
	{
		Resources::init(apkPath);
	}

	void Director::runScene(ScenePtr scene)
	{
		_runningScene = scene;
		_runningScene->handleActivated();
	}

	void Director::setSize(int width, int height, int scaleWidth, int scaleHeight)
	{
		m_scaleSize = Size(scaleWidth, scaleHeight);

		glViewport(0, 0, width, height);
		LOGD("setSize(w:%d,h:%d, sw:%d,sh:%d)", width, height, scaleWidth, scaleHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gl::ortho(-scaleWidth/2.0f, scaleWidth/2.0f, -scaleHeight/2.0f, scaleHeight/2.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	void Director::renderNextFrame()
	{
		updateNextFrame();
		displayFrame();
	}

	void Director::updateNextFrame()
	{
		DateTime thisFrameStartTime = DateTime::getCurrent();
		TimeDuration deltaTime = (lastFrameStartTime == DateTime()) ? TimeDuration() : thisFrameStartTime - lastFrameStartTime;
		lastFrameStartTime = thisFrameStartTime;

		//		LOGD("Director::renderNextFrame() deltaTime microseconds: %lld", (long long)deltaTime.microseconds());

		if( _runningScene )
		{
			//			LOGI("Updating next frame for the current running scene.");
			_runningScene->update(thisFrameStartTime, deltaTime);

			_runningScene->handleCollisions(thisFrameStartTime, deltaTime);
		}
		else
		{
			//			LOGE("No running scene in Director::renderNextFrame.");
		}
	}

	void Director::displayFrame()
	{
		draw();
	}

	void Director::draw()
	{
		using namespace gl;
		clearColorBuffer();
		clearDepthBuffer();
		loadIdentity();
		VertexArrayScope vas;
		ColorArrayScope cas;
		TextureCoordArrayScope tcas;

		if( _runningScene )
		{
			_runningScene->draw(screen());
		}
		else
		{
			LOGE("No running scene in Director::draw()");
		}
	}

	void Director::setCameraPosition(const Point& cameraPosition)
	{
		if (cameraPosition == m_cameraPosition) return;

		m_cameraPosition = cameraPosition;

		gl::matrixMode(GL_PROJECTION);
		gl::loadIdentity();
		Rectangle screen = this->screen();
		//LOGD("setting ortho to (%f, %f: %f, %f)",
		//	 screen.left, screen.right,
		//	 screen.bottom, screen.top);
		gl::ortho(screen.left, screen.right,
			screen.bottom, screen.top,
			-1.0f, 1.0f);

		gl::matrixMode(GL_MODELVIEW);
	}


	void Director::handleTouchEvent(const TouchEvent& touchEvent)
	{
		_runningScene->handleTouchEvent(touchEvent);
	}

}
