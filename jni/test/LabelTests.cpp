/*
 * LabelTests.cpp
 *
 *  Created on: Jun 24, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "engine/Resource.hpp"
#include "engine/Label.hpp"
#include <iostream>

using namespace engine;
using namespace blocxx;

class TexturedFontMock : public TexturedFont
{
public:
	TexturedFontMock():TexturedFont(new TextureLibrary(), "Mock") {}
	virtual ~TexturedFontMock() {}
	
	virtual TexturedQuadPtr getQuadForCharacter(char c) const
		{ return new TexturedQuad(0,0,5,5,new Texture(),5,5); }
};

AUTO_UNIT_TEST(LabelSetTextBlank)
{
	// Arrange
	Label label(new TexturedFontMock());
	
	// Act
	label.setText("");
	
	// Assert
	unitAssert(label.text() == "");
}

AUTO_UNIT_TEST(LabelSetTextLetters)
{
	// Arrange
	Label label(new TexturedFontMock());
	
	// Act
	label.setText("Word");
	
	// Assert
	unitAssert(label.text() == "Word");
}

AUTO_UNIT_TEST(LabelSetTextSymbols)
{
	// Arrange
	Label label(new TexturedFontMock());
	
	// Act
	label.setText("!@#$%^&*()_+");
	
	// Assert
	unitAssert(label.text() == "!@#$%^&*()_+");
}

AUTO_UNIT_TEST(LabelSetTextMixed)
{
	// Arrange
	Label label(new TexturedFontMock());
	
	// Act
	label.setText("Ma'am, got any $$$?  Stick 'em UP!!!!");
	
	// Assert
	unitAssert(label.text() == "Ma'am, got any $$$?  Stick 'em UP!!!!");
}

AUTO_UNIT_TEST(LabelSetPosition)
{
	// Arrange
	Label label(new TexturedFontMock());
	
	// Act
	label.setPosition(Point(1,1));
	
	// Assert
	unitAssert(label.position() == Point(1,1));
}

AUTO_UNIT_TEST(LabelSizeAndScaleLarger)
{
	// Arrange
	Label label(new TexturedFontMock());
	Size original(label.size());
	
	// Act
	label.setScale(2.0f);
	
	// Assert
	unitAssert((label.size().width() == original.width() * 2) &&
			(label.size().height() == original.height() * 2));
}

AUTO_UNIT_TEST(LabelSizeAndScaleSmaller)
{
	// Arrange
	Label label(new TexturedFontMock());
	Size original(label.size());
	
	// Act
	label.setScale(0.5f);
	
	// Assert
	unitAssert((label.size().width() == original.width() / 2) &&
			   (label.size().height() == original.height() / 2));
}
