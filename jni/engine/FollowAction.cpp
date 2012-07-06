#include "EngineConfig.hpp"
#include "FollowAction.hpp"
#include "engine/Drawable.hpp"

namespace engine
{
void FollowAction::OnPositionChanged(const Point & newPos)
{
		Point tempPos(newPos.x() + _posRelativeToFollowed.x(), 
						newPos.y() + _posRelativeToFollowed.y());
						
		_target->setPosition(tempPos);
}
}
