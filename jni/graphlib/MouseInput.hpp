/*
 * $Id: MouseInput.hpp,v 1.4 2010/08/31 04:10:07 kpharris Exp $
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

#if       !defined(GRAPHLIB_MOUSEINPUT_HPP)
#define            GRAPHLIB_MOUSEINPUT_HPP

#include "miniblocxx/Function.hpp"

namespace graphlib
{
	/**
	 *
	 * A basic class for mouse input (to be subclassed by the class which
	 * actually has the capability to detect mouse stuff).
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.4 $
	 *
	 */
	class MouseInput
	{
	public:
		enum MouseStateMasks { mouse_button_1      = 1 << 0,
									  mouse_button_2      = 1 << 1,
									  mouse_button_3      = 1 << 2,
									  mouse_button_4      = 1 << 3,
									  mouse_button_5      = 1 << 4,
									  mouse_shift_down    = 1 << 5,
									  mouse_control_down  = 1 << 6,
									  mouse_alt_down      = 1 << 7,
									  mouse_numlock_on    = 1 << 8,
									  mouse_mod3_down     = 1 << 9,
									  mouse_super_down    = 1 << 10,
									  mouse_mod5_down     = 1 << 11,
									  mouse_capslock_on   = 1 << 12,
									  mouse_button_mask   = 0x001f,
									  mouse_modifier_mask = 0x1fe0,
									  max_mouse_button   = 5
		};
	private:

	protected:
		/** Dish out any required operations to the callback functions, update
			 state, etc.
			 Note that this is NOT public, because only subclasses should tell this
			 class that some mouse event occurred.
		*/
		virtual bool handle_mouse_event(int x, int y, unsigned state);

		struct mouse_press_data 
		{ 
			int x; 
			int y; 
			unsigned press_state; 
			mouse_press_data() : x(0), y(0), press_state(0) {}
		};

		int mouse_x, mouse_y;           // Current X and Y
		int last_x,  last_y;            // X and Y of the last event.
		unsigned last_state;            // Holds the state at the last event
		mouse_press_data press_data[5]; // Press data for each of the 5 allowed buttons
		int maximum_click_distance;     // The max # of pixels dragable when clicking.
		bool valid_data;                // says if data (positions, etc) is valid valid.

	public:
		MouseInput();
		virtual ~MouseInput();
		MouseInput(const MouseInput& old);
		MouseInput& operator= (const MouseInput& old);


		/** Return the current button state (if there is one). */
		virtual unsigned get_button_state() const;

		/** Return the number of buttons the mouse has. */
		//    virtual int num_mouse_buttons() const = 0;

		/** Return the current mouse position (in x and y), valid IFF it returns
			 true */
		virtual bool get_mouse_position(int& x, int& y) const;

		/** Return the last (previous) mouse position (in x and y), valid IFF it
			 returns true */
		virtual bool get_last_position(int& x, int& y) const;

		/** Get the maximum distance between press and release to be counted as a
			 click. */
		int get_max_click_distance() const { return maximum_click_distance; }

		/** Set the maximum distance between press and release to be counted as a
			 click. */
		void set_max_click_distance(int d) { maximum_click_distance = d; }

		//bool mouse_clicked(int x, int y, unsigned press_state, unsigned release_state);
		//bool mouse_dragged(int x1, int y1, int x2, int y2, unsigned state_diff);
		//bool mouse_moved(int x, int y, int dx, int dy);
		//bool mouse_pressed(int x, int y, unsigned press_state, unsigned state_diff);
		//bool mouse_released(int x, int y, unsigned release_state, unsigned state_diff);

		// Here's how the callbacks will work:
		// Upon button press, the state will be set, then 'pressed' will be called.
		// Upon button release, state will be updated, released called, then either
		//      clicked or dragged will be called (depending on the distance
		//      moved).
		// Note: if the internals of the callback function for 'dragged' need to
		//   find out what the current state is, then it will need to call the
		//   get_button_state function.  SicC++ only allowed up to 5 arguments for
		//   signals. :(

		// See above comments for the arguments to these callbacks.
		blocxx::Function4<bool,int,int,unsigned,unsigned> mouse_clicked;
		blocxx::Function5<bool,int,int,int,int,unsigned> mouse_dragged;
		blocxx::Function4<bool,int,int,int,int> mouse_moved;
		blocxx::Function4<bool,int,int,unsigned,unsigned> mouse_pressed;
		blocxx::Function4<bool,int,int,unsigned,unsigned> mouse_released;
	}; // class MouseInput

} // namespace graphlib

#endif /* !defined(GRAPHLIB_MOUSEINPUT_HPP) */
