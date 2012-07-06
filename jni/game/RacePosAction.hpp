// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __RACEPOSACTION_HPP__
#define __RACEPOSACTION_HPP__

#include "RRConfig.hpp"
#include "engine/EngineConfig.hpp"
#include "engine/Action.hpp"

namespace rr
{
	// Used to show player truck race position on label.
	class RacePosAction: public Action
	{
	public:
		RacePosAction(const blocxx::String& prefix, const blocxx::String& postfix = "")
				: prefix(prefix)
				, postfix(postfix)
				, racePosition(1)
			{}

	virtual void apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	void updateRacePosition(float playerPosY, float opponentPosY);
	void resetRacePosition();
	Int32 getRacePosition() const { return racePosition; }
	void setPostfix(const blocxx::String& post)
	{
		postfix = post;
	}

	private:
		blocxx::String prefix;
		blocxx::String postfix;
		Int32 racePosition;
	};

} // namespace rr

#endif // __RACEPOSACTION_HPP__
