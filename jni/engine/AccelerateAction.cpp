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
#include "AccelerateAction.hpp"

namespace engine
{

void AccelerateAction::apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	// basic physics
	// v = a t
	// d = v t
	// d = 1/2 a t^2
	// delta_d = 1/2 a t^2 - 1/2 a (t-delta_t)^2    : 6 *, 2 -
	// alternate forms (thanks to wolfram alpha)
	// delta_d = -1/2 a delta_t (delta_t - 2 t)     : 4 *, 1 - (using this one)
	// delta_d = a delta_t t - (a delta_t^2)/2      : 4 *, 1 /, 1 -
	// delta_d = 1/2 a delta_t (2 t - delta_t)      : 4 *, 1 -

	elapsedTime += deltaTime;
	double delta_t = deltaTime.realSeconds();
	double t = elapsedTime.realSeconds();
	Point position = target->position();
	currentSpeed = t * yPixelsPerSecondSquared;
	if( (useMaxSpeed && (currentSpeed <= maxSpd)) || (!useMaxSpeed))
	{
		position.y() += -.5 * yPixelsPerSecondSquared * delta_t * (delta_t - 2 * t);
	}
	else
	{
		position.y() += maxSpd *delta_t;
	}

	position.x() += -.5 * xPixelsPerSecondSquared * delta_t * (delta_t - 2 * t);

	target->setPosition(position);
}

}
