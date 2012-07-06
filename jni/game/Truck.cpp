// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "RRConfig.hpp"
#include "Truck.hpp"
#include "TrackBorder.hpp"
#include "engine/MoveAction.hpp"
#include "engine/FollowAction.hpp"
#include "engine/AccelerateAction.hpp"
#include "engine/Log.hpp"
#include "RaceScene.hpp"
#include "Obstacle.hpp"
#include "RoadBound.hpp"
#include "CivilCar.hpp"
#include "boost/signals2.hpp"
#include "boost/bind.hpp"


namespace rr
{
	namespace
	{
		ActionPtr MoveActionForAngle(float degrees, float speed)		
		{
			float exponent = std::pow(abs(degrees), 1.35f);
			if (exponent > 90) {
				exponent = 90;
			}
			if (degrees < 0) {
				exponent *= -1;
			}
			return new MoveAction(sin(exponent * M_PI / 180.0) * speed, 0);
		}
	}


	void Truck::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		// Set damage multiplier by current speed.
		damageMultiplier = truckParams.truckSpeed / 200.0;

		Sprite::update(thisFrameStartTime, deltaTime);
		float turnAngle = getTurnAngle();
		if (turnAngle != 0.0f)
		{		
			MoveActionForAngle(turnAngle, truckParams.truckSpeed)->apply(this, thisFrameStartTime, deltaTime);
		}

