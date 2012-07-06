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

#ifndef _engine_TOUCHBUTTON_HPP_
#define _engine_TOUCHBUTTON_HPP_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "Drawable.hpp"
#include "Sprite.hpp"
#include "TouchHandler.hpp"
#include <map>
#include <tr1/functional>


namespace engine
{	
	class TouchButton: public virtual Boundable, public Drawable, public TouchHandler
	{
	public:
		TouchButton(const vector<TexturedQuadPtr>& texturedQuads);
		virtual void draw(const Rectangle& screen);
		virtual std::string name() const;
		virtual void setPosition(const Point& p);
		virtual void setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation);
		virtual ~TouchButton(){};
		void updateBoundingRect();
		bool checkHit(float screenX, float screenY);

	private:
		int statesCount;
		vector<SpritePtr> buttonSprites;

	};
}

#endif // __TOUCHBUTTON_HPP__
