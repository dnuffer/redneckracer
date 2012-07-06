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

#ifndef DRAWABLE_HPP_INCLUDED
#define DRAWABLE_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "miniblocxx/stl/vector"
#include "miniblocxx/TimeDuration.hpp"
#include "miniblocxx/String.hpp"
#include "Point.hpp"
#include "Action.hpp"
#include "Rectangle.hpp"
#include "boost/signals2.hpp"



namespace engine
{

class Drawable : public virtual IntrusiveCountableBase
{
public:
	Drawable()
		: m_rotation(0.0)
		, m_position(0.0, 0.0)
		, m_positionInterpretation(E_ORIGIN)
	{}

	virtual void update(const DateTime& thisFrameStartTime, const TimeDuration& deltaTime);
	virtual void draw(const Rectangle& screen) = 0;
	virtual std::string name() const;

	float rotation() const { return m_rotation; }
	virtual void setRotation(float rotation);

	const Point& position() const { return m_position; }
	virtual void setPosition(const Point& position);
	Point getPositionRelativeToOrigin(const Rectangle& screen);

	void addAction(const ActionPtr& action)
	{
		m_actions.push_back(action);
	}
	void clearActions()
	{
		m_actions.clear();
	}
	void setAction(const ActionPtr& action)
	{
		clearActions();
		addAction(action);
	}

	enum EPositionRelativeToOption
	{
		E_SCREEN,
		E_ORIGIN
	};

	EPositionRelativeToOption positionInterpretation() const
	{
		return m_positionInterpretation;
	}
	virtual void setPositionInterpretation(EPositionRelativeToOption positionInterpretation) 
	{ 
		m_positionInterpretation = positionInterpretation; 
	}
	
	typedef boost::signals2::signal<void (const Point & /* old Position */, const Point & /* new Position */)> positionChangedSignalT;
	
	positionChangedSignalT & OnPositionChanged()
	{
		return onPositionChanged;
	}

private:
	float m_rotation;
	Point m_position;
	std::vector<ActionPtr> m_actions;
	
	positionChangedSignalT onPositionChanged;
	
	EPositionRelativeToOption m_positionInterpretation;
};

}

#endif