		for (vector<AttachedAnimationSprite>::iterator sprite = attachedAnimations.begin(); sprite != attachedAnimations.end(); ++sprite)
		{
			if(sprite->animationSprite != NULL)
			{
				(sprite->animationSprite)->update(thisFrameStartTime, deltaTime);
			}
		}
		elapsedTime = thisFrameStartTime;
	}

	void Truck::processState()
	{
		const float turnDelay = 0.5;
		const float turnAngle = 45.0;
		
		//LOGD("Starting processState() STATE: %i INPUT: L: %i, R: %i", (size_t)state, (int)inputLeft(), (int)inputRight());
		
		ETurningState oldState = state;
		
		switch (state)
		{
			case E_STRAIGHT:
			{
				// transition to E_TURNING_LEFT when tilted left
				// transition to E_TURNING_RIGHT when tilted right
				
				if (inputLeft())
				{
					state = E_TURNING_LEFT;
					flipHorizontal(false);
					animation(turningLeft);
					setDirection(-10);
					turningStartedTime = elapsedTime;
				}
				if (inputRight())
				{
					state = E_TURNING_RIGHT;
					flipHorizontal(true);
					animation(turningRight);
					setDirection(10);
					turningStartedTime = elapsedTime;
				}	
				break;
			}

			case E_TURNING_LEFT:
			{
				// transition to E_DRIVING_LEFT after turn animation complete
				// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted left
				if (!inputLeft())
				{
					state = E_TURNING_LEFT_TO_STRAIGHT;
					flipHorizontal(false);
					animation(turningLeftToStraight);
					setDirection(-5);
				}
				else
				{
					if ((elapsedTime - turningStartedTime).realSeconds() >= turnDelay)
					{
						state = E_DRIVING_LEFT;
						flipHorizontal(false);
						animation(drivingLeft);
						setDirection(-turnAngle);
					}
				}
				break;
			}

			case E_DRIVING_LEFT:
			{
				// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted left
				if (!inputLeft())
				{
					state = E_TURNING_LEFT_TO_STRAIGHT;
					flipHorizontal(false);
					animation(turningLeftToStraight);
					setDirection(-5);
					turningStartedTime = elapsedTime;
				}
				break;
			}

			case E_TURNING_LEFT_TO_STRAIGHT:
			{
				// transition to E_STRAIGHT after turn animation complete
				// transition to E_TURNING_LEFT if tilted left
				if (inputLeft())
				{
					state = E_TURNING_LEFT;
					flipHorizontal(false);
					animation(turningLeft);
					setDirection(-10);
					turningStartedTime = elapsedTime;
				}
				else
				{
					if ((elapsedTime - turningStartedTime).realSeconds() >= turnDelay)
					{
						state = E_STRAIGHT;
						flipHorizontal(false);
						animation(drivingStraight);
						clearTurningAction();
					}
				}
				break;
			}

			case E_TURNING_RIGHT:
			{
				// transition to E_DRIVING_RIGHT after turn animation complete
				// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted right
				if (!inputRight())
				{
					state = E_TURNING_RIGHT_TO_STRAIGHT;
					flipHorizontal(true);
					animation(turningRightToStraight);
					setDirection(-5);
				}
				else
				{
					if ((elapsedTime - turningStartedTime).realSeconds() >= turnDelay)
					{
						state = E_DRIVING_RIGHT;
						flipHorizontal(true);
						animation(drivingRight);
						setDirection(turnAngle);
					}
				}
				break;
			}

			case E_DRIVING_RIGHT:
			{
				// transition to E_TURNING_RIGHT_TO_STRAIGHT if not tilted right
				if (!inputRight())
				{
					state = E_TURNING_RIGHT_TO_STRAIGHT;
					flipHorizontal(true);
					animation(turningRightToStraight);
					setDirection(5);
					turningStartedTime = elapsedTime;
				}
				break;
			}

			case E_TURNING_RIGHT_TO_STRAIGHT:
			{
				// transition to E_STRAIGHT after turn animation complete
				// transition to E_TURNING_RIGHT if tilted right
				if (inputRight())
				{
					state = E_TURNING_RIGHT;
					flipHorizontal(true);
					animation(turningRight);
					setDirection(10);
				}
				else
				{
					if ((elapsedTime - turningStartedTime).realSeconds() >= turnDelay)
					{
						state = E_STRAIGHT;
						flipHorizontal(false);
						animation(drivingStraight);
						clearTurningAction();
					}
				}
				break;
			}
		}
		
		handleExhaustAnimation(oldState, state);
		//LOGD("Finished processState()");
	}

	void Truck::handleExhaustAnimation(ETurningState oldTurningState, ETurningState newTurningState)
	{
		//LOGD("Truck::handleExhaustAnimation()");
		
		if(rageModeActivated)
		{	
			/* if the newState equals the old state, keep playing current exhaust anim (IMPLIED DEFAULT)
			 * if the state has changed and anim is already playing, switch exhaust animation based on new state OR
			 * if there is no exhaust anim playing, start playing it based on new state 
			 */
			if(newTurningState != oldTurningState || exhaustFlamesSprite == NULL)
			{	
				switch(newTurningState)
				{	
					case E_STRAIGHT:
						exhaustFlamesSprite = game().library().exhaustFlamesDrivingStraight();
						
					break;
					case E_DRIVING_LEFT:
						exhaustFlamesSprite = game().library().exhaustFlamesDrivingLeft();

					break;
					case E_DRIVING_RIGHT:
						exhaustFlamesSprite = game().library().exhaustFlamesDrivingRight();
					break;
					default:
						// while turning, exhaust animation does not play.  If it is playing, it is removed here.
						if(exhaustFlamesSprite != NULL)
						{
							game().raceScene->removeChild(exhaustFlamesSprite);
							exhaustFlamesSprite = NULL;
						}
					break;
				}
				if(exhaustFlamesSprite != NULL)
				{
					followingTruck = boost::shared_ptr<FollowAction>(new FollowAction(exhaustFlamesSprite, exhaustFlamesSprite->position()));
					exhaustFlamesSprite->setPosition(Point(0.0, 0.0));		
					flamesFollowTruckConnection = OnPositionChanged().connect(Drawable::positionChangedSignalT::slot_type(&FollowAction::OnPositionChanged, 
																				followingTruck.get(), _2).track(followingTruck));
					game().raceScene->addChild(exhaustFlamesSprite, RaceScene::sparksZOrder);
				}
			}
		} 
		else if(exhaustFlamesSprite != NULL)
		{
			//Exhaust flames Sprite is active, but no longer in rage mode - removing exhaust Flames from scene
			game().raceScene->removeChild(exhaustFlamesSprite);
			exhaustFlamesSprite = NULL;
		}
		//LOGD("finished Truck::handleExhaustAnimation()");
	}
	
	/*
	 * Truck::endExhaustAnimation() called from TruckController when truck dies or reaches finish line
	 * and the Truck's animation is externally set to driving straight w/o calling Truck::processState()
	 */
	void Truck::endExhaustAnimation()
	{
		handleExhaustAnimation(state,  E_STRAIGHT);
	}
	
	typedef GenericVector2<float> Vector;

	bool pointingLeft(const Vector& v)
	{
		return (v.x() <= 0.0) && (-v.x() >= v.y());
	}
	
	bool pointingUp(const Vector& v)
	{
		return (v.y() > 0.0) && (v.y() > v.x());
	}
	
	bool pointingRight(const Vector& v)
	{
		return (v.x() >= 0.0) && (v.x() >= v.y());
	}
	
	bool pointingDown(const Vector& v)
	{
		return (v.y() < 0.0) && (-v.y() > v.x());
	}
	
	bool Truck::containsPoint(Point p)
	{
		//LOGD("MICHAEL >>>>> screenX: %f, screenY: %f",screenX,screenY);
		//LOGD("MICHAEL >>>>> Truck #%s : %f, %f",name().c_str(),screenPosition.x(),screenPosition.y());
		
		Rectangle bound = Rectangle::makeCenteredOn(position(),size());
		//LOGD("MICHAEL >>>>> Position: %f, %f, %f, %f",bound.left,bound.right,bound.top,bound.bottom);
		if(bound.left   > p.x()) return false;
		if(bound.right  < p.x()) return false;
		if(bound.top    < p.y()) return false;
		if(bound.bottom > p.y()) return false;
		//LOGD("MICHAEL >>>>> HIt a truck!!!");
		return true;
	}
	
	Point getNearestNonCollidingPosition(const Point& colliderCenter, const Rectangle& collider, 
										 const Point& collideeCenter, const Rectangle& collidee)
	{
		Vector normCenterDiff = colliderCenter - collideeCenter;
		normCenterDiff.x() /= collidee.width();
		normCenterDiff.y() /= collidee.height();
		//LOGD("normCenterDiff: %f, %f", normCenterDiff.x(), normCenterDiff.y());
		if (pointingLeft(normCenterDiff))
		{
			//LOGD("l");
			return Point(collidee.left - (collider.width() / 2) - 1, colliderCenter.y());
		}
		else if (pointingUp(normCenterDiff))
		{
			//LOGD("u");
			return Point(colliderCenter.x(), collidee.top + (collider.height() / 2) + 1);
		}
		else if (pointingRight(normCenterDiff))
		{
			//LOGD("r");
			return Point(collidee.right + (collider.width() / 2) + 1, colliderCenter.y());
		}
		else if (pointingDown(normCenterDiff))
		{
			//LOGD("d");
			return Point(colliderCenter.x(), collidee.bottom - (collider.height() / 2) - 1);
		}
		return Point(); // silence warning since the compiler can't know one of the if branches will execute
	}
	
	void splitCollisionMovement(const Point& colliderCenter, const Rectangle& collider, 
								const Point& collideeCenter, const Rectangle& collidee,
								Point& colliderCenterOut, Point& collideeCenterOut)
	{
		Vector colliderMovement = getNearestNonCollidingPosition(colliderCenter, collider, collideeCenter, collidee) 
									- colliderCenter;
		//if (colliderMovement.x() > 30)
		//	LOGD("BIG JUMP!!! colliderMovement: %f, %f", colliderMovement.x(), colliderMovement.y());
		colliderMovement /= 2;
		colliderCenterOut = colliderCenter + colliderMovement;
		collideeCenterOut = collideeCenter - colliderMovement;
	}

	void Truck::handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		float damage(0);
		float rageIncrement(0);
		if (Destroyer* destroyer = dynamic_cast<Destroyer*>(&other))
		{
			damage = destroyer->getDamage();
			rageIncrement = damage * 1.0f;
			if (this->isPlayer()) { rageIncrement = damage * 2.0f; }
		}

		float damMult(1.0); // Damage multiplier.
		if (RoadBound* road = dynamic_cast<RoadBound*>(&other))
		{
			Coord border = road->getBoundCoordinates(this->getBoundingRect().top);
			float mid = (this->getBoundingRect().left +  this->getBoundingRect().right) / 2.0;
			if ((mid < border.x()) || (mid > border.y()))
			{
				// Offroad driving.
				// Damage and rage from offroad driving are processed in class TruckController.
				onRoad = false;
			}
			else
			{
				// On road driving.
				onRoad = true;
			}
		}
		else if (TrackBorder* border = dynamic_cast<TrackBorder*>(&other))
		{
			//LOGD("Truck %s collided with edge. Position: %f, %f", name().c_str(), position().x(), position().y());
			Rectangle borderRect(border->getBoundingRect());
			Point newPosition = getNearestNonCollidingPosition(position(), getBoundingRect(),
															   center(borderRect), borderRect);
			//LOGD("Set truck position to: %f, %f", newPosition.x(), newPosition.y());
			setPosition(newPosition);
		}
		else if (Truck* truck = dynamic_cast<Truck*> (&other))
		{
			//LOGD("Truck %s collided with another truck %s! Position: %f, %f", name().c_str(), truck->name().c_str(), 
			//	 position().x(), position().y());
			Point newColliderPosition, newCollideePosition;
			splitCollisionMovement(position(), getBoundingRect(), truck->position(), truck->getBoundingRect(),
								   newColliderPosition, newCollideePosition);
			//LOGD("Set truck to: %f, %f", newColliderPosition.x(), newColliderPosition.y());
			//LOGD("Set other truck to: %f, %f", newCollideePosition.x(), newCollideePosition.y());
			setPosition(newColliderPosition);
			truck->setPosition(newCollideePosition);
			
			// start sparks animation
			//attachedAnimations[(size_t)Sparks].animationSprite->animation(game().library().effect(GameLibrary::Sparks));
			Point mid = midpoint(newColliderPosition, newCollideePosition);
			attachedAnimations[(size_t)Sparks].position = Point(mid.x() - newColliderPosition.x() , mid.y() - newColliderPosition.y() );
			activateAttachedAnimation(Truck::Sparks);
            if(this->isPlayer())
                truck->playHitSound();
			// Set damage multiplier for this truck (if opponent truck is faster, damage for you is bigger).
			damMult = truck->getDamageMultiplier() * deltaTime.realSeconds() * 5.0;
		}
		else if (Obstacle* obstacle = dynamic_cast<Obstacle*> (&other))
		{
			// Cars don't destroy obstacles
			if ( dynamic_cast<CivilCar*> (this) == NULL)
			{
				if ( obstacle->destroyed()) { rageIncrement = 0; }
				else
				{
					obstacle->switchToHitAnimation();
					// Reduce damage level (obstacle is destructed).
					obstacle->setDamage(1);
                    
                    if (this->isPlayer())
                        obstacle->playHitSound();
				}

				// Set damage multiplier (when your truck is faster, damage from obstacle for you is bigger).
				damMult = getDamageMultiplier();
			}
		}
		else if (Animal* animal = dynamic_cast<Animal*> (&other))
		{
			// Cars don't destroy animals
			if ( dynamic_cast<CivilCar*> (this) == NULL)
			{
				animal->switchToDead();
				// Reset damage level (animal is dead).
				animal->setDamage(0);
				damMult = getDamageMultiplier();
                if (this->isPlayer())
                    animal->playHitSound();
			}
		}

		// Update truck health level.
		damage = damMult * damage;
		decrementHealth(damage);

		// Update truck rage level.
		// Increment when hitting someone object (truck, obstacle, border)!
		if (rageIncrement > defaultTruckRage - truckParams.truckRage)
		{// Rage cap catch
			rageIncrement = defaultTruckRage - truckParams.truckRage;
		}
		truckParams.truckRage += rageIncrement;
	}

	void Truck::decrementHealth(float damage)
	{
		if (truckParams.truckArmor > damage)
		{
			truckParams.truckArmor -= damage;
		}
		else
		{
			if (truckParams.truckDefense > damage)
			{
				truckParams.truckDefense -= damage - truckParams.truckArmor;
			}
			else
			{
				truckParams.truckDefense = 0;
			}
			truckParams.truckArmor = 0;
		}
	}

	void Truck::clearTurningAction()
	{
		currentTurningDegrees = 0.0f;
	}

	void Truck::setDirection(float angleInDegrees)
	{
		//setRotation(angleInDegrees);
		currentTurningDegrees = angleInDegrees;
	}
	
	void Truck::setAttachedAnimationHorizFlip(AttachedAnimationType type, bool flipHoriz)
	{
		SpritePtr & attachedAnim = getAttachedAnimationSprite(type);
		attachedAnim->setFlippedHorizontal(flipHoriz);
	}

	void Truck::attachAnimation(AttachedAnimationType type,AnimationPtr animation, Point position)
	{
		LOGASSERT((size_t)type <= attachedAnimations.size(), "Invalid attached animation type: %zu", (size_t)type);
		attachedAnimations[(size_t)type].animation = animation;
		if (attachedAnimations[(size_t)type].animationSprite == NULL)
		{
			attachedAnimations[(size_t)type].animationSprite = new Sprite(attachedAnimations[(size_t)type].animation, "Attached");
		}
		else
		{
			attachedAnimations[(size_t)type].animationSprite->animation(attachedAnimations[(size_t)type].animation);
		}

		attachedAnimations[(size_t)type].position = position;
	}

	// Return NULL if animation was not attached by calling attachAnimation.
	SpritePtr& Truck::getAttachedAnimationSprite(AttachedAnimationType type)
	{
		LOGASSERT((size_t)type <= attachedAnimations.size(), "Invalid attached animation type: %zu", (size_t)type);
		return attachedAnimations[(size_t)type].animationSprite;
	}

	void Truck::draw(const Rectangle& screen)
	{
		Sprite::draw(screen);
		for (vector<AttachedAnimationSprite>::iterator sprite = attachedAnimations.begin(); sprite != attachedAnimations.end(); ++sprite)
		{
			if(sprite->animationSprite != NULL)
			{
				sprite->animationSprite->setPosition(Point(position().x() + sprite->position.x(), position().y() + sprite->position.y()));
				sprite->animationSprite->draw(screen);
			}
		}
	}

	void Truck::activateAttachedAnimation(AttachedAnimationType type, bool loop)
	{
		LOGASSERT((size_t)type <= attachedAnimations.size(), "Invalid attached animation type: %zu", (size_t)type);
		if (attachedAnimations[(size_t)type].animationSprite != NULL)
		{
			attachedAnimations[(size_t)type].animation->reset(loop?(Animation::E_LOOP):(Animation::E_PLAY_ONCE));
		}
	}

	void Truck::setAttachedAnimationPosition(AttachedAnimationType type, const Point& pos)
	{
		LOGASSERT((size_t)type <= attachedAnimations.size(), "Invalid attached animation type: %zu", (size_t)type);
		if (attachedAnimations[(size_t)type].animationSprite != NULL)
		{
			attachedAnimations[(size_t)type].position  = pos;
		}
	}
	
	void Truck::rotateAttachedAnimation(AttachedAnimationType type, float rotation)
	{
		if (attachedAnimations.size() >= (size_t)type && attachedAnimations[(size_t)type].animationSprite != NULL)
		{
			attachedAnimations[(size_t)type].animationSprite->setRotation( rotation );
		}
	}
	
	void Truck::setRageActive( bool isRageActivated )
	{
		rageModeActivated = isRageActivated;
	}
	void Truck::setNitroActive( bool isNitroActivated )
	{
		nitroActivated = isNitroActivated;
	}
	
	void Truck::replaceAttachedAnimation( AttachedAnimationType type, AnimationPtr& newAnimation, bool horizFlip, Point pos )
	{
		attachAnimation(type, newAnimation, pos);
		attachedAnimations[(size_t)type].animationSprite->setFlippedHorizontal(horizFlip);
	}
    
    void Truck::playHitSound() const
    {
        if (_hitSound)
            _hitSound->play();
    }
	
	void Truck::stopSounds() const
	{
		if (_hitSound) {
			_hitSound->stop();
		}
	}
	
	void Truck::flipHorizontal(bool flip)
	{
		Sprite::setFlippedHorizontal(flip);
	}
}
