// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_Obstacle_hpp_INCLUDED_
#define game_Obstacle_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "engine/Sprite.hpp"
#include "engine/Animation.hpp"
#include "Destroyer.hpp"
#include "engine/Sound.hpp"

#include <string>

namespace rr
{
	class Obstacle : public Sprite, public Destroyer
	{
	public:
		Obstacle(const TexturedQuadPtr& undisturbed, const AnimationPtr& hit, const SoundPtr& hitSound)
			: Sprite(undisturbed, "Obstacle")
			, Destroyer()
			, _undisturbed(undisturbed)
			, _hit(hit)
		    , _destroyed(false)
            , _hitSound(hitSound)
			, _isInvincible(false)
		{
			Destroyer::setDamage(10);
		}
		
		virtual bool shouldCheckForCollision() const { return true; }
		
		void switchToHitAnimation()
		{
			animation(_hit);
			if (!_isInvincible) {
				_destroyed = true;
			}
		}

		bool destroyed() const { return _destroyed; }
		
		void makeInvincible() { _isInvincible = true; }
        
        void playHitSound() const;
		void stopSounds() const;

	private:
		TexturedQuadPtr _undisturbed;
		AnimationPtr _hit;
		bool _destroyed;
        SoundPtr _hitSound;
		bool _isInvincible;
	};
}

#endif
