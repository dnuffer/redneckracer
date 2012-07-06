// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_PlayerTruck_hpp_INCLUDED_
#define game_PlayerTruck_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "Truck.hpp"
#include "Globals.hpp"

namespace rr
{
	
	class PlayerTruck : public Truck
	{
	public:
		PlayerTruck(float& rollAngle, 
					const TexturedQuadPtr& stopped,
					const AnimationPtr& accelerating,
					const AnimationPtr& drivingStraight,
					const AnimationPtr& turningLeft,
					const AnimationPtr& drivingLeft,
					const AnimationPtr& turningLeftToStraight,
					const AnimationPtr& turningRight,
					const AnimationPtr& drivingRight,
					const AnimationPtr& turningRightToStraight,
					const std::string& name,
                    const std::vector<SoundPtr>& rageSounds,
                    const SoundPtr& hitSound)
			: Truck(stopped, accelerating, drivingStraight, turningLeft, drivingLeft, turningLeftToStraight, turningRight, drivingRight, turningRightToStraight, name, hitSound) 
			, m_rollAngle(rollAngle)
            , _rageSounds(rageSounds)
		{
            // modify the player truck parameters (defense and armor)
            truckParams = rr::Truck::TruckParameters(0, 0, defaultTruckDefense*2, defaultTruckArmor*2, 0);
		}
		
		virtual bool inputLeft() const;
		virtual bool inputRight() const;
		virtual float getTurnAngle();
		virtual bool isRacer() const { return true; }
        virtual bool isPlayer() const { return true; }
		virtual void setRageActive( bool isRageActivated );
		
	private:
		float& m_rollAngle;
        std::vector<SoundPtr> _rageSounds;
	};
	
}

#endif
