// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "PlayerTruck.hpp"
#include "boost/random.hpp"

namespace rr
{

	namespace
	{
		const float TURN_THRESHOLD = 7.0;
        boost::rand48 randEng(::time(NULL));
	}
	
	bool PlayerTruck::inputLeft() const
	{
		return m_rollAngle < -TURN_THRESHOLD;
	}
	
	bool PlayerTruck::inputRight() const
	{
		return m_rollAngle > TURN_THRESHOLD;
	}
	
	float PlayerTruck::getTurnAngle()
	{
		return m_rollAngle;
	}

	void PlayerTruck::setRageActive( bool isRageActivated )
    {
        Truck::setRageActive(isRageActivated);
        if (isRageActivated)
        {
            boost::uniform_int<int> effect(0, _rageSounds.size() - 1);
            _rageSounds[effect(randEng)]->play();
        }
    }
}
