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
#include "MoveAction.hpp"
//#include "Log.hpp"

namespace engine
{

void MoveAction::apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	Point position = target->position();
	//	Point oldPosition = position;
	position.x() += m_xPixelsPerSecond * deltaTime.realSeconds();
	position.y() += m_yPixelsPerSecond * deltaTime.realSeconds();
	target->setPosition(position);
	//	LOGD("MoveAction moved %s from (%f,%f) to (%f,%f)",
	//		target->name().c_str(), oldPosition.x(), oldPosition.y(), position.x(), position.y());
}

}
