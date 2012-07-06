// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef game_CivilCar_hpp_INCLUDED_
#define game_CivilCar_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "Truck.hpp"

namespace rr
{
	class CivilCar : public Truck
	{
	public:
		CivilCar(const TexturedQuadPtr& stopped,
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

	};

}

#endif
