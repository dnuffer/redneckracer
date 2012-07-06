/*
 * $Id: FTFont.hpp$
 *
 * Part of "Graphlib"
 * Copyright (C) 2010 Kevin Harris
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

#if       !defined(GRAPHLIB_FTFONT_HPP)
#define            GRAPHLIB_FTFONT_HPP

#if defined(HAVE_FREETYPE)
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Image.hpp"
#include "AlphaImage.hpp"

namespace graphlib
{
	class FTFont
	{
	public:
		FTFont();
		~FTFont();

		// Load a font from the supplied path
		bool loadFont(const char* pathname);

		// Set the pixel size of the current font
		bool setFontSize(int pixelWidth, int pixelHeight);

		GenericRectangle<int> characterRectangle(char c) const;

		bool drawCharacter(char c, Image<unsigned char>& img, int x, int y) const;
		bool drawCharacter(char c, AlphaImage<unsigned char>& img, int x, int y) const;
	private:
		FT_Library m_ftlib;
		FT_Face m_fontFace;
		bool m_ok;
	};
} // namespace graphlib

#endif

#endif /* !defined(GRAPHLIB_FTFONT_HPP) */

