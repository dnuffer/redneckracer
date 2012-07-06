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

#include "Label.hpp"
#include "Vector2.hpp"
#include "Size.hpp"

namespace engine
{
	using namespace blocxx;

	Label::Label(const TexturedFontPtr& font)
		: labelSize( 0.0, 0.0 ), m_font(font), scale(1.0f)
	{
	}

	void Label::setPosition(const Point& p)
	{
		Point oldPosition = this->position();
		Vector2 adjustment = p - oldPosition;

		//LOGD("Moving label by %f,%f", adjustment.x(), adjustment.y());

		Drawable::setPosition(p);

		for( Array<SpritePtr>::const_iterator sprite = m_sprites.begin(); sprite != m_sprites.end(); ++sprite )
		{
			(*sprite)->setPosition((*sprite)->position() + adjustment);
		}
	}
	
	void Label::setPositionInterpretation(Drawable::EPositionRelativeToOption positionInterpretation)
	{
		Drawable::setPositionInterpretation(positionInterpretation);
		
		for( Array<SpritePtr>::const_iterator sprite = m_sprites.begin(); sprite != m_sprites.end(); ++sprite )
		{
			(*sprite)->setPositionInterpretation(positionInterpretation);
		}
	}

	void Label::draw(const Rectangle& screen)
	{
		//		LOGD("Drawing label with text \"%s\"", m_text.c_str());
		for( Array<SpritePtr>::const_iterator sprite = m_sprites.begin(); sprite != m_sprites.end(); ++sprite )
		{
			(*sprite)->draw(screen);
		}
	}

	string Label::name() const
	{
		return "Label";
	}

	String Label::text() const
	{
		return m_text;
	}

	void Label::setText(const String& text)
	{
		Array<TexturedQuadPtr> quads;
		quads.reserve(text.length());

		int totalWidth = 0;
		int height = 0;

		// If anything goes wrong with text conversion, it will happen in this
		// loop before any changes are made to the actual label.
		for(unsigned i = 0; i < text.length(); ++i)
		{
			TexturedQuadPtr quad = m_font->getQuadForCharacter(text[i]);
			quad->scale(scale);
			quads.push_back(quad);

			Size quadSize = quad->size();
			totalWidth += quadSize.width();
			// All characters have the same height, so we can pick any one of them
			// and use its height.
			height = quadSize.height();
		}

		// Now that we know there are valid quads for each of the characters, we
		// can replace our current sprites.
		m_sprites.clear();
		m_sprites.reserve(quads.size());
		String nameText = "letter_";

		Point myPosition = this->position();
		float characterLeft = (-totalWidth / 2.0) + myPosition.x();
		float labelWidth = 0.0;
		float labelHeight = 0.0;
		
		for( unsigned i = 0; i < quads.size(); ++i )
		{
			TexturedQuadPtr& quad = quads[i];
			Size quadSize = quad->size();
			SpritePtr sprite = new Sprite(quad, nameText + text[i]);
			Point p(characterLeft + quadSize.width() / 2, myPosition.y());
			// LOGD("Setting character '%c' to be at %f,%f", text[i], p.x(), p.y());
			sprite->setPosition(p);
			sprite->setPositionInterpretation(positionInterpretation());
			m_sprites.push_back(sprite);
			characterLeft += quadSize.width();
			
			// update size of label based on sum of quads
			float currHeight = quadSize.height();
			labelWidth+=quadSize.width();

			if( currHeight > labelHeight )
				labelHeight = currHeight;
		}
		
		//store updated label size
		Size _labelSize( labelWidth, labelHeight );
		labelSize = _labelSize;

		m_text = text;
	}
	
	void Label::setScale(float newScale)
	{
		scale = newScale;
		setText(m_text);
	}

	Size Label::size() const
	{
		return labelSize;
	}
}
