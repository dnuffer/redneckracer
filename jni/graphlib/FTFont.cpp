/*
 * Part of GraphLib -- A framework for graphics production.
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
#include "FTFont.hpp"

#if defined(HAVE_FREETYPE)

#include "graphlib/Types.hpp"
#include <freetype/ftglyph.h>


namespace graphlib
{

	struct GlyphFreer
	{
		GlyphFreer(FT_Glyph& g) : glyph(g)
		{
		}
		~GlyphFreer()
		{
			FT_Done_Glyph(glyph);
		}
		FT_Glyph& glyph;
	};

	FTFont::FTFont()
		: m_ftlib(NULL)
		, m_fontFace(NULL)
		, m_ok(true)
	{
		FT_Init_FreeType(&m_ftlib);
	}

	FTFont::~FTFont()
	{
		if( m_fontFace )
		{
			FT_Done_Face(m_fontFace);
		}
		if( m_ftlib )
		{
			FT_Done_FreeType(m_ftlib);
			m_ftlib = NULL;
		}
	}

	bool FTFont::loadFont(const char* pathname)
	{
		if( !m_ftlib || !pathname )
		{
			return false;
		}

		int error = FT_New_Face(m_ftlib, pathname, 0, &m_fontFace);
		return error == 0;
	}

	bool FTFont::setFontSize(int pixelWidth, int pixelHeight)
	{
		if( !m_ftlib || !m_fontFace )
		{
			return false;
		}
		//		int error = FT_Set_Pixel_Sizes(m_fontFace, pixelWidth, pixelHeight);
		int error = FT_Set_Char_Size(m_fontFace, pixelWidth * 72, pixelHeight * 72, 72, 72);
		return error == 0;
	}


	namespace
	{
		void storePixelOpacity(RGBColor& c, unsigned char opacity)
		{
			c.set(opacity, opacity, opacity);
		}

		void storePixelOpacity(RGBAColor& c, unsigned char opacity)
		{
			c.set(RGBAColor::color_traits_type::max(),
				RGBAColor::color_traits_type::max(),
				RGBAColor::color_traits_type::max(),
				RGBAColor::color_traits_type::max() - opacity);
		}

		GenericRectangle<int> getCharacterRectangle(FT_Face fontFace, char c)
		{
			GenericRectangle<int> retval(0,0,0,0);
			if( !fontFace )
			{
				return retval;
			}
			int index = FT_Get_Char_Index(fontFace, c);
			int error = FT_Load_Glyph(fontFace, index, FT_LOAD_DEFAULT);

			if( error )
				return retval;

			// glyph metrics are in 26.6 format in this case, so the value represents
			// 1/64th of a pixel.
			const int unitsPerPixel = 64;

			retval.left = -fontFace->glyph->metrics.horiBearingX / unitsPerPixel;
			retval.right = (fontFace->glyph->metrics.horiAdvance - fontFace->glyph->metrics.horiBearingX) / unitsPerPixel;
			retval.top = fontFace->glyph->metrics.horiBearingY / unitsPerPixel;
			retval.bottom = (fontFace->glyph->metrics.horiBearingY - fontFace->glyph->metrics.height) / unitsPerPixel;

			return retval;
		}

		template <typename RasterType>
		bool genericDrawCharacter(char c, RasterType& img, int xoff, int yoff, FT_Face fontFace)
		{
			if( !fontFace )
			{
				return false;
			}
			GenericRectangle<int> charsize = getCharacterRectangle(fontFace, c);

			int index = FT_Get_Char_Index(fontFace, c);
			int error = FT_Load_Glyph(fontFace, index, FT_LOAD_DEFAULT);
			if( error) return false;

			error = FT_Render_Glyph(fontFace->glyph, FT_RENDER_MODE_NORMAL);
			if( error ) return false;

			const FT_Bitmap& bitmap = fontFace->glyph->bitmap;
			printf("The bitmap is %d rows and %d pixels wide\n", bitmap.rows, bitmap.width);

			//			for( int y = charsize.left; y < charsize.right; ++y )
			for( int y = 0; y < bitmap.rows; ++y )
			{
				int ypos = yoff + y;

				if( (ypos < 0) || (ypos >= int(img.getHeight())) )
				{
					printf("Y out of bounds: y=%d, ypos=%d, height=%d\n", y, ypos, img.getHeight());
					continue;
				}

				unsigned buffer_offset = y * bitmap.width;

				//				for( int x = charsize.bottom; x < charsize.top; ++x )
				for( int x = 0; x < bitmap.width; ++x )
				{
					int xpos = xoff + x;
					if( (xpos < 0) || (xpos >= int(img.getWidth())) )
					{
						printf("X out of bounds: x=%d, xpos=%d, width=%d\n", x, xpos, img.getWidth());
						continue;
					}

					unsigned char opacity = 0;

					if( (y >= 0) && (y < bitmap.rows) && (x >= 0) && (x < bitmap.width))
					{
						opacity = bitmap.buffer[buffer_offset + x];
					}

					//printf("Setting opacity of %d,%d to %d\n", xpos, ypos, opacity);

					storePixelOpacity(img(xpos, ypos), opacity);
				}
			}

			//			printf("Returning...\n");
			return true;
		}
	}

	GenericRectangle<int> FTFont::characterRectangle(char c) const
	{
		return getCharacterRectangle(m_fontFace, c);
	}

	bool FTFont::drawCharacter(char c, Image<unsigned char>& img, int x, int y) const
	{
		if( !m_ftlib || !m_fontFace )
		{
			return false;
		}
		return genericDrawCharacter(c, img, x, y, m_fontFace);
	}

	bool FTFont::drawCharacter(char c, AlphaImage<unsigned char>& img, int x, int y) const
	{
		if( !m_ftlib || !m_fontFace )
		{
			return false;
		}
		return genericDrawCharacter(c, img, x, y, m_fontFace);
	}
}

#endif
