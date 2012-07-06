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

#ifndef _engine_TOUCHHANDLER_HPP_
#define _engine_TOUCHHANDLER_HPP_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "boost/signals2.hpp"
#include <tr1/functional>


namespace engine
{
	class TouchHandler: public virtual IntrusiveCountableBase
	{
	public:		
		typedef boost::signals2::signal<void (const TouchHandler&)> signal_t;
		typedef std::tr1::function<void (const TouchHandler&)> action_t;

		enum TouchState
		{
			Untouched,
			Touched
		};
		
		TouchHandler() : touchState(Untouched)
		{}
		
		virtual bool checkHit(float screenX, float screenY) = 0;
		
		// Note: setState automatically call callback function, associated with that button state.
		void setState(TouchState state)
		{
			touchState = state;
			switch (state) {
				case Touched:
					touchedEvent(*this);
					break;
				case Untouched:
					unTouchedEvent(*this);
					break;
				default:
					break;
			}
		}
		
		TouchState getState() const
		{
			return touchState;
		}
		
		signal_t& getTouchedEvent() { return touchedEvent; }
		signal_t& getUnTouchedEvent() { return unTouchedEvent; } 
		
	private:
		signal_t touchedEvent;
		signal_t unTouchedEvent;
		TouchState touchState;
	};
}

#endif // _engine_TOUCHHANDLER_HPP_