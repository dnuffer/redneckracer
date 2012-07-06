// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __TRUCK_CONTROLLER_HPP__
#define __TRUCK_CONTROLLER_HPP__


#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "ShotGun.hpp"

namespace rr
{
	class TruckController: public virtual IntrusiveCountableBase
	{
	public:

		TruckController(TruckPtr truck, GameLibrary& gameLib);
		TruckController(TruckPtr truck, GameLibrary& gameLib, float maxSpeed, bool useRageMode);
        TruckController(TruckPtr truck, GameLibrary& gameLib, float maxSpeed, bool useRageMode, float rageSpeed);

		void update(const TimeDuration& deltaTime);

		enum EState
		{
			E_WAIT_FOR_START = 0,
			E_ACCELERATING,
			E_FULL_SPEED,
			E_SLOWDOWN,
			E_FINISHED
		};

		// Functions to interact with brakes button.
		void brakeActionDown();
		void brakeActionUp();

		// Functions to interact with nitro button.
		void nitroActionDown();

		bool isFinished() const;
		bool isDestroyed() const;

		void setFinished();

		void startRace();

		TruckPtr& getTruck();
		ShotGun& getShotGun();

		bool isBraking() const { return isBrakePressed; }

		const static double nitroMaxSpeed = 520.0;
		const static double defaultRageMaxSpeed = 620.0;
		const static double offroadMaxSpeed = 370.0;

		const static double defaultMaxSpeed = 470.0;
		const static int defaultSlowDown = -250;
		const static int defaultAcceleration = 250;

	private:
		void resetElapsedTime(TimeDuration& elTime);
		void accelerate();
		void slowdown();
		void constSpeed();

	private:
		TruckPtr playerTruck; // playerTruck may be a opponent truck, controlling by AI.
		GameLibrary* gameLibrary;
		TimeDuration elapsedTime;
		bool playerTruckFinished;
		EState state;
		double truckMaxSpeed;
		double rageMaxSpeed;

		const static unsigned int nitroChargingTime = 20; // In seconds.
		const static unsigned int nitroActivityTime = 3; // In seconds.
		const static unsigned int rageMeterDecrementSpeed = 15;
		const static float rageByOffroadPerSecond = 1.0f;
		const static float damageByOffroadPerSecond = 1.0f;
		TimeDuration nitroElapsedTime;
		TimeDuration truckRageElapsedTime;
		bool isNitroActivated;
		bool isBrakePressed;
		bool isRageModeActive;
		double prevSpeed; // For slowdown handling.
		bool useRageMode;
		bool truckDestroyed;
		ShotGun shotGun;


	};

} // namespace rr

#endif // __TRUCK_CONTROLLER_HPP__
