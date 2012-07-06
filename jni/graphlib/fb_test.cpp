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
#include "graphlib/Types.hpp"
#include "XFrameBuffer.hpp"
#include "miniblocxx/Function.hpp"
#include <stdio.h>

#include "MaskedImage.hpp"
#include "AlphaImage.hpp"
#include "XPMLoader.hpp"
#include "PPMLoader.hpp"
#include "FTFont.hpp"

using graphlib::XFrameBuffer;

namespace
{
	XFrameBuffer buffer(400,400);

	int x_shift = 0;
	int y_shift = 0;

	/*
	  XPM data taken from wikipedia.  I swapped out the colors so that the background is transparent
	*/
	const char* BLARG_XPM[] = {
		"16 7 2 1",
		"* c #ffffff",
		". c #None",
		"**..*...........",
		"*.*.*...........",
		"**..*..**.**..**",
		"*.*.*.*.*.*..*.*",
		"**..*..**.*...**",
		"...............*",
		".............**."
	};
	const char* FEEP_PPM =
		"P3\n"
		"# feep.ppm\n"
		"4 4\n"
		"15\n"
		" 0  0  0    0  0  0    0  0  0   15  0 15\n"
		" 0  0  0    0 15  7    0  0  0    0  0  0\n"
		" 0  0  0    0  0  0    0 15  7    0  0  0\n"
		"15  0 15    0  0  0    0  0  0    0  0  0\n";

	graphlib::MaskedImage<unsigned char> WIKIPEDIA_IMAGE;
	graphlib::MaskedImage<unsigned char> FEEP;

	graphlib::AlphaImage<unsigned char> img_w;

#if defined(HAVE_FREETYPE)
	graphlib::FTFont font;
#endif
}

void update()
{
	const XFrameBuffer::BaseFrameBuffer::color_type green(0,255,0);
	const XFrameBuffer::BaseFrameBuffer::color_type blue(0,0,255);

	int width = buffer.getWidth();
	int height = buffer.getHeight();

	if( (width == 0) || (height == 0) )
	{
		return;
	}

	for(int y = 0; y < height; ++y)
	{
		int effective_y = (y - y_shift);
		int distance = 0;
		for(int x = 0; x < width; ++x)
		{
			int effective_x = (x - x_shift);
			//      if( (effective_y << 1) & (effective_x << 1) )
			if( effective_y & effective_x )
			{
				static int last_width = 0;
				static XFrameBuffer::BaseFrameBuffer::color_type* precalculated_color_gradient = NULL;

				if( (width != last_width) || !precalculated_color_gradient )
				{
					//      printf("Recalculating gradient\n");
					// Deleting NULL is safe!
					delete[] precalculated_color_gradient;

					last_width = width;
					precalculated_color_gradient = new XFrameBuffer::BaseFrameBuffer::color_type[width];

					const double fraction = 1.0 / 3.0;
					for( int dist = 0; dist < width; ++dist)
					{
						int value = int(fraction * 255 +
							((255 *(1 - fraction)) * dist / float(width)));
						precalculated_color_gradient[dist] = XFrameBuffer::BaseFrameBuffer::color_type(0, value, 0);
					}
					//      printf("Gradient calculated...\n");
				}

				//      printf("Setting pixel from gradient\n");
				buffer.setpixel_back(x,y,precalculated_color_gradient[distance]);
				++distance;

				//      setpixel_back(x,y,green);
			}
			else
			{
				buffer.setpixel_back(x,y,blue);
				distance = 0;
			}
			/*
			// Add a red 'fogging'.
			setpixel_back(x,y,manygames::rgbacolor<unsigned char,double>(128,0,0,0.5));
			*/

		}
	}
	y_shift -= 1;
	x_shift -= 2;

	unsigned cx = buffer.getWidth() / 2;
	unsigned cy = buffer.getHeight() / 2;

	// Draw the wikipedia image in the center of the screen.
	buffer.bg_draw_image(WIKIPEDIA_IMAGE,
		cx - WIKIPEDIA_IMAGE.getWidth() / 2,
		cy - WIKIPEDIA_IMAGE.getHeight() / 2);

	buffer.bg_draw_image(FEEP,
		cx - FEEP.getWidth() / 2,
		cy + WIKIPEDIA_IMAGE.getHeight() / 2);

	buffer.bg_draw_image(img_w,
		cx - img_w.getWidth() / 2,
		cy - WIKIPEDIA_IMAGE.getHeight() / 2 - img_w.getHeight());

	buffer.flip();
}

