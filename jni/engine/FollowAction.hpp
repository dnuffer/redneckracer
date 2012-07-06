#ifndef engine_FollowAction_hpp_INCLUDED_
#define engine_FollowAction_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "engine/Point.hpp"
#include "engine/EngineFwd.hpp"

namespace engine
{

class FollowAction
{
public:
	FollowAction(const DrawablePtr& target, Point posRelativeToFollowed)
		: _target(target)
		, _posRelativeToFollowed(posRelativeToFollowed)
	{	
	}
	
	void OnPositionChanged(const Point & newPos);
	
private:
	
	DrawablePtr _target;
	Point _posRelativeToFollowed;
};

}

#endif
