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

#ifndef engine_Sound_HPP_INCLUDED
#define engine_Sound_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "Resource.hpp"
#ifdef __APPLE__
#include <OpenAL/al.h>
#else
#include "openal/include/AL/al.h"
#endif


namespace engine
{
    class Sound : public IntrusiveCountableBase
    {
    public:
        enum ELoopingOption
        {
            E_LOOP,
            E_PLAY_ONCE
        };

        Sound(const ResourcePtr& data, ELoopingOption loopingOption, float gainAdjustmentFactor = 1.0);
        ~Sound();
    	void play();
		void stop();
        void setGain(float gain);

    private:

    	ALuint source;
    	ALuint buffer;
    	
    	float _gainAdjustmentFactor;
        
        void alErrorCheck();
    };
}
#endif
