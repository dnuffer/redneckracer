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
#include "XFrameBuffer.hpp"
#include <stdlib.h> // for malloc
#include <stdio.h>
#include <cassert>

namespace graphlib
{
	namespace // anonymous
	{
		static unsigned keyboard_key_translate(unsigned key)
		{
			// FIXME! Do whatever translation is required.
			return key;
		}

		static unsigned keyboard_state_translate(unsigned state)
		{
			unsigned translated_state = 0;

			if( state & ShiftMask )
			{
				state &= ~ShiftMask;
				translated_state |= KeyboardInput::keyboard_shift;
			}
			if( state & ControlMask )
			{
				state &= ~ControlMask;
				translated_state |= KeyboardInput::keyboard_control;
			}
			if( state & LockMask )
			{
				state &= ~LockMask;
				translated_state |= KeyboardInput::keyboard_capslock;
			}
			if( state & Mod1Mask )
			{
				state &= ~Mod1Mask;
				translated_state |= KeyboardInput::keyboard_alt;
			}
			if( state & Mod2Mask )
			{
				state &= ~Mod2Mask;
				translated_state |= KeyboardInput::keyboard_numlock;
			}
			if( state & Mod3Mask )
			{
				state &= ~Mod3Mask;
				translated_state |= KeyboardInput::keyboard_mod3;
			}
			if( state & Mod4Mask )
			{
				state &= ~Mod4Mask;
				translated_state |= KeyboardInput::keyboard_super;
			}
			if( state & Mod5Mask )
			{
				state &= ~Mod5Mask;
				translated_state |= KeyboardInput::keyboard_mod5;
			}

			// FIXME! Do whatever remaining translation is required.
			return (state << 16) | translated_state;
		}

		unsigned translate_x_state(int button, unsigned state, unsigned previous_state)
		{
			// Note: I have put assertions in here, even for my own constants, so that
			// this function will detect changes (and let me know) if I change my
			// constants, or the X constants change (unlikely).
			unsigned result = 0;

			// Translate the buttons....
			assert( (MouseInput::mouse_button_1 == (1 << 0)) &&
				(MouseInput::mouse_button_2 == (1 << 1)) &&
				(MouseInput::mouse_button_3 == (1 << 2)) &&
				(MouseInput::mouse_button_4 == (1 << 3)) &&
				(MouseInput::mouse_button_5 == (1 << 4)) );

			if( button > 0 )    // press
			{
				result |= previous_state & MouseInput::mouse_button_mask;
				result |= 1 << (button - 1);
			}
			else if(button < 0) // release
			{
				result |= previous_state & MouseInput::mouse_button_mask;
				result &= ~(1 << (-button - 1));
			}
			else
			{
				result |= previous_state & MouseInput::mouse_button_mask;
			}

			if( state & ShiftMask )
			{
				result |= MouseInput::mouse_shift_down;
			}
			if( state & LockMask )
			{
				result |= MouseInput::mouse_capslock_on;
			}

			assert( (ControlMask == (1 << 2)) &&
				(Mod1Mask    == (1 << 3)) &&
				(Mod2Mask    == (1 << 4)) &&
				(Mod3Mask    == (1 << 5)) &&
				(Mod4Mask    == (1 << 6)) &&
				(Mod5Mask    == (1 << 7)) );

			assert( (MouseInput::mouse_control_down == (1 << 6)) &&
				(MouseInput::mouse_alt_down     == (1 << 7)) &&
				(MouseInput::mouse_numlock_on   == (1 << 8)) &&
				(MouseInput::mouse_mod3_down    == (1 << 9)) &&
				(MouseInput::mouse_super_down   == (1 << 10)) &&
				(MouseInput::mouse_mod5_down    == (1 << 11)) );

			result |= ((state >> 2) & 0x3F) << 6;

			return result;
		}
	}

	XFrameBuffer::XFrameBuffer(int width, int height, Display* disp, int scr)
		: display(disp)
		, screen(scr)
		, img(NULL)
		, image_buffer(NULL)
		, drawable(false)
	{
		BaseFrameBuffer::resize(width, height);

		initialize();
	}

	XFrameBuffer::XFrameBuffer(const XFrameBuffer& old)
		: BaseClassType(old)
		, display(NULL)
		, screen(-1)
		, img(NULL)
		, image_buffer(NULL)
		, drawable(false)
	{
		initialize();
	}

