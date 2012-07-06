// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "TruckController.hpp"
#include "PlayerTruck.hpp"
#include "GameLibrary.hpp"
#include "engine/MoveAction.hpp"
#include "engine/AccelerateAction.hpp"


namespace rr
{
	TruckController::TruckController(TruckPtr truck, GameLibrary& gameLib):
		playerTruck(truck)
		, gameLibrary(&gameLib)
		, playerTruckFinished(false)
		, state(E_WAIT_FOR_START)
		, truckMaxSpeed(defaultMaxSpeed)
		, rageMaxSpeed(defaultRageMaxSpeed)
		, nitroElapsedTime(double(nitroChargingTime+1))
		, isNitroActivated(false)
		, isBrakePressed(false)
		, isRageModeActive(false)
		, useRageMode(true)
		, truckDestroyed(false)
	{
	}

	TruckController::TruckController(TruckPtr truck, GameLibrary& gameLib, float maxSpeed, bool useRageMode):
		playerTruck(truck)
		, gameLibrary(&gameLib)
		, playerTruckFinished(false)
		, state(E_WAIT_FOR_START)
		, truckMaxSpeed(maxSpeed)
		, rageMaxSpeed(defaultRageMaxSpeed)
		, nitroElapsedTime(double(nitroChargingTime+1))
		, isNitroActivated(false)
		, isBrakePressed(false)
		, isRageModeActive(false)
		, useRageMode(useRageMode)
		, truckDestroyed(false)
	{
	}

	TruckController::TruckController(TruckPtr truck, GameLibrary& gameLib, float maxSpeed, bool useRageMode, float rageSpeed):
		playerTruck(truck)
		, gameLibrary(&gameLib)
		, playerTruckFinished(false)
		, state(E_WAIT_FOR_START)
		, truckMaxSpeed(maxSpeed)
		, rageMaxSpeed(rageSpeed)
		, nitroElapsedTime(double(nitroChargingTime+1))
		, isNitroActivated(false)
		, isBrakePressed(false)
		, isRageModeActive(false)
		, useRageMode(useRageMode)
		, truckDestroyed(false)
	{
	}


	void TruckController::accelerate()
	{
		playerTruck->truckParams.truckAcceleration = defaultAcceleration;
		if (!playerTruck->isTurningAnimationActive())
		{
			playerTruck->animation(playerTruck->accelerating);
		}
		playerTruck->setAction(new MoveAction(0, playerTruck->truckParams.truckSpeed));
		playerTruck->addAction(new AccelerateAction(
				0, playerTruck->truckParams.truckAcceleration, playerTruck->truckParams.targetSpeed - playerTruck->truckParams.truckSpeed));

		resetElapsedTime(elapsedTime);
		prevSpeed = playerTruck->truckParams.truckSpeed;
		state = E_ACCELERATING;
	}


	void TruckController::slowdown()
	{
		playerTruck->truckParams.truckAcceleration = defaultSlowDown;
		if (!playerTruck->isTurningAnimationActive())
		{
			playerTruck->animation(playerTruck->drivingStraight);
		}
		playerTruck->setAction(new MoveAction(0, playerTruck->truckParams.truckSpeed));
		playerTruck->addAction(new AccelerateAction(0, playerTruck->truckParams.truckAcceleration));
		prevSpeed = playerTruck->truckParams.truckSpeed;
		resetElapsedTime(elapsedTime);
		state = E_SLOWDOWN;
	}

	void TruckController::constSpeed()
	{
		playerTruck->truckParams.truckAcceleration = 0.0;
		playerTruck->truckParams.truckSpeed = playerTruck->truckParams.targetSpeed;
		if (!playerTruck->isTurningAnimationActive())
		{
			playerTruck->animation(playerTruck->drivingStraight);
		}
		playerTruck->setAction(new MoveAction(0, playerTruck->truckParams.truckSpeed));
		prevSpeed = playerTruck->truckParams.truckSpeed;
		resetElapsedTime(elapsedTime);
		state = E_FULL_SPEED;
	}

