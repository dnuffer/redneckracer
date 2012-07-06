// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "Obstacle.hpp"
#include "Globals.hpp"

#include "engine/Log.hpp"

namespace rr
{

    void Obstacle::playHitSound() const
    {
        if (_hitSound)
            _hitSound->play();
    }
	
	void Obstacle::stopSounds() const
	{
		if (_hitSound) {
			_hitSound->stop();
		}
	}
}