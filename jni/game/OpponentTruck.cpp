// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "OpponentTruck.hpp"

namespace rr
{
	
	float OpponentTruck::getTurnAngle()
	{
		switch (state) 
		{
			case E_DRIVING_LEFT:
				return -17.0;
			case E_TURNING_LEFT:
			case E_TURNING_LEFT_TO_STRAIGHT:
				return -9.0;
			case E_STRAIGHT:
				return 0.0;
			case E_TURNING_RIGHT:
			case E_TURNING_RIGHT_TO_STRAIGHT:
				return 9.0;
			case E_DRIVING_RIGHT:
				return 17.0;
		}
		return 0.0;
	}
	
	void PoliceTruck::flipHorizontal(bool flip)
	{
	}

}
