// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "FPSAction.hpp"
#include "miniblocxx/Format.hpp"
#include "engine/Label.hpp"

namespace rr
{
	using namespace engine;

	void FPSAction::apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		++m_framesPassed;
		m_timeElapsed += deltaTime;

		TimeDuration desiredUpdatePeriod = Time::microseconds(250000);

		if( (m_timeElapsed > desiredUpdatePeriod) && (m_framesPassed > 3) )
		{
			boost::intrusive_ptr<Label> label = boost::dynamic_pointer_cast<Label>(target);
			if( label )
			{
				float fps = m_framesPassed / m_timeElapsed.realSeconds();
				label->setText( m_prefix + String(fps).substring(0,4) );
			}
			m_timeElapsed = TimeDuration(fmod(m_timeElapsed.realSeconds(), desiredUpdatePeriod.realSeconds()));
			m_framesPassed = 0;
		}
	}
}
