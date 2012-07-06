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

#include "Sound.hpp"
#include "Log.hpp"
#include "openal/include/AL/al.h"
#include "openal/include/AL/alc.h"
#include "openal/include/AL/alext.h"
#include <string.h>

namespace engine{

	typedef struct{
	 char  riff[4];//'RIFF'
		 unsigned int riffSize;
		 char  wave[4];//'WAVE'
		 char  fmt[4];//'fmt '
		 unsigned int fmtSize;
		 unsigned short format;
		 unsigned short channels;
		 unsigned int samplesPerSec;
		 unsigned int bytesPerSec;
		 unsigned short blockAlign;
		 unsigned short bitsPerSample;
		 unsigned short byteExtraData;
		 unsigned short extraData;
		 char fact[4]; //'fact'
		 unsigned int subChunk2Size;
		 unsigned int numOfSamples;
		 char  data[4];//'data'
		 unsigned int dataSize;
	  }BasicIMAWAVEHeader;


Sound::Sound(const ResourcePtr& resource, ELoopingOption loopingOption, float gainAdjustmentFactor) 
: _gainAdjustmentFactor(gainAdjustmentFactor)
{
	BasicIMAWAVEHeader header;
	const char* data = (const char*)&*resource->begin();
	memcpy(&header, data, sizeof(header));
	LOGE("samplseParSec %d", header.samplesPerSec);
	LOGE("blockSize %d", header.blockAlign);
	LOGE("samplesPerBlock %d", header.extraData);
	if (strcmp(header.riff, "RIFF"))
	{
		ALuint format = 0;
		switch (header.bitsPerSample)
		{
			case 4:
				format = AL_FORMAT_MONO_IMA4;
				break;
			default:
				LOGE("Unknown wav format");
				abort();
				break;
		}
		if(format != 0)
		{
			alGenSources(1, &source);
            alErrorCheck();
			alGenBuffers(1, &buffer);
            alErrorCheck();
			alBufferData(buffer,format,data+sizeof(header),header.dataSize,header.samplesPerSec);
            alErrorCheck();
			alSourcei(source, AL_BUFFER, buffer);
            alErrorCheck();

			if (loopingOption == E_LOOP)
				alSourcei(source, AL_LOOPING, AL_TRUE);
		}
	}
    else
    {
        LOGE("Couldn't play sound, header.riff is %s",header.riff);
    }
}

Sound::~Sound()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}

void Sound::play()
{
    ALint state = 0;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
        alSourcePlay(source);
    alErrorCheck();
}
	
void Sound::stop()
{
	alSourceStop(source);
	alErrorCheck();
}

void Sound::setGain(float gain)
{
	alSourcef(source, AL_GAIN, gain*_gainAdjustmentFactor);
    alErrorCheck();
}
    
void Sound::alErrorCheck()
{
    ALenum problem;
    if((problem = alGetError()) != AL_NO_ERROR)
        LOGE("openAL detected an error: %d",problem);
}

}

