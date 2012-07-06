// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef rr_game_Truck_hpp_INCLUDED_
#define rr_game_Truck_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "Globals.hpp"
#include "engine/Sprite.hpp"
#include "Destroyer.hpp"
#include "engine/FollowAction.hpp"

namespace rr
{
	class Truck : public Sprite, public Destroyer
	{
	public:
		Truck(const TexturedQuadPtr& stopped,
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
			: Sprite(stopped, name)
			, Destroyer()
			, truckParams(0, 0, defaultTruckDefense, defaultTruckArmor, 0)
			, accelerating(accelerating)
			, drivingStraight(drivingStraight)
			, currentTurningDegrees(0.0f)
			, state(E_STRAIGHT)
			, currentSpeed(0)
			, turningLeft(turningLeft)
			, drivingLeft(drivingLeft)
			, turningLeftToStraight(turningLeftToStraight)
			, turningRight(turningRight)
			, drivingRight(drivingRight)
			, turningRightToStraight(turningRightToStraight)
			, damageMultiplier(1.0)
			, onRoad(true)
			, isInputLeft(false)
			, isInputRight(false)
			, exhaustFlamesSprite(NULL)
			, rageModeActivated(false)
			, nitroActivated(false)
			, screenPosition(0,0)
            , _hitSound(hitSound)
		{
			// Set default damage when trucks collide.
			Destroyer::setDamage(1.0f);

			// Clear vector of attached animation sprites
			// (Each sprite will be created after first calling AttachAnimation(...)).
			attachedAnimations.resize(attachedAnimationTypesCount);

			// Attach sparks animation (appear when truck collide with other truck).
			attachAnimation(Sparks, game().library().effect(GameLibrary::Sparks), Point(0, 0));
			attachedAnimations[(size_t)Sparks].animationSprite->update(DateTime(0), TimeDuration(15.0));
		}

		virtual bool shouldCheckForCollision() const { return true; }
		virtual void handleCollision(Collidable& other, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
		virtual bool containsPoint(Point p);
		
		virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
		void processState();
		virtual void setDamageMultiplier(float multiplier) { damageMultiplier = multiplier; }
		virtual float getDamageMultiplier() const { return damageMultiplier; }
		
		static const float defaultTruckDefense = 1000;
		static const float defaultTruckArmor = 700;
		static const float defaultTruckRage = 100;

		virtual void decrementHealth(float damage);

		struct TruckParameters // these parameters managed by TruckController
		{
			TruckParameters(float speed, float accel, float def, float arm, float rage):
				  truckSpeed(speed), truckAcceleration(accel), truckDefense(def), truckArmor(arm), truckRage(rage)
				, targetSpeed(0)
			{}
			float truckSpeed;
			float truckAcceleration;
			float truckDefense;
			float truckArmor;
			float truckRage;
			float targetSpeed;
		};

		TruckParameters truckParams;

		enum AttachedAnimationType
		{
			Sparks,
			ShotgunBlast,
			EngineBlow
		};
		
		static const unsigned int attachedAnimationTypesCount = 3;

		virtual void attachAnimation(AttachedAnimationType type, AnimationPtr animation, Point position);
		virtual void activateAttachedAnimation(AttachedAnimationType type, bool loop = false);
		virtual SpritePtr& getAttachedAnimationSprite(AttachedAnimationType type);
		virtual void setAttachedAnimationPosition(AttachedAnimationType type, const Point& pos);
		
		virtual void setAttachedAnimationHorizFlip(AttachedAnimationType type, bool flipHoriz);
		virtual void rotateAttachedAnimation(AttachedAnimationType type, float rotation);
		
		virtual void setRageActive( bool isRageActivated );
		virtual void setNitroActive( bool isNitroActivated );
		
		virtual void replaceAttachedAnimation( AttachedAnimationType type, AnimationPtr& newAnimation, bool horizFlip, Point pos );
		
		virtual void draw(const Rectangle& screen);

		virtual bool isOnRoad() const { return onRoad; }

		TexturedQuadPtr stopped;
		AnimationPtr accelerating;
		AnimationPtr drivingStraight;
		virtual void setInputLeft(bool input) { isInputLeft = input; isInputRight = false; }
		virtual void setInputRight(bool input) { isInputRight = input; isInputLeft = false; }

		bool isTurningAnimationActive() const
		{
			if (currentTurningDegrees != 0.0f) return true;
			else return false;
		}
        
        virtual bool isRacer() const { return false; }
        virtual bool isPlayer() const { return false; }
        
        void playHitSound() const;
		void stopSounds() const;
        
        virtual void endExhaustAnimation();

	private:
		virtual bool inputLeft() const { return isInputLeft; }
		virtual bool inputRight() const { return isInputRight; }
		virtual void clearTurningAction();
		float currentTurningDegrees;
		virtual float getTurnAngle() = 0;
		// overridden by PoliceTruck because the turning right animations are not re-used left animations
		virtual void flipHorizontal(bool flip);

	protected:
		enum ETurningState
		{
			// transition to E_TURNING_LEFT when tilted left
			// transition to E_TURNING_RIGHT when tilted right
			E_STRAIGHT,
			
			// transition to E_DRIVING_LEFT after turn animation complete
			// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted left
			E_TURNING_LEFT,
			
			// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted left
			E_DRIVING_LEFT,
			
			// transition to E_STRAIGHT after turn animation complete
			// transition to E_TURNING_LEFT if tilted left
			E_TURNING_LEFT_TO_STRAIGHT,
			
			// transition to E_DRIVING_RIGHT after turn animation complete
			// transition to E_TURNING_LEFT_TO_STRAIGHT if not tilted right
			E_TURNING_RIGHT,
			
			// transition to E_TURNING_RIGHT_TO_STRAIGHT if not tilted right
			E_DRIVING_RIGHT,
			
			// transition to E_STRAIGHT after turn animation complete
			// transition to E_TURNING_RIGHT if tilted right
			E_TURNING_RIGHT_TO_STRAIGHT,
			
		};

		ETurningState state;
		
	private:
	
		virtual void handleExhaustAnimation(ETurningState oldState, ETurningState newState);

		DateTime turningStartedTime;
		DateTime elapsedTime;
		int currentSpeed;
		
		AnimationPtr turningLeft;
		AnimationPtr drivingLeft;
		AnimationPtr turningLeftToStraight;
		AnimationPtr turningRight;
		AnimationPtr drivingRight;
		AnimationPtr turningRightToStraight;
		
		float damageMultiplier; // Used to manage damage level by truck speed.
		bool onRoad;

		bool isInputLeft;
		bool isInputRight;
		
		SpritePtr exhaustFlamesSprite;
		boost::shared_ptr<FollowAction> followingTruck;
		bool rageModeActivated;
		bool nitroActivated;
		
		boost::signals2::connection flamesFollowTruckConnection;
		
		static const bool FLIP_HORIZ = true;
		
		void setDirection(float angleInDegrees);

		struct AttachedAnimationSprite
		{
			AttachedAnimationSprite(): animation(), animationSprite(), position(0,0){}
			AnimationPtr animation;
			SpritePtr animationSprite;
			Point position;
		};
		vector<AttachedAnimationSprite> attachedAnimations;
		
		Point screenPosition;

        SoundPtr _hitSound;
    };
}


#endif
