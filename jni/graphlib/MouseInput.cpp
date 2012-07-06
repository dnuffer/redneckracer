/*
 * $Id: MouseInput.cpp,v 1.2 2003/11/10 09:35:14 kpharris Exp $
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

/*
  TODO:
  constructor, copy constructor, assignment: copy press state...
*/

#include "MouseInput.hpp"

namespace graphlib
{

	namespace
	{
		bool stub_clicked(int x, int y, unsigned press_state, unsigned release_state)
		{
			return false;
		}
		bool stub_dragged(int x1, int y1, int x2, int y2, unsigned state_diff)
		{
			return false;
		}
		bool stub_moved(int x, int y, int dx, int dy)
		{
			return false;
		}
		bool stub_pressed(int x, int y, unsigned press_state, unsigned state_diff)
		{
			return false;
		}
		bool stub_released(int x, int y, unsigned release_state, unsigned state_diff)
		{
			return false;
		}
	}

	MouseInput::MouseInput():
		mouse_x(0), mouse_y(0),
		last_x(0), last_y(0),
		last_state(0),
		maximum_click_distance(2),
		valid_data(false),
		mouse_clicked(blocxx::ptrfun(&stub_clicked)),
		mouse_dragged(blocxx::ptrfun(&stub_dragged)),
		mouse_moved(blocxx::ptrfun(&stub_moved)),
		mouse_pressed(blocxx::ptrfun(&stub_pressed)),
		mouse_released(blocxx::ptrfun(&stub_released))
	{

	} // MouseInput()

	MouseInput::~MouseInput()
	{

	} // ~MouseInput()

	MouseInput::MouseInput(const MouseInput& old):
		mouse_x(old.mouse_x), mouse_y(old.mouse_y),
		last_x(old.last_x), last_y(old.last_y),
		last_state(old.last_state),
		maximum_click_distance(old.maximum_click_distance),
		valid_data(old.valid_data),
		mouse_clicked(old.mouse_clicked),
		mouse_dragged(old.mouse_dragged),
		mouse_moved(old.mouse_moved),
		mouse_pressed(old.mouse_pressed),
		mouse_released(old.mouse_released)
	{

	} // MouseInput(MouseInput)

	MouseInput& MouseInput::operator= (const MouseInput& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{
			mouse_x = old.mouse_x;
			mouse_y = old.mouse_y;
			last_x = old.last_x;
			last_y = old.last_y;
			last_state = old.last_state;
			maximum_click_distance = old.maximum_click_distance;
			valid_data = old.valid_data;

			mouse_clicked = old.mouse_clicked;
			mouse_dragged = old.mouse_dragged;
			mouse_moved = old.mouse_moved;
			mouse_pressed = old.mouse_pressed;
			mouse_released = old.mouse_released;
		}
		return (*this);
	} // MouseInput::operator=(MouseInput)

	int which_button(unsigned change)
	{
		for(int i = 0; i < 5; ++i, change >>= 1)
		{
			if( change & 1 )
			{
				return i;
			}
		}
		return -1;
	}

	bool MouseInput::handle_mouse_event(int x, int y, unsigned state)
	{
		//    printf("handle_mouse_event(%d,%d,0x%06x)\n", x,y,state);

		// Notes: the way that this 'handled' variable is used should ALWAYS be put
		// on the RIGHT side of the ||, so as to avoid short circuiting.
		// CHECKME! If it is desirable to allow handling of 'release' to prevent
		// calling of 'clicked' or 'dragged', then this could be moved to the left
		// side.

		// Not initially handled... The callbacks must do that.
		bool handled = false;

		// Initialize stuff, so that everything will work properly.
		if( !valid_data )
		{
			last_state = 0;

			mouse_x = x;
			mouse_y = y;

			valid_data = true;
		}

		// Store current stuff as previous
		last_x = mouse_x;
		last_y = mouse_y;

		// Store new stuff as current.
		mouse_x = x;
		mouse_y = y;

		// Find all differences in buttons...
		unsigned state_difference = (last_state ^ state);
		last_state = state;

		// If there is a button difference... Either a press or release.
		unsigned button_state_change = state_difference & mouse_button_mask;
		if( button_state_change != 0 )
		{
			if( state & button_state_change )
			{
				// It was a press!
				int button_pressed = which_button(button_state_change);
				if( button_pressed >= 0 )
				{
					press_data[button_pressed].x = mouse_x;
					press_data[button_pressed].y = mouse_y;
					press_data[button_pressed].press_state = state;
				}
				handled = mouse_pressed(mouse_x, mouse_y, state, state_difference) || handled;
			} // if a press
			else
			{
				// It was a release!
				handled = mouse_released(x,y,state,state_difference) || handled;

				// Check to see if it could be a click...
				int button_released = which_button(button_state_change);

				if( button_released >= 0 )
				{
					int diff_x = x - press_data[button_released].x;
					int diff_y = y - press_data[button_released].y;

					if( ((diff_x <  maximum_click_distance) &&
							(diff_x > -maximum_click_distance)) &&
						((diff_y <  maximum_click_distance) &&
							(diff_y > -maximum_click_distance)) )
					{
						// It's a click!
						handled = mouse_clicked(x, y,
							press_data[button_released].press_state,
							state) || handled;
					}
					else
					{
						// It's a drag!
						// Reset the state difference to be that between when it was
						// pressed and when it was released.
						state_difference = (press_data[button_released].press_state ^ state);

						handled = mouse_dragged(press_data[button_released].x,
							press_data[button_released].y,
							mouse_x, mouse_y, state_difference) || handled;
					}
				} // button >= 0 (if it's valid)
			} // if a release
		} // button changed
		else
		{
			// Just a mouse movement...
			handled = mouse_moved(mouse_x, mouse_y,
				mouse_x - last_x,
				mouse_y - last_y) || handled;
		} // no buttons changed

		return handled;
	} // MouseInput::handle_mouse_event(int,int,int)


	unsigned MouseInput::get_button_state() const
	{
		if( valid_data )
		{
			return last_state;
		}
		return 0;
	} // MouseInput::get_button_state()

	bool MouseInput::get_mouse_position(int& x, int& y) const
	{
		if( valid_data )
		{
			x = mouse_x;
			y = mouse_y;
			return true;
		}
		return false;

	} // MouseInput::get_mouse_position(int,int)

	bool MouseInput::get_last_position(int& x, int& y) const
	{
		if( valid_data )
		{
			x = last_x;
			y = last_y;
			return true;
		}
		return false;
	} // MouseInput::get_last_position(int,int)

} // namespace graphlib

