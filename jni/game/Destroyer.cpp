// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include "Destroyer.hpp"

namespace rr
{
	Destroyer::Destroyer(): damageLevel(0.0f)
	{ }

	float Destroyer::getDamage() const
	{
		return damageLevel;
	}

	void Destroyer::setDamage(float damage)
	{
		damageLevel = damage;
	}

	Destroyer::~Destroyer()
	{

	}

} // namespace rr
