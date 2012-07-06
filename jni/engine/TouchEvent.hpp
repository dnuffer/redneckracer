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

#ifndef TouchEvent_HPP_INCLUDED
#define TouchEvent_HPP_INCLUDED

#include "EngineConfig.hpp"

namespace engine
{
	enum TouchAction
	{
		TouchDown, TouchUp, TouchMove
	};

	struct TouchEvent
	{
		TouchEvent(long downTime, long eventTime, int action, float x, float y, float pressure,
				float size, int metaState, float xPrecision, float yPrecision, int deviceId, int edgeFlags)
			: downTime(downTime)
			, eventTime(eventTime)
			, action(action)
			, x(x)
			, y(y)
			, pressure(pressure)
			, size(size)
			, metaState(metaState)
			, xPrecision(xPrecision)
			, yPrecision(yPrecision)
			, deviceId(deviceId)
			, edgeFlags(edgeFlags)
		{}

		long downTime;
		long eventTime;
		int action;
		float x;
		float y;
		float pressure;
		float size;
		int metaState;
		float xPrecision;
		float yPrecision;
		int deviceId;
		int edgeFlags;
	};
}

#endif
