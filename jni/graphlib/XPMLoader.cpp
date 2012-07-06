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
#include "XPMLoader.hpp"
#include "TextUtils.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/StringBuffer.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/stl/pair.hpp"
#include <ctype.h>
//#include "stdio.h"

// FIXME! Use something other than std::map
#include <map>

namespace graphlib
{
	using blocxx::String;

	namespace
	{
		typedef blocxx::pair<String, GenericRGBColor<unsigned char> > NamedColor;
		typedef std::map<String, GenericRGBColor<unsigned char> > ColorNameMap;
		typedef blocxx::Array<String> NameList;

		bool parseColorDefinition(
			const char*& line,
			unsigned chars_per_pixel,
			NamedColor& color
		)
		{
			bool retval = false;
			if( line && *line )
			{
				// Eat up whitespace
				skipSpaces(line);

				color.first = String(line, chars_per_pixel);

				//printf("color name=\"%s\"\n", color.first.c_str());
				line += chars_per_pixel;

				// Eat up whitespace
				skipSpaces(line);

				// Make sure it is a color.
				if( *line != 'c' )
				{
					//printf("Not a color:\n%s\n", line);
					return retval;
				}
				++line;

				// Eat up whitespace
				skipSpaces(line);

				if( *line == '#' )
				{
					++line;
					// A color...
					unsigned lineEnd = lengthToNewline(line);
					String colorData(line, lineEnd);
					colorData.trim();

					// FIXME! There may be a different xpm format that has
					// an alpha component...
					unsigned bytesPerComponent = colorData.length() / 3;
					unsigned long r = dehexify(line + 0 * bytesPerComponent, bytesPerComponent);
					unsigned long g = dehexify(line + 1 * bytesPerComponent, bytesPerComponent);
					unsigned long b = dehexify(line + 2 * bytesPerComponent, bytesPerComponent);

					//printf("bytesPerComponent=%d\n", bytesPerComponent);
					//printf("r=%ld, g=%ld, b=%ld\n", r, g, b);

					// The normal case...
					if( bytesPerComponent == 2 )
					{
						color.second.set(r,g,b);
						retval = true;
					}
					// Wider colors
					else if( bytesPerComponent == 4 )
					{
						// Convert down.  The loss of precision sucks, but this simplifies everything.
						color.second.set(r / 0xFF, g / 0xFF, b / 0xFF);
						retval = true;
					}
					else
					{
						retval = false;
					}
					line += lineEnd;
				}
				else if( String(line, lengthToNewline(line)).trim() == String("None") )
				{
					retval = false;
				}
			}

			return retval;
		}

		bool parseHeaderLine(const char*& line, unsigned& width, unsigned& height, unsigned& numColors, unsigned& charsPerPixel)
		{
			skipSpaces(line);
			if( *line == '!' )
			{
				// Assume it is some xpm2 format.
				line += lengthToNewline(line);
			}
			bool result = true;
			result = readInteger(line, width) && result;
			result = readInteger(line, height) && result;
			result = readInteger(line, numColors) && result;
			result = readInteger(line, charsPerPixel) && result;

			return result;
		}

		bool loadColorMap(const char*& data, unsigned numColors, unsigned charsPerPixel, ColorNameMap& colormap, NameList& noncolors)
		{
			if( !data || !*data )
			{
				return false;
			}

			for( unsigned color = 0; color < numColors; ++color )
			{
				NamedColor current;
				if( parseColorDefinition(data, charsPerPixel, current) )
				{
					colormap[current.first] = current.second;
					//printf("Parsed color (%d) \"%s\"=%s\n", color, current.first.c_str(), toString(current.second).c_str());
				}
				else if( !*data )
				{
					//printf("Hit end of input looking for color def\n");
					return false;
				}
				else // not a color, but we aren't done parsing -- could be transparent
				{
					noncolors.push_back(current.first);
				}
			}

			return true;
		}
	}

	bool loadXPM(const char* data, MaskedImage<unsigned char>& image)
	{
		if( !data || !*data )
		{
			//printf("No data\n");
			return false;
		}
		// Read the header
		unsigned width, height, numColors, charsPerPixel;
		if( !parseHeaderLine(data, width, height, numColors, charsPerPixel) )
		{
			//printf("Bad header\n");
			return false;
		}

		// Read the palette
		ColorNameMap colormap;
		NameList noncolors;
		if( !loadColorMap(data, numColors, charsPerPixel, colormap, noncolors) )
		{
			//printf("Bad color map\n");
			return false;
		}

		//printf("width=%d, height=%d, #colors=%d, pixelwidth=%d\n", width, height, numColors, charsPerPixel);

		// Read the image data
		Image<unsigned char>::color_type black(0,0,0);
		image.resize(width, height);


		for( unsigned y = 0; y < height; ++y )
		{
			skipSpaces(data);
			unsigned endOfLine = lengthToNewline(data);
			if( endOfLine < (width * charsPerPixel) )
			{
				//printf("Short line.  Expected %d, have %d\nRemaining text:\n%s", width * charsPerPixel, endOfLine, data);
				return false;
			}

			for( unsigned x = 0; x < width; ++x )
			{
				String name(data + x * charsPerPixel, charsPerPixel);
				ColorNameMap::const_iterator it = colormap.find(name);
				if( it != colormap.end() )
				{
					image(x,y) = it->second;
					image.mask(x,y) = true;
				}
				else if( noncolors.find(name) != noncolors.end() )
				{
					image(x,y) = black;
					image.mask(x,y) = false;
				}
				else
				{
					//printf("Garbage pixel at %d,%d", x, y);
					// Non-color garbage.
					return false;
				}
			}
			data += endOfLine;
		}

		//printf("Successfully loaded %dx%d image", width, height);
		return true;
	}

	bool loadXPM(const char** data, MaskedImage<unsigned char>& image)
	{
		if( !data || !*data )
		{
			//printf("Invalid at the start\n");
			return false;
		}

		// Just convert it to have newlines and forward it to the other loading function.
		blocxx::StringBuffer sb;

		sb.append("! XPM2\n");
		for(; *data; ++data )
		{
			sb.append(*data);
			sb.append('\n');
		}
		return loadXPM(sb.c_str(), image);
	}
}
