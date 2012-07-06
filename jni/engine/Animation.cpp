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
#include "Animation.hpp"
#include "boost/foreach.hpp"

#define foreach BOOST_FOREACH

namespace engine
{

void Animation::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
{
	elapsedTime_ += deltaTime;
	size_t frame = elapsedTime_.realSeconds()/(1.0/speed_);
	if (loops_ == E_LOOP)
		frame = frame % frames_.size();
	if (frame >= frames_.size())
	{
		finished_ = true;
		currentFrame_ = frames_.size() - 1;
	}
	else
	{
		currentFrame_ = frame;
	}
}

	bool Animation::flippedHorizontal()
	{
		return frames_[0]->flippedHorizontal();
	}
	
	void Animation::setFlippedHorizontal(bool x)
	{
		foreach (TexturedQuadPtr& t, frames_)
		{
			t->setFlippedHorizontal(x);
		}
	}
	
	bool Animation::flippedVertical()
	{
		return frames_[0]->flippedVertical();
	}
	
	void Animation::setFlippedVertical(bool x)
	{
		foreach (TexturedQuadPtr& t, frames_)
		{
			t->setFlippedVertical(x);
		}
	}
	
	void Animation::setScale(float scale)
	{
		foreach (TexturedQuadPtr& t, frames_)
		{
			t->scale(scale);
		}
	}
	void Animation::setScaleX(float scaleX)
	{
		foreach (TexturedQuadPtr& t, frames_)
		{
			t->scaleX(scaleX);
		}
	}
	void Animation::setScaleY(float scaleY)
	{
		foreach (TexturedQuadPtr& t, frames_)
		{
			t->scaleY(scaleY);
		}
	}
	
	void Animation::reset(ELoopsOption playType)
	{
		loops_ = playType;
		if(finished_) // Reset only if all animation was playing.
		{
			TimeDuration td;
			elapsedTime_ = td;
			finished_ = false;
		}
	}

	Point Animation::getCurrentRealBoundCenter() const
	{
		return frames_[currentFrame_]->getRealBoundCenter();
	}

}
