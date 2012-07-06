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

#ifndef LABEL_HPP_INCLUDED
#define LABEL_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "Sprite.hpp"
#include "TexturedFont.hpp"
#include "Size.hpp"

namespace engine
{
	class Label : public Drawable
	{
	public:
		Label(const TexturedFontPtr& font);

		virtual void draw(const Rectangle& screen);
		virtual std::string name() const;
		virtual void setPosition(const Point& p);
		virtual void setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation);

		void setText(const String& text);
		void setScale(float scale);
		String text() const;

		Size size() const;
		
	private:
		blocxx::String m_text;
		Size labelSize;
		TexturedFontPtr m_font;
		float scale;
		Array<SpritePtr> m_sprites;
	};
}

#endif
