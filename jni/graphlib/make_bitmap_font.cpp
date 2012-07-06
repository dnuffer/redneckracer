/*
 * Part of GraphLib
 * Copyright (C) 2009 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "graphlib/PPMLoader.hpp"
#include "graphlib/FTFont.hpp"
#include "graphlib/AlphaImage.hpp"
#include "graphlib/Types.hpp"
#include "graphlib/GenericRectangle.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Format.hpp"
#include <limits>
#include <iostream>
#include <fstream>

using namespace graphlib;
using namespace blocxx;

const int MinChar = 32;
const int MaxChar = 126; // inclusive

typedef Image<unsigned char> Img;

GenericRectangle<int> findMaximumCharacterRect(const FTFont& font)
{
	int m = std::numeric_limits<int>::max();
	GenericRectangle<int> retval(m, -m, -m, m);

	for( int i = MinChar; i <= MaxChar; ++i )
	{
		GenericRectangle<int> rect = font.characterRectangle(char(i));
		retval.left = std::min(retval.left, rect.left);
		retval.right = std::max(retval.right, rect.right);

		retval.top = std::max(retval.top, rect.top);
		retval.bottom = std::min(retval.bottom, rect.bottom);
	}

	return retval;
}

// A stupid ordering (not optimal packing) rendering function
bool renderFont(FTFont& font, Img& img, String& atlasData, const String& fontName)
{
	GenericRectangle<int> maxRect = findMaximumCharacterRect(font);

	printf("maxRect = (left: %d, right: %d, top: %d, bottom: %d)\n", maxRect.left, maxRect.right, maxRect.top, maxRect.bottom);

	const int hpad = 50;

	int y = hpad;
	int x = hpad;
	GenericRectangle<int> spaceRect = font.characterRectangle(' ');
	spaceRect.bottom = y + maxRect.height();
	spaceRect.top = y;
	spaceRect.right = x + spaceRect.width();
	spaceRect.left = x;

	x += spaceRect.width() + hpad;

	for( int c = MinChar; c <= MaxChar; ++c )
	{
		GenericRectangle<int> charRect = font.characterRectangle(char(c));
		if( charRect.height() != 0 )
		{
			if( (x + charRect.width()) >= int(img.getWidth()) )
			{
				y += maxRect.height() + hpad;
				x = hpad;
			}
			if( (y + charRect.height()) >= int(img.getHeight()) )
			{
				return false;
			}

			int putx = x;
			int puty = y + maxRect.height() - charRect.top;

			font.drawCharacter(char(c), img, putx, puty);
			atlasData += Format("quad: fonts/%1/%<2:03> %3 %4 %5 %6 1\n",
				fontName, c, x, y, charRect.width(), maxRect.height());

			x += charRect.width() + hpad;
		}
		else
		{
			atlasData += Format("quad: fonts/%1/%<2:03> %3 %4 %5 %6 1\n",
				fontName, c, spaceRect.left, spaceRect.top, spaceRect.width(), spaceRect.height());
		}
	}

	return true;
}

int main(int argc, const char** argv)
{
	if( argc <= 2 )
	{
		printf("Usage: make_bitmap_font <font file> <size>\n");
		return 1;
	}

	const char* fontName = argv[1];
	int size = atoi(argv[2]);

	FTFont font;
	if( !font.loadFont(fontName) )
	{
		printf("Failed to load the font \"%s\"\n", fontName);
		return 1;
	}

	if( !font.setFontSize(0, size) )
	{
		printf("Failed to set font size to %d\n", size);
		return 1;
	}

	String fontNameStr(fontName);
	String atlasName = fontNameStr.substring(fontNameStr.lastIndexOf('/') + 1);
	atlasName = atlasName.substring(0, atlasName.indexOf('.'));

	int atlasSize = 64;
	String atlas;
	bool rendered = false;
	Img img;
	do
	{
		atlasSize *= 2;
		img = Img(atlasSize, atlasSize);
		atlas = Format(
			"image: fonts/%1.png\n"
			"size: %2 %3\n"
			"group: %4\n",
			atlasName,
			img.getWidth(), img.getHeight(),
			atlasName);

		rendered = renderFont(font, img, atlas, atlasName);
	} while( !rendered );

	if( rendered )
	{
		savePPM(img, String(atlasName + ".ppm").c_str());
		std::ofstream o(String(atlasName + ".atlas").c_str());
		o << atlas.c_str();
	}

	return 0;
}
