// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "miniblocxx/Format.hpp"

namespace engine
{
	void Drawable::update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime)
	{
		for (vector<ActionPtr>::const_iterator it = m_actions.begin(); it != m_actions.end(); ++it)
			(*it)->apply(this, thisFrameStartTime, deltaTime);
	}

	std::string Drawable::name() const
	{
		return Format("%<1:x!>", reinterpret_cast<size_t>(this));
	}

	void Drawable::setRotation(float rotation)
	{
		m_rotation = rotation;
	}

	void Drawable::setPosition(const Point& position)
	{
		Point oldPos(m_position);
		m_position = position;
		
		onPositionChanged(oldPos, position);
	}

	Point Drawable::getPositionRelativeToOrigin(const Rectangle& screen)
	{
		switch (m_positionInterpretation) 
		{
			case E_ORIGIN:
				return m_position;
			case E_SCREEN:
				return Point(screen.left + screen.width() / 2 + m_position.x(),
							 screen.bottom + screen.height() / 2 + m_position.y());
		}
		return Point();
	}
}
