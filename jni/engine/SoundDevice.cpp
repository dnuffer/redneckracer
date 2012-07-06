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

#include "SoundDevice.hpp"
#include "Log.hpp"
#include "libzip/zip.h"
#include "Resources.hpp"
#include "Resource.hpp"

namespace engine
{

    SoundDevice::SoundDevice()
    {
        const ALint context_attribs[] = { ALC_FREQUENCY, 22050, 0 };
        device = alcOpenDevice(NULL);
        context = alcCreateContext(device, context_attribs);
        alcMakeContextCurrent(context);
    }

    SoundDevice::~SoundDevice()
    {
        // Don't do this because there currently is no mechanism to ensure that the Sound instances
        // are first destroyed. Also there is currently no need to cleanup, only one SoundDevice is
        // used for the duration of the process, and when the process exits, the OS does the cleanup.
        //alcMakeContextCurrent(NULL);
        //alcDestroyContext(context);
        //alcCloseDevice(device);
    }
}

