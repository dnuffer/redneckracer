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

#ifndef ANIMATION_HPP_INCLUDED
#define ANIMATION_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Point.hpp"
#include "Size.hpp"
#include "TexturedQuad.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "boost/range/algorithm_ext/push_back.hpp"
#include "miniblocxx/vector.hpp"
#include "miniblocxx/TimeDuration.hpp"

namespace engine
{

class Animation : public virtual IntrusiveCountableBase
{	
public:
	enum ELoopsOption
	{
		E_LOOP,
		E_PLAY_ONCE
	};

	template <typename RangeT>
	explicit Animation(const RangeT& frames, ELoopsOption loops = E_PLAY_ONCE, float speed = 15.0)
		: currentFrame_(0)
		, speed_(speed)
		, loops_(loops)
		, finished_(false)
	{
		boost::range::push_back(frames_, frames);
		offsets_.resize(frames_.size());
	}

	template <typename FramesRangeT, typename OffsetsRangeT>
	explicit Animation(const FramesRangeT& frames, const OffsetsRangeT& offsets, ELoopsOption loops = E_PLAY_ONCE, float speed = 15.0)
		: currentFrame_(0)
			, speed_(speed)
			, loops_(loops)
			, finished_(false)
	{
		boost::range::push_back(frames_, frames);
		boost::range::push_back(offsets_, offsets);
	}

	Point frameOffset() const { return offsets_[currentFrame_]; }
	Size currentSize() const { return frames_[currentFrame_]->size(); }
	Size currentRealSize() const { return frames_[currentFrame_]->getRealSize(); }

	void draw(const Rectangle& screen) { frames_[currentFrame_]->draw(screen); }

	void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	
	bool flippedHorizontal();
	void setFlippedHorizontal(bool x);
	bool flippedVertical();
	void setFlippedVertical(bool x);
	
	void setScale(float scale);
	void setScaleX(float scaleX);
	void setScaleY(float scaleY);
	
	void reset(ELoopsOption playType = E_PLAY_ONCE);
	
#if DEBUG // For testing
	TimeDuration getElapsedTime() { return elapsedTime_; }
	size_t getCurrentFrame() { return currentFrame_; }
#endif

	Point getCurrentRealBoundCenter() const;

private:
	vector<TexturedQuadPtr> frames_;
	vector<Point> offsets_;
	size_t currentFrame_;
	float speed_;
	ELoopsOption loops_;
	bool finished_;
	TimeDuration elapsedTime_;
};

}

#endif
