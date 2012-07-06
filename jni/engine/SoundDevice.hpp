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

#ifndef SOUND_DEVICE_HPP_INCLUDED
#define SOUND_DEVICE_HPP_INCLUDED

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include "openal/include/AL/al.h"
#include "openal/include/AL/alc.h"
#include "openal/include/AL/alext.h"
#endif
#include "miniblocxx/vector.hpp"
#include "miniblocxx/String.hpp"
#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include <map>
#include <string>



namespace engine
{
class SoundDevice
{
public:
    SoundDevice();
    ~SoundDevice();

private:
	ALCdevice* device;
	ALCcontext* context;
};
}
#endif
