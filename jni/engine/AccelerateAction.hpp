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

#ifndef engine_AccelerateAction_hpp_INCLUDED_
#define engine_AccelerateAction_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "Action.hpp"

namespace engine
{

// currently the class assumes acceleration from velocity=0
class AccelerateAction : public Action
{
public:
	AccelerateAction(double xPixelsPerSecondSquared, double yPixelsPerSecondSquared)
	: xPixelsPerSecondSquared(xPixelsPerSecondSquared), yPixelsPerSecondSquared(yPixelsPerSecondSquared)
	, maxSpd(0), useMaxSpeed(false)
	{}

	// maxSpeed limiter now working only for Y coordinate!
	AccelerateAction(double xPixelsPerSecondSquared, double yPixelsPerSecondSquared, double maxSpeed)
	: xPixelsPerSecondSquared(xPixelsPerSecondSquared), yPixelsPerSecondSquared(yPixelsPerSecondSquared)
	, maxSpd(maxSpeed), useMaxSpeed(true)
	{}


	virtual void apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);

private:
	double xPixelsPerSecondSquared;
	double yPixelsPerSecondSquared;
	TimeDuration elapsedTime;
	double maxSpd;
	double currentSpeed;
	bool useMaxSpeed;
};

}

#endif
