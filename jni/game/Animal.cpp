// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "Animal.hpp"
#include "engine/MoveAction.hpp"

namespace rr
{

	namespace
	{
		// animal speed in coords per second
		const int SpeedCPS = 70;
	}
	void Animal::startMoving()
	{
		if (_moving || _isDead) return;
		
		_moving = true;
		setAction(new MoveAction((_direction == E_RIGHT ? 1 : -1) * SpeedCPS, 0));
	}
	
	void Animal::stopMoving()
	{
		if (!_moving) return;
		
		_moving = false;
		clearActions();
	}
	
	void Animal::switchToDead()
	{
		stopMoving();
		texturedQuad(_dead);
		_isDead = true;
	}
    
    void Animal::playHitSound() const
    {
        if (_hitSound)
            _hitSound->play();
    }
	
	void Animal::stopSounds() const
	{
		if (_hitSound) {
			_hitSound->stop();
		}
	}
}
