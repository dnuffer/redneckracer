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

#ifndef SCENE_DIRECTOR_HPP_INCLUDED
#define SCENE_DIRECTOR_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Scene.hpp"
#include "TouchEvent.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "Rectangle.hpp"
#include "miniblocxx/DateTime.hpp"

namespace engine
{
/**
 * You probably only want one instance of this class.
 */
class Director
{
public:
	void init(const char* apkPath);
	void runScene(ScenePtr scene);
	void setSize(int width, int height, int scaleWidth, int scaleHeight);
	void handleTouchEvent(const TouchEvent& touchEvent);
	
	const Point& cameraPosition() const					{ return m_cameraPosition; }
	void setCameraPosition(const Point& cameraPosition);


	// Update and display the next frame.
	void renderNextFrame();

	void updateNextFrame();
	void displayFrame();

private:
	ScenePtr _runningScene;
	DateTime lastFrameStartTime;
	Point m_cameraPosition;
	Size m_scaleSize;
	
private:
	void draw();
	Rectangle screen() const;

};

}

#endif
