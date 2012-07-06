// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "ShotGun.hpp"
#include "engine/Point.hpp"
#include "graphlib/GenericVector2.hpp"
#include "engine/Log.hpp"

namespace rr
{
	ShotGun::ShotGun()
		: shotRange(500)
		, reloadingTime(1.5f)
		, elapsedTime(11.0f)
		, ammoCount(200)
	{
		setDamage(50);
	}

	ShotGun::~ShotGun()
	{

	}

	void ShotGun::shoot()
	{
		if(ammoCount > 0)
		{
			elapsedTime = TimeDuration(0.0f);
			--ammoCount;
		}
	}

	bool ShotGun::isReadyToShoot()
	{
		if (elapsedTime.realSeconds() > reloadingTime) return true;
		else return false;
	}

	void ShotGun::update(const TimeDuration& deltaTime)
	{
		elapsedTime += deltaTime;
	}

	bool ShotGun::isObjectInRange(const Rectangle& plRect, const Rectangle& objRect) const
	{
		Point playerCenter = Point((plRect.left + plRect.right) / 2, (plRect.top + plRect.bottom) / 2);
		Point opponentCenter = Point((objRect.left + objRect.right) / 2, (objRect.top + objRect.bottom) / 2);
		graphlib::GenericVector2<float> v1(playerCenter.x(),playerCenter.y());
		graphlib::GenericVector2<float> v2(opponentCenter.x(),opponentCenter.y());
		float dist = (v1-v2).length();
		return (dist <= shotRange);
	}
	
	int ShotGun::calculateDamage(const Point& p1, const Point& p2) const
	{
		graphlib::GenericVector2<float> v1(p1.x(),p1.y());
		graphlib::GenericVector2<float> v2(p2.x(),p2.y());
		float dist = (v1-v2).length();
		float mult = 0.0;
		if (dist <= shotRange/2) {
			mult = 1.0;
		} else if (dist <= shotRange) {
			mult = (-dist / (shotRange/2)) + 2.0;
		}
		return (mult * getDamage());
	}

} // namespce rr


