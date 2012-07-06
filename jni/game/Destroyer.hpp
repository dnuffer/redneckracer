// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef __DESTROYER_HPP__
#define __DESTROYER_HPP__
namespace rr
{

	class Destroyer
	{
	public:
		Destroyer();
		virtual float getDamage() const;
		virtual void setDamage(float damage);
		virtual ~Destroyer();
	private:
		float damageLevel;
	};

} // namespace rr

#endif //__DESTROYER_HPP__