bool mouse_moved(int x, int y, int dx, int dy)
{
	unsigned buttons_down = buffer.get_button_state() & graphlib::MouseInput::mouse_button_mask;

	if( buttons_down != 0 )
	{
		x_shift += dx;
		y_shift += dy;

		static timeval last_time = {0,0};
		const unsigned max_fps = 15;
		const int min_us_wait = int(1000000 / double(max_fps));

		timeval t;
		gettimeofday(&t, NULL);

		long us_diff = (t.tv_usec - last_time.tv_usec);

		if( t.tv_sec > last_time.tv_sec )
		{
			us_diff += 1000000;
		}

		if( us_diff > min_us_wait )
		{
			// printf("td=%ld, min=%d\n", us_diff, min_us_wait);

			last_time.tv_sec = t.tv_sec;
			last_time.tv_usec = t.tv_usec;

			buffer.update();
			buffer.draw();
		}
	}
	return true;
}

struct Clicker
{
	bool operator()(int,int,unsigned,unsigned) const
	{
		printf("Clicker::operator() const: That was a click!\n");
		return true;
	}
};

bool drag(int,int,int,int,unsigned)
{
	printf("That was a drag!\n");
	return true;
}

struct KeyHandler
{
	bool key_down(unsigned key, unsigned mods) const
	{
		printf("KeyHandler::key_down(0x%06x,0x%06x)==%s -- %d keys are now down\n", key, mods,
			buffer.keyname(key, mods, false).c_str(),
			buffer.num_keys_down());

#if defined(HAVE_FREETYPE)
		//		if( isprint(key) )
		{
			graphlib::GenericRectangle<int> sizes = font.characterRectangle(char(key));
			img_w = graphlib::AlphaImage<unsigned char>();
			img_w.resize(sizes.width(), sizes.height(), 0, 0, graphlib::RGBAColor(0,0,0,255));
			if( !font.drawCharacter(char(key), img_w, 0, 0) )
			{
				printf("Failed to draw character %c\n", char(key));
			}

			update();
			buffer.draw();
		}
#endif

		return true;
	}
	bool key_up(unsigned key, unsigned press_mods, unsigned release_mods)
	{
		printf("KeyHandler::key_up(0x%06x,0x%06x,0x%06x)==%s -- %d keys are now down\n", key, press_mods, release_mods,
			buffer.keyname(key, release_mods, false).c_str(),
			buffer.num_keys_down());

		// Quit when they let up on Q or escape.
		if( (key == 'q') || (key == 'Q') || (key == graphlib::KeyboardInput::KEY_escape) )
		{
			buffer.quitWindow();
		}

		return true;
	}
};

int main(int argc, const char** argv)
{
#if defined(HAVE_FREETYPE)
	{
		const int fsize = 10;
		//if( font.loadFont("/usr/share/fonts/truetype/msttcorefonts/cour.ttf") )
		if( font.loadFont("bumpyroad.ttf") )
		{
			printf("setFontSize returned %d\n", font.setFontSize(0, fsize));
			graphlib::GenericRectangle<int> sizes = font.characterRectangle('Q');
			img_w.resize(sizes.width(), sizes.height(), 0, 0, graphlib::RGBAColor(0,0,0,255));
			printf("Sizes of 'Q'=(%d,%d),(%d,%d)\n", sizes.left, sizes.top, sizes.right, sizes.bottom);
			printf("Resized image to %d,%d\n", img_w.getWidth(), img_w.getHeight());
			printf("drawCharacter returned %d\n", font.drawCharacter('Q', img_w, 0, 0));
		}
		else
		{
			printf("Failed to load font\n");
		}
	}
#endif

	printf("Loading xpm returned %d\n", graphlib::loadXPM(BLARG_XPM, WIKIPEDIA_IMAGE));
	printf("The wikipedia image is\n%s\n", WIKIPEDIA_IMAGE.toString().c_str());
	{
		graphlib::Image<unsigned char> feep;
		graphlib::RGBColor black(0,0,0);
		printf("Loading ppm returned %d\n", graphlib::loadPPM(FEEP_PPM, strlen(FEEP_PPM), feep));
		FEEP = graphlib::MaskedImage<unsigned char>(feep, black);
	}
	printf("The ppm image is\n%s\n", FEEP.toString().c_str());

	// Make it big enough to see.
	WIKIPEDIA_IMAGE = scaleImage(WIKIPEDIA_IMAGE, 6);
	FEEP = scaleImage(FEEP, 6);

	printf("Saving the ppm returned %d\n", savePPM(FEEP, "feep.ppm"));

	buffer.disableRepeat();
	Clicker clicker;
	KeyHandler keys;
	// Functor.  Note that the const operator() can currently only be applied to
	// const objects.  I need to find some way to work around this.
	buffer.mouse_clicked.connect(const_cast<const Clicker&>(clicker));
	// Free functions
	buffer.buffer_update.connect(&update);
	buffer.mouse_moved.connect(&mouse_moved);
	buffer.mouse_dragged.connect(&drag);
	buffer.buffer_update.connect(&update);
	// Member function: const
	buffer.key_pressed.connect(blocxx::memfun(keys, &KeyHandler::key_down));
	// Member function: non-const
	buffer.key_released.connect(blocxx::memfun(keys, &KeyHandler::key_up));

	// Hit esc or "q" to quit this.
	buffer.runWindow();

	return 0;
}
