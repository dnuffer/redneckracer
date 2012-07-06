/*
 * $Id: XFrameBuffer.hpp,v 1.3 2003/11/10 09:32:27 kpharris Exp $
 *
 * Part of "Graphlib" - A nearly infinitely expandable gaming framework
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

#if       !defined(GRAPHLIB_XFRAMEBUFFER_HPP)
#define            GRAPHLIB_XFRAMEBUFFER_HPP

#include "InputWindow.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

namespace graphlib
{
	class XFrameBuffer : public InputWindow<unsigned char>
	{
	public:
		typedef unsigned char BaseDataType;
		typedef FrameBuffer<BaseDataType> BaseFrameBuffer;
		typedef InputWindow<BaseDataType> BaseClassType;

		XFrameBuffer(int width = 200, int height = 200, Display* disp = NULL, int screen = -1);
		XFrameBuffer(const XFrameBuffer& old);
		virtual ~XFrameBuffer();

		// Implemented from the base class.
		void draw();
		CloneableRef clone() const;
		blocxx::String name() const { return "XFrameBuffer"; }

		void enableRepeat();
		void disableRepeat();
		void sizeChangedCheck();

		unsigned getWidth()  const { return BaseFrameBuffer::getWidth(); }
		unsigned getHeight() const { return BaseFrameBuffer::getHeight(); }

		/** Enter the main message loop, returning only when the program is
			 terminated, or if quitWindow() below is called. . */
		void runWindow();
		/** Terminate any running message loop from above, allowing it to return.
			 Note that this will close the window completely, so it should only be
			 called when appropriate. */
		void quitWindow();
	private:
		XFrameBuffer& operator= (const XFrameBuffer& old); // not implemented
		void initialize();

		Display *display;
		bool display_created;
		int screen;
		Window window;
		GC gc;
		XImage* img;
		unsigned char* image_buffer; // Required for display on X.
		bool drawable;
	};
} // end namespace graphlib

#endif /* !defined(GRAPHLIB_XFRAMEBUFFER_HPP) */
