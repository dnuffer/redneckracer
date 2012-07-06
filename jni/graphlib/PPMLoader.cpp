/*
 * Part of "Graphlib"
 * Copyright (C) 2003 Kevin Harris
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
#include "PPMLoader.hpp"
#include "TextUtils.hpp"

#include <stdio.h>


namespace graphlib
{
	namespace
	{
		// PPM files are supposed to have non-linear colors with a BT.709 gamma transfer function:
		//
		// E709(L) = { 4.5 * L                     |  0 <= L <= 0.018
		//           { 1.099 * L^(1/2.2) - 0.099   |  0.018 < L < 1.0
		//
		// Where the resulting correction is 255 * E709(N / 255)

		// While this works perfectly, I'm ignoring it because it seems
		// most new ppms expect to be in the RGB color space instead of
		// BT.709

		/*
		const unsigned char PPMGammaTable[256] = {
			0, 4, 9, 13, 18, 21, 25, 29, 32, 36, 39, 41, 44, 47, 49, 52,
			54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 75, 77, 79, 80, 82,
			83, 85, 86, 88, 89, 91, 92, 94, 95, 96, 98, 99, 100, 102, 103, 104,
			105, 107, 108, 109, 110, 111, 113, 114, 115, 116, 117, 118, 119, 121, 122, 123,
			124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
			140, 141, 142, 143, 143, 144, 145, 146, 147, 148, 149, 150, 151, 151, 152, 153,
			154, 155, 156, 157, 157, 158, 159, 160, 161, 161, 162, 163, 164, 165, 165, 166,
			167, 168, 169, 169, 170, 171, 172, 172, 173, 174, 175, 175, 176, 177, 178, 178,
			179, 180, 181, 181, 182, 183, 183, 184, 185, 186, 186, 187, 188, 188, 189, 190,
			190, 191, 192, 192, 193, 194, 194, 195, 196, 196, 197, 198, 198, 199, 200, 200,
			201, 202, 202, 203, 204, 204, 205, 205, 206, 207, 207, 208, 209, 209, 210, 210,
			211, 212, 212, 213, 213, 214, 215, 215, 216, 216, 217, 218, 218, 219, 219, 220,
			221, 221, 222, 222, 223, 223, 224, 225, 225, 226, 226, 227, 227, 228, 229, 229,
			230, 230, 231, 231, 232, 232, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238,
			238, 239, 240, 240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246,
			247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 255
		};
		*/

		void skipComments(const char*& data, unsigned& len)
		{
			bool have_comment = true;
			while(have_comment)
			{
				have_comment = false;
				skipSpaces(data, len);
				if( (len > 0) && data[0] == '#' )
				{
					unsigned newline = lengthToNewline(data, len);
					data += newline;
					len -= newline;
					have_comment = true;
				}
			}
		}

		bool readHeader(const char*& data, unsigned& len, unsigned& width, unsigned& height, unsigned& maxColor)
		{
			if( len < 2 )
			{
				return false;
			}
			const char* start = data;

			if( data[0] == 'P' )
			{
				if( data[1] == '6' || data[1] == '3' )
				{
					data += 2;
					// Newlines exist before the width/height
					skipComments(data, len);
					bool ok = true;
					// These two are on a single line.
					ok = readInteger(data, len, width) || ok;
					ok = readInteger(data, len, height) || ok;
					// New line for this one.
					skipComments(data, len);
					ok = readInteger(data, len, maxColor) || ok;

					len += data - start;
					return ok;
				}
			}
			return false;
		}

		unsigned readBinaryColorEntry(const char*& data, unsigned maxColor)
		{
			unsigned value = *reinterpret_cast<const unsigned char*>(data++);
			if( maxColor > 256 )
			{
				value = (value << 8) + *reinterpret_cast<const unsigned char*>(data++);
			}
			return value;
		}

		bool readBinaryColor(const char*& data, unsigned& len, GenericRGBColor<unsigned char>& color, unsigned maxColor)
		{
			unsigned bytesNeeded = 3u;
			if( maxColor > 256 )
			{
				bytesNeeded = 6u;
			}
			if( len < bytesNeeded )
			{
				return false;
			}

			// THe max color is supposed to be white.  Normalize the colors.
			unsigned char r = 255 * readBinaryColorEntry(data, maxColor) / maxColor;
			unsigned char g = 255 * readBinaryColorEntry(data, maxColor) / maxColor;
			unsigned char b = 255 * readBinaryColorEntry(data, maxColor) / maxColor;

			if( (r >= 256) || (g >= 256) || (b >= 256) )
			{
				return false;
			}

			// Apply the gamma function.
			//	color.set(PPMGammaTable[r], PPMGammaTable[g], PPMGammaTable[b]);
			color.set(r, g, b);

			len -= bytesNeeded;

			return true;
		}

		bool readColor(const char*& data, unsigned& len, GenericRGBColor<unsigned char>& color, unsigned maxColor)
		{
			unsigned r, g, b;
			bool ok = true;
			ok = readInteger(data, len, r) || ok;
			ok = readInteger(data, len, g) || ok;
			ok = readInteger(data, len, b) || ok;
			if( !ok )
			{
				return false;
			}

			r = 255 * r / maxColor;
			g = 255 * g / maxColor;
			b = 255 * b / maxColor;

			if( (r >= 256) || (g >= 256) || (b >= 256) )
			{
				return false;
			}
			//	color.set(PPMGammaTable[r], PPMGammaTable[g], PPMGammaTable[b]);
			color.set(r, g, b);
			return true;
		}

		bool loadP3(const char* data, unsigned len, Image<unsigned char>& image)
		{
			unsigned width, height, maxColor;
			if( !readHeader(data, len, width, height, maxColor) )
			{
				return false;
			}

			image.resize(width, height);
			for( unsigned y = 0; y < height; ++y )
			{
				for( unsigned x = 0; x < width; ++x )
				{
					skipComments(data, len);
					if( !readColor(data, len, image(x,y), maxColor) )
					{
						return false;
					}
				}
			}
			return true;
		}

		bool loadP6(const char* data, unsigned len, Image<unsigned char>& image)
		{
			unsigned width, height, maxColor;
			if( !readHeader(data, len, width, height, maxColor) )
			{
				return false;
			}
			image.resize(width, height);
			for( unsigned y = 0; y < height; ++y )
			{
				for( unsigned x = 0; x < width; ++x )
				{
					if( !readBinaryColor(data, len, image(x,y), maxColor) )
					{
						return false;
					}
				}
			}
			return true;
		}
	}

	bool loadPPM(const char* data, unsigned len, Image<unsigned char>& image)
	{
		if( !data || !*data || !*(data+1) || len < 2 )
		{
			return false;
		}
		if( data[0] == 'P' )
		{
			char version = data[1];
			if( version == '3' )
			{
				return loadP3(data, len, image);
			}
			else if( version == '6' )
			{
				return loadP6(data, len, image);
			}
		}
		return false;
	}

	bool savePPM_P3(const Image<unsigned char>& image, const char* filename)
	{
		FILE* f = fopen(filename, "wb");

		if( !f )
		{
			return false;
		}

		// Write the header
		fprintf(f, "P3\n");
		fprintf(f, "# Saved as %s\n", filename);
		fprintf(f, "%d %d\n", image.getWidth(), image.getHeight());
		fprintf(f, "255\n"); // max color

		unsigned written = 0;
		const unsigned charsPerPixel = 13; // "%3d %3d %3d\t\t"
		const unsigned maxCharsPerLine = 70; // This sucks, but it has been documented that only 70 chars per line are allowed.
		const unsigned pixelsPerLine = maxCharsPerLine / (charsPerPixel + 1);

		for( unsigned y = 0; y < image.getHeight(); ++y )
		{
			for( unsigned x = 0; x < image.getWidth(); ++x )
			{
				const GenericRGBColor<unsigned char>& c = image(x,y);
				fprintf(f, "%3d %3d %3d", int(c.r()), int(c.g()), int(c.b()));
				++written;

				if( (written % pixelsPerLine) == 0 )
				{
					fprintf(f, "\n");
				}
				else
				{
					fprintf(f, "\t\t");
				}
			}
		}
		fprintf(f, "\n");

        fclose(f);
		return true;
	}

	bool savePPM(const Image<unsigned char>& image, const char* filename)
	{
		FILE* f = fopen(filename, "wb");

		if( !f )
		{
			return false;
		}

		// Write the header
		fprintf(f, "P6\n");
		fprintf(f, "# Saved as %s\n", filename);
		fprintf(f, "%d %d\n", image.getWidth(), image.getHeight());
		fprintf(f, "255\n"); // max color

		for( unsigned y = 0; y < image.getHeight(); ++y )
		{
			for( unsigned x = 0; x < image.getWidth(); ++x )
			{
				const GenericRGBColor<unsigned char>& c = image(x,y);
				fprintf(f, "%c%c%c", c.r(), c.g(), c.b());
			}
		}
		fprintf(f, "\n");

        fclose(f);
		return true;
	}
}
