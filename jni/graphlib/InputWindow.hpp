/*
 * $Id: InputWindow.hpp,v 1.3 2003/11/10 09:32:27 kpharris Exp $
 *
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

#if       !defined(GRAPHLIB_INPUTWINDOW_HPP)
#define            GRAPHLIB_INPUTWINDOW_HPP

#include "miniblocxx/Function.hpp"
#include "MouseInput.hpp"
#include "KeyboardInput.hpp"
#include "FrameBuffer.hpp"


namespace graphlib
{
	/**
	 *
	 * A class which combines a framebuffer with the ability to accept input.
	 * Note that this class MUST be changed at some point in the future, as I
	 * need MUCH more flexible input methods (KB, JS, mouse, whatever).
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.6 $
	 *
	 */
	template <typename T>
	class InputWindow : public FrameBuffer<T>, public MouseInput, public KeyboardInput
	{

	private:

	protected:

	public:
		InputWindow();
		InputWindow(int width, int height);
		virtual ~InputWindow();
		InputWindow(const InputWindow& old);
		InputWindow& operator= (const InputWindow& old);

		virtual void update() { buffer_update(); }

		/** Enter the main message loop, returning only when the program is
			 terminated, or if quit_window() below is called. . */
		virtual void runWindow() = 0;

		/** Terminate any running message loop from above, allowing it to return.
			 Note that this will close the window completely, so it should only be
			 called when appropriate. */
		virtual void quitWindow() = 0;

		blocxx::Function0<void> buffer_update;

	}; // class InputWindow

	template <typename T>
	InputWindow<T>::InputWindow():
		FrameBuffer<T>(),
		MouseInput(),
		KeyboardInput()
	{

	} // InputWindow()

	template <typename T>
	InputWindow<T>::InputWindow(int width, int height):
		FrameBuffer<T>(),
		MouseInput(),
		KeyboardInput()
	{
		FrameBuffer<T>::resize(width, height);
	} // InputWindow()

	template <typename T>
	InputWindow<T>::~InputWindow()
	{

	} // ~InputWindow()

	template <typename T>
	InputWindow<T>::InputWindow(const InputWindow<T>& old):
		FrameBuffer<T>(old),
		MouseInput(old),
		KeyboardInput(old)
	{

	} // InputWindow(InputWindow)

	template <typename T>
	InputWindow<T>& InputWindow<T>::operator= (const InputWindow<T>& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{
			FrameBuffer<T>::operator=(old);
			MouseInput::operator=(old);
			KeyboardInput::operator=(old);
		}
		return (*this);
	} // InputWindow::operator=(InputWindow)
}
#endif /* !defined(GRAPHLIB_INPUTWINDOW_HPP) */

