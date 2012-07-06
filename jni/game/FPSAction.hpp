// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef engine_FPSAction_hpp_INCLUDED_
#define engine_FPSAction_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "engine/EngineConfig.hpp"
#include "engine/Action.hpp"

namespace rr
{

class FPSAction : public Action
{
public:
	FPSAction(const blocxx::String& prefix)
		: m_prefix(prefix)
		, m_framesPassed(0)
		, m_timeElapsed()
	{}

	virtual void apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
private:
	blocxx::String m_prefix;
	int m_framesPassed;
	TimeDuration m_timeElapsed;
};

}

#endif
