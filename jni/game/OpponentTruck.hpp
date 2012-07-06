// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_OpponentTruck_hpp_INCLUDED_
#define game_OpponentTruck_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "Truck.hpp"

namespace rr
{

	class OpponentTruck : public Truck
	{
	public:
		OpponentTruck(const TexturedQuadPtr& stopped,
					  const AnimationPtr& accelerating,
					  const AnimationPtr& drivingStraight,
					  const AnimationPtr& turningLeft,
					  const AnimationPtr& drivingLeft,
					  const AnimationPtr& turningLeftToStraight,
					  const AnimationPtr& turningRight,
					  const AnimationPtr& drivingRight,
					  const AnimationPtr& turningRightToStraight,
					  const std::string& name,
                      const SoundPtr& hitSound)
			: Truck(stopped, accelerating, drivingStraight, turningLeft, drivingLeft, turningLeftToStraight, turningRight, drivingRight, turningRightToStraight, name, hitSound)
		{}
		virtual float getTurnAngle();
        virtual bool isRacer() const { return true; }
	};
	

	class PoliceTruck : public OpponentTruck
	{
	public:
		PoliceTruck(const TexturedQuadPtr& stopped,
					const AnimationPtr& accelerating,
					const AnimationPtr& drivingStraight,
					const AnimationPtr& turningLeft,
					const AnimationPtr& drivingLeft,
					const AnimationPtr& turningLeftToStraight,
					const AnimationPtr& turningRight,
					const AnimationPtr& drivingRight,
					const AnimationPtr& turningRightToStraight,
					const std::string& name,
                    const SoundPtr& hitSound)
			: OpponentTruck(stopped, accelerating, drivingStraight, turningLeft, drivingLeft, turningLeftToStraight, turningRight, drivingRight, turningRightToStraight, name, hitSound)
		{
            // modify the police truck parameters (armor and defense)
            //truckParams = rr::Truck::TruckParameters(0, 0, defaultTruckDefense*2, defaultTruckArmor*2, 0);
			Destroyer::setDamage(0.0f);
		}
        virtual bool isRacer() const { return false; }
		virtual void flipHorizontal(bool flip);
	};

}

#endif