	void TruckController::update(const TimeDuration& deltaTime)
	{
		elapsedTime += deltaTime;
		nitroElapsedTime += deltaTime;
		shotGun.update(deltaTime);
		if (!playerTruckFinished)
		{
			if (playerTruck->truckParams.truckSpeed > playerTruck->truckParams.targetSpeed - 10 && playerTruck->truckParams.truckSpeed < playerTruck->truckParams.targetSpeed + 10)
			{
				if (state != E_FULL_SPEED) constSpeed();
			}
			else if (playerTruck->truckParams.truckSpeed < playerTruck->truckParams.targetSpeed)
			{
				if (state != E_ACCELERATING) accelerate();
			}
			else // playerTruck->truckSpeed > playerTruck->targetSpeed
			{
				if (state != E_SLOWDOWN) slowdown();
			}
			// Update current truck speed.
			playerTruck->truckParams.truckSpeed = prevSpeed + elapsedTime.realSeconds() * playerTruck->truckParams.truckAcceleration;
			if (!truckDestroyed)
			{
				playerTruck->processState();

				// Handle offroad mode
				if (!playerTruck->isOnRoad())
				{
					// original
					if (playerTruck->truckParams.targetSpeed > offroadMaxSpeed)
					{
					    playerTruck->truckParams.targetSpeed = offroadMaxSpeed;
					}
					// relative to max speed only if rage is off
					//if (!isRageModeActive && playerTruck->truckParams.targetSpeed > truckMaxSpeed-20)
					//{
					//	playerTruck->truckParams.targetSpeed = truckMaxSpeed-20;
					//}
				}
				if (useRageMode && playerTruck->truckParams.truckRage >= Truck::defaultTruckRage && !isRageModeActive) // Handle rage mode.
				{
					// Activate Rage Mode
					isRageModeActive = true;
					playerTruck->setRageActive( true );
					playerTruck->truckParams.targetSpeed = rageMaxSpeed;
				}
				else if (isNitroActivated) // Handle nitro mode.
				{
					if (nitroElapsedTime.realSeconds() > nitroActivityTime)
					{
						resetElapsedTime(nitroElapsedTime);

						// Deactivate nitro mode after elapsed time
						isNitroActivated = false;
						playerTruck->setNitroActive( false );
						playerTruck->truckParams.targetSpeed = truckMaxSpeed;
						state = E_FULL_SPEED;
					}
				}
				else if (!isBrakePressed)
				{
					if (isRageModeActive) playerTruck->truckParams.targetSpeed = rageMaxSpeed;
					else playerTruck->truckParams.targetSpeed = truckMaxSpeed;
				}

				// Decrement rage meter when rage is active.
				if (isRageModeActive)
				{
					if (playerTruck->isOnRoad())
					{
						playerTruck->truckParams.targetSpeed = rageMaxSpeed;
					}
					else// Still boost and still slow if offroad.
					{
						playerTruck->truckParams.targetSpeed = rageMaxSpeed / 2;
					}
					playerTruck->truckParams.truckRage -= deltaTime.realSeconds() * rageMeterDecrementSpeed;
					if (playerTruck->truckParams.truckRage <= 0)
					{
						playerTruck->truckParams.truckRage = 0;
						isRageModeActive = false;
						playerTruck->setRageActive( false );

						if (playerTruck->truckParams.targetSpeed > truckMaxSpeed)
						{
							playerTruck->truckParams.targetSpeed = truckMaxSpeed;
							if (!playerTruck->isOnRoad())
							{
								playerTruck->truckParams.targetSpeed = offroadMaxSpeed;
							}
						}
					}
				}

				// If health <= 0 - truck is destroyed.
				if (playerTruck->truckParams.truckDefense <= 0 && state != E_FINISHED)
				{
					truckDestroyed = true;
					playerTruck->animation(playerTruck->drivingStraight);
					playerTruck->attachAnimation(Truck::EngineBlow, gameLibrary->effect(GameLibrary::FireAndSmoke), Point(-6, 60));
					playerTruck->activateAttachedAnimation(Truck::EngineBlow, true);
					playerTruck->setRageActive( false );
					playerTruck->endExhaustAnimation();
					playerTruck->truckParams.targetSpeed = 0;
					slowdown();
				}
			}
			else if (playerTruck->truckParams.truckSpeed == 0)
			{
				playerTruckFinished = true;
				playerTruck->setRageActive( false );
			}

		}
		else if(playerTruckFinished)// Truck is finished
		{
			playerTruck->clearActions();
			playerTruck->truckParams.truckAcceleration = 0;
			playerTruck->truckParams.truckSpeed = 0;
			playerTruck->truckParams.targetSpeed = 0;
			playerTruck->animation(playerTruck->drivingStraight);
			playerTruck->setRageActive( false );
			playerTruck->endExhaustAnimation();
			state = E_FINISHED;
		}

	}

	void TruckController::startRace()
	{
		playerTruck->truckParams.truckSpeed = 0;
		playerTruck->truckParams.targetSpeed = truckMaxSpeed;
		brakeActionUp();
        gameLibrary->truckRevFadeSound()->play();
	}

	void TruckController::brakeActionDown()
	{
		if (state != E_WAIT_FOR_START && state != E_FINISHED && !truckDestroyed)
		{
			playerTruck->truckParams.targetSpeed = 0;
			isBrakePressed = true;
		}
	}

	void TruckController::brakeActionUp()
	{
		if (state != E_WAIT_FOR_START && state != E_FINISHED && !truckDestroyed)
		{
			if (!playerTruck->isOnRoad())
			{
				playerTruck->truckParams.targetSpeed = offroadMaxSpeed;
			}
			else if (isNitroActivated)
			{
				playerTruck->truckParams.targetSpeed = nitroMaxSpeed;
			}
			else if (isRageModeActive)
			{
				playerTruck->truckParams.targetSpeed = rageMaxSpeed;
			}
			else
			{
				playerTruck->truckParams.targetSpeed = truckMaxSpeed;
			}

			isBrakePressed = false;
		}
	}

	void TruckController::nitroActionDown()
	{
		/*
		if (state != E_WAIT_FOR_START && state != E_FINISHED && !truckDestroyed)
		{
			if (nitroElapsedTime.realSeconds() > nitroChargingTime)
			{
				playerTruck->truckParams.targetSpeed = nitroMaxSpeed;
				resetElapsedTime(nitroElapsedTime);
				isNitroActivated = true;
				playerTruck->setNitroActive( true );
				LOGD("TruckController::nitroActionDown() --- isNitroActivated = true");
				//playerTruck->activateAttachedAnimation(Truck::ExhaustFlames, true);
				
			}
		}
		* */
	}

	void TruckController::resetElapsedTime(TimeDuration& elTime)
	{
		TimeDuration td;
		elTime = td;
	}

	void TruckController::setFinished()
	{
		playerTruckFinished = true;
	}

	bool TruckController::isFinished() const
	{
		return playerTruckFinished;
	}

	bool TruckController::isDestroyed() const
	{
		return truckDestroyed;
	}

	TruckPtr& TruckController::getTruck()
	{
		return playerTruck;
	}

	ShotGun&  TruckController::getShotGun()
	{
		return shotGun;
	}

} // namespace rr

