// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_Animal_hpp_INCLUDED_
#define game_Animal_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "engine/Sprite.hpp"
#include "engine/Animation.hpp"
#include "Destroyer.hpp"
#include "engine/Sound.hpp"

namespace rr
{
	class Animal : public Sprite, public Destroyer
	{
	public:
		enum EMovementDirection
		{
			E_LEFT,
			E_RIGHT
		};
		
		Animal(const AnimationPtr& alive, const TexturedQuadPtr& dead, EMovementDirection direction, 
			   const std::string& name, const SoundPtr& hitSound)
			: Sprite(alive, name)
			, Destroyer()
			, _alive(alive)
			, _dead(dead)
			, _direction(direction)
			, _moving(false)
			, _isDead(false)
            , _hitSound(hitSound)
		{
			if (direction == E_LEFT)
				Sprite::setFlippedHorizontal(true);

			Destroyer::setDamage(5.0f);
		}
		
		bool moving() const { return _moving; }
		void startMoving();
		void stopMoving();
		void switchToDead();
		
		virtual bool shouldCheckForCollision() const { return true; }
        
        void playHitSound() const;
		void stopSounds() const;
		
	private:
		AnimationPtr _alive;
		TexturedQuadPtr _dead;
		EMovementDirection _direction;
		bool _moving;
		bool _isDead;
        SoundPtr _hitSound;
	};
}

#endif
