// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#ifndef rr_game_TrackBorder_hpp_INCLUDED_
#define rr_game_TrackBorder_hpp_INCLUDED_

#include "RRConfig.hpp"
#include "engine/Collidable.hpp"
#include "engine/Rectangle.hpp"
#include "Destroyer.hpp"
#include "engine/Log.hpp"

namespace rr
{
	class TrackBorder : public Collidable, public Destroyer
	{
	public:
		TrackBorder(const Rectangle& rect)
			: Collidable(rect)
			, Destroyer()
		{
			Destroyer::setDamage(0);

			LOGD("Creating track border with rectangle %f,%f,%f,%f", rect.left, rect.right, rect.top, rect.bottom);
		}
	};

	typedef boost::intrusive_ptr<TrackBorder> TrackBorderPtr;
}


#endif