	void XFrameBuffer::initialize()
	{

		if( !display )
		{
			display = XOpenDisplay((char*)0);
			if( display )
			{
				screen = DefaultScreen(display);

				display_created = true;
			}
		}
		if( display )
		{
			if( screen < 0 )
			{
				screen = DefaultScreen(display);
			}

			unsigned long black, white;

			black = BlackPixel(display, screen);
			white = WhitePixel(display, screen);

			unsigned long border_color = black;
			int border_width = 5;
			unsigned long background = white;

			window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0,
				getWidth(), getHeight(), border_width, border_color,
				background);

			XSetStandardProperties(display, window,
				"Default window title",
				"Minimized window title",
				None,
				NULL,0,NULL); // Hints, params (argc,argv), etc.

			XSelectInput(display, window,
				ExposureMask |
				ButtonPressMask | ButtonReleaseMask |
				KeyPressMask | KeyReleaseMask |
				PointerMotionMask |
				Button1MotionMask | Button2MotionMask | Button3MotionMask |
				Button4MotionMask | Button5MotionMask |
				FocusChangeMask  // Detects FocusIn, FocusOut
			);
			// SubstructureNotifyMask ???? nothing?
			// OwnerGrabButtonMask ???? Nothing?
			// PropertyChangeMask // Detects PropertyNotify,
			// EnterWindowMask | LeaveWindowMask   // Detects mouse in/out of window
			// StructureNotifyMask | // Detects configure, reparent, map
			// VisibilityChangeMask | // Detects Visibilty

			gc = XCreateGC(display, window, 0, 0);
			XSetBackground(display, gc, black);
			XSetForeground(display, gc, white);
			XClearWindow(display, window);
			XMapRaised(display, window);
		}
		enableRepeat();
	}

	XFrameBuffer::~XFrameBuffer()
	{
		if( img )
		{
			XDestroyImage(img);
			img = NULL;
		}

		if( display )
		{
			XFreeGC(display, gc);
			XDestroyWindow(display, window);

			XAutoRepeatOn(display);

			if( display_created )
			{
				XCloseDisplay(display);
			}
		}
	} // ~XFrameBuffer()

	void XFrameBuffer::draw()
	{
		if( display && img && drawable )
		{
			unsigned width = getWidth();
			unsigned height = getHeight();
			unsigned* image_buf2 = reinterpret_cast<unsigned*>(image_buffer);
			const unsigned char* image_data = m_foreground->reinterpret<unsigned char*>();
			for(unsigned y = 0; y < height; ++y)
			{
				unsigned y_offset = y * width;
				for(unsigned x = 0; x < width; ++x)
				{
					unsigned linear = y_offset + x;
					image_buf2[linear] = (
						unsigned(image_data[linear*3 + 0]) << 16 |
						unsigned(image_data[linear*3 + 1]) << 8 |
						unsigned(image_data[linear*3 + 2]) << 0 );
				}
			}

			// Draw it to the screen now (ignore any errors here)...
			XPutImage(display, window, gc, img,
				0, 0, // Source X & Y (useful for subimages)
				0, 0, // Dest   X & Y
				width, height); // Width and height to draw (useful for subimages)
		}
	}

	void XFrameBuffer::disableRepeat()
	{
		if( display )
		{
			// NOTE! using XAutoRepeatOn or XAutoRepeatOff is GLOBAL to the entire
			// display, not the single window... I need to find a workaround.
			XAutoRepeatOff(display);
		}
		KeyboardInput::disableRepeat();
	}

	void XFrameBuffer::enableRepeat()
	{
		if( display )
		{
			XAutoRepeatOn(display);
		}
		KeyboardInput::enableRepeat();
	}

	void XFrameBuffer::sizeChangedCheck()
	{
		if( display )
		{
			bool size_changed = false;

			int x, y;
			unsigned width, height, border, depth;
			Window root;

			x = y = width = height = border = depth = 0;

			XGetGeometry(display, window, &root, &x, &y, &width, &height, &border, &depth);

			//    printf("Geometry returned x,y=[%d,%d], w,h=[%d,%d], b=%d, d=%d\n", x, y, width, height, border, depth);

			if( (width != getWidth()) || (height != getHeight()) )
			{
				resize(width, height);
				size_changed = true;
			}

			// If the size changed...
			if( size_changed || !img || !image_buffer )
			{
				int bytes_per_pixel = 4;

				if( (width > 0) && (height > 0) )

				{
					if( img )
					{
						XDestroyImage(img);
						img = NULL;
					}
					image_buffer = (unsigned char*)malloc(getWidth() * getHeight() * bytes_per_pixel);

					img = XCreateImage(display,
						XDefaultVisual(display, screen),
						depth, // This one has to match the depth of
						// the screen or a crash will happen.
						ZPixmap,
						0,  // offset...
						(char*)image_buffer,
						getWidth(), getHeight(),
						32, // bits per pixel (32 for 24 bit image)
						0); // Scanline width
				}
				else
				{
					if( img )
					{
						XDestroyImage(img);
						img = NULL;
					}
					image_buffer = NULL;
				}
			}
		} // display is valid...
	}

	void XFrameBuffer::runWindow()
	{
		// Event loop!

		XEvent event; /* the XEvent declaration !!! */
		KeySym key; /* something to aid in handling KeyPress Events */
		char text[255]; /* a buffer for KeyPress Events (as text)*/

		/* look for events forever... */
		while( display )
		{
			// FIXME! Don't get the next event like this.  Select() on the display's socket descriptor.

			/* get the next event and stuff it into our event variable.
				Note:  only events we set the mask for are detected!
			*/
			XNextEvent(display, &event);

			// printf("Event type %d\n", event.type);

			switch( event.type )
			{
			case Expose:
				if( event.xexpose.count == 0 )
				{
					/* the window was exposed redraw it! */
					drawable = true;
					sizeChangedCheck();
					update();
					draw();
				}
				break;
			case KeyPress:
				{
					// Note: I don't actually care if the lookup was successful.  All I
					// want this to do is to get the key symbol (button) filled in, so
					// that it can be used. I'm generating my own strings anyways.
					XLookupString(&event.xkey,text,255,&key,0);
					handle_keyboard_event(keyboard_key_translate(key),
						keyboard_state_translate(event.xkey.state) |
						KeyboardInput::keyboard_key_pressed);
				}
				break;
			case KeyRelease:
				{
					// See comments for key press above.
					XLookupString(&event.xkey,text,255,&key,0);
					handle_keyboard_event(keyboard_key_translate(key),
						keyboard_state_translate(event.xkey.state) &
						~KeyboardInput::keyboard_key_pressed);
				}
				break;
			case ButtonPress:
				{
					/* tell where the mouse Button was Pressed */
					int x = event.xbutton.x;
					int y = event.xbutton.y;
					unsigned state = event.xbutton.state;
					unsigned button = event.xbutton.button;

					state = translate_x_state(button, state, get_button_state());

					handle_mouse_event(x,y,state);
				}
				break;
			case ButtonRelease:
				{
					/* tell where the mouse Button was Released */
					int x = event.xbutton.x;
					int y = event.xbutton.y;
					unsigned state = event.xbutton.state;
					unsigned button = event.xbutton.button;

					state = translate_x_state(-button, state, get_button_state());

					handle_mouse_event(x,y,state);
				}
				break;
			case MotionNotify:
				{
					/* tell where the mouse is currently located */
					int x = event.xmotion.x;
					int y = event.xmotion.y;

					unsigned state = translate_x_state(0, event.xmotion.state, get_button_state());
					handle_mouse_event(x,y,state);
				}
				break;
			case FocusIn:
				{
					if( display && !repeat_enabled() )
					{
						XAutoRepeatOff(display);
					}
				}
				break;
			case FocusOut:
				{
					if( display )
					{
						XAutoRepeatOn(display);
					}
				}
				break;
			case DestroyNotify:
				{
					printf("Got a destroy event!\n"); fflush(stdout);
				}
				break;
			default:
				{
					printf("X supplied event type %d\n", event.type);
				}
				break;
			}
		} // while 1
	}

	void XFrameBuffer::quitWindow()
	{
		if( img )
		{
			XDestroyImage(img);
			img = NULL;
		}

		if( display )
		{
			XFreeGC(display, gc);
			XDestroyWindow(display, window);

			XAutoRepeatOn(display);

			if( display_created )
			{
				XCloseDisplay(display);
			}
		}
		display = NULL;
	}

	CloneableRef XFrameBuffer::clone() const
	{
		return CloneableRef(new XFrameBuffer(*this));
	}
}

