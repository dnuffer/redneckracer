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

#include "TouchButton.hpp"
#include "GL.hpp"

namespace engine
{
	TouchButton::TouchButton(const vector<TexturedQuadPtr>& texturedQuads)
		: Boundable((*texturedQuads[0]).size())
		, statesCount(texturedQuads.size())
	{
		buttonSprites.reserve(texturedQuads.size());

		for (vector<TexturedQuadPtr>::const_iterator tQuad = texturedQuads.begin(); tQuad != texturedQuads.end(); ++tQuad)
		{
			buttonSprites.push_back(new Sprite(*tQuad));
		}
	}

	void TouchButton::setPosition(const Point& p)
		{
			Drawable::setPosition(p);
			for (vector<SpritePtr>::const_iterator sprite = buttonSprites.begin(); sprite != buttonSprites.end(); ++sprite)
			{
				(*sprite)->setPosition(p);
			}
			updateBoundingRect();
		}

		void TouchButton::setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation)
		{
			Drawable::setPositionInterpretation(positionInterpretation);
			for (vector<SpritePtr>::const_iterator sprite = buttonSprites.begin(); sprite!=buttonSprites.end(); ++sprite)
			{
				(*sprite)->setPositionInterpretation(positionInterpretation);
			}
		}

		void TouchButton::draw(const Rectangle& screen)
		{
			buttonSprites[getState()]->draw(screen);
		}

		string TouchButton::name() const
		{
			return "TouchButton";
		}

		void TouchButton::updateBoundingRect()
		{
			LOGD("button state=%#x, #sprites=%zu", (int)getState(), buttonSprites.size());
			setBoundingRect(buttonSprites[getState()]->getBoundingRect());

			LOGD("TouchButton: BoundingRect=%f,%f,%f,%f", getBoundingRect().left, getBoundingRect().right, getBoundingRect().top, getBoundingRect().bottom);
		}

		bool TouchButton::checkHit(float screenX, float screenY)
		{
			const Rectangle& bound = getBoundingRect();
			if(bound.left   > screenX) return false;
			if(bound.right  < screenX) return false;
			if(bound.top    < screenY) return false;
			if(bound.bottom > screenY) return false;
			return true;
		}
}
