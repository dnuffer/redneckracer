// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __SHOTGUN_HPP__
#define __SHOTGUN_HPP__

#include "RRConfig.hpp"
#include "RRFwd.hpp"
#include "Destroyer.hpp"
#include "miniblocxx/TimeDuration.hpp"
#include "engine/Rectangle.hpp"
#include "engine/Point.hpp"

namespace rr
{
	class ShotGun: public Destroyer
	{
	public:

		ShotGun();
		virtual ~ShotGun();
		float getShotRange() const { return shotRange; }
		void setShotRange(float range) { shotRange = range; }
		virtual void setReloadingTime(unsigned int seconds) { reloadingTime = seconds; }
		virtual unsigned int getReloadingTime() const { return reloadingTime; }
		virtual void shoot();
		virtual bool isReadyToShoot();
		virtual void update(const TimeDuration& deltaTime); // For handle reloading.
		bool isObjectInRange(const Rectangle& plRect, const Rectangle& objRect) const;
		int calculateDamage(const Point& p1, const Point& p2) const;

	private:

		float shotRange;
		float reloadingTime;
		TimeDuration elapsedTime;
		unsigned int ammoCount;
	};

} // namespace rr


#endif // __SHOTGUN_HPP__
