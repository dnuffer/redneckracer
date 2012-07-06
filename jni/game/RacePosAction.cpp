// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RacePosAction.hpp"
#include "miniblocxx/Format.hpp"
#include "engine/Label.hpp"

namespace rr
{
	void RacePosAction::apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		boost::intrusive_ptr<Label> label = boost::dynamic_pointer_cast<Label>(target);
		if (label)
		{
			label->setText(prefix + String(racePosition) + postfix);
		}
	}

	void RacePosAction::resetRacePosition()
	{
		racePosition = 1;
	}

	void RacePosAction::updateRacePosition(float playerPosY, float opponentPosY)
	{
		if (playerPosY < opponentPosY)
		{
			++racePosition;
		}
	}
}
