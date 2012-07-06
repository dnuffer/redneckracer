// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "AndroidKeyboardInput.hpp"
#include "miniblocxx/Format.hpp"

namespace engine
{
	BLOCXX_DEFINE_EXCEPTION(Keyboard);

	namespace
	{
		enum AndroidKeyMasks
			{
				META_ALT_ON = 0x02,
				META_ALT_LEFT_ON = 0x10,
				META_SHIFT_ON = 0x1,
				META_SHIFT_LEFT_ON = 0x40,
				META_SHIFT_RIGHT_ON = 0x80,
				META_SYM_ON = 0x4,
				FLAG_WOKE_HERE = 0x1,
				FLAG_SOFT_KEYBOARD = 0x2,
				FLAG_KEEP_TOUCH_MODE = 0x4,
				FLAG_FROM_SYSTEM = 0x8,
				FLAG_EDITOR_ACTION = 0x10,
				FLAG_CANCELED = 0x20,
				FLAG_VIRTUAL_HARD_KEY = 0x40,
				FLAG_LONG_PRESS = 0x80,
				FLAG_CANCELED_LONG_PRESS = 0x100,
				FLAG_TRACKING = 0x200,
				FLAG_START_TRACKING = 0x40000000,
			};
		enum AndroidKeyCodes
			{
				KEYCODE_UNKNOWN         = 0,
				KEYCODE_SOFT_LEFT       = 1,
				KEYCODE_SOFT_RIGHT      = 2,
				KEYCODE_HOME            = 3,
				KEYCODE_BACK            = 4,
				KEYCODE_CALL            = 5,
				KEYCODE_ENDCALL         = 6,
				KEYCODE_0               = 7,
				KEYCODE_1               = 8,
				KEYCODE_2               = 9,
				KEYCODE_3               = 10,
				KEYCODE_4               = 11,
				KEYCODE_5               = 12,
				KEYCODE_6               = 13,
				KEYCODE_7               = 14,
				KEYCODE_8               = 15,
				KEYCODE_9               = 16,
				KEYCODE_STAR            = 17,
				KEYCODE_POUND           = 18,
				KEYCODE_DPAD_UP         = 19,
				KEYCODE_DPAD_DOWN       = 20,
				KEYCODE_DPAD_LEFT       = 21,
				KEYCODE_DPAD_RIGHT      = 22,
				KEYCODE_DPAD_CENTER     = 23,
				KEYCODE_VOLUME_UP       = 24,
				KEYCODE_VOLUME_DOWN     = 25,
				KEYCODE_POWER           = 26,
				KEYCODE_CAMERA          = 27,
				KEYCODE_CLEAR           = 28,
				KEYCODE_A               = 29,
				KEYCODE_B               = 30,
				KEYCODE_C               = 31,
				KEYCODE_D               = 32,
				KEYCODE_E               = 33,
				KEYCODE_F               = 34,
				KEYCODE_G               = 35,
				KEYCODE_H               = 36,
				KEYCODE_I               = 37,
				KEYCODE_J               = 38,
				KEYCODE_K               = 39,
				KEYCODE_L               = 40,
				KEYCODE_M               = 41,
				KEYCODE_N               = 42,
				KEYCODE_O               = 43,
				KEYCODE_P               = 44,
				KEYCODE_Q               = 45,
				KEYCODE_R               = 46,
				KEYCODE_S               = 47,
				KEYCODE_T               = 48,
				KEYCODE_U               = 49,
				KEYCODE_V               = 50,
				KEYCODE_W               = 51,
				KEYCODE_X               = 52,
				KEYCODE_Y               = 53,
				KEYCODE_Z               = 54,
				KEYCODE_COMMA           = 55,
				KEYCODE_PERIOD          = 56,
				KEYCODE_ALT_LEFT        = 57,
				KEYCODE_ALT_RIGHT       = 58,
				KEYCODE_SHIFT_LEFT      = 59,
				KEYCODE_SHIFT_RIGHT     = 60,
				KEYCODE_TAB             = 61,
				KEYCODE_SPACE           = 62,
				KEYCODE_SYM             = 63,
				KEYCODE_EXPLORER        = 64,
				KEYCODE_ENVELOPE        = 65,
				KEYCODE_ENTER           = 66,
				KEYCODE_DEL             = 67,
				KEYCODE_GRAVE           = 68,
				KEYCODE_MINUS           = 69,
				KEYCODE_EQUALS          = 70,
				KEYCODE_LEFT_BRACKET    = 71,
				KEYCODE_RIGHT_BRACKET   = 72,
				KEYCODE_BACKSLASH       = 73,
				KEYCODE_SEMICOLON       = 74,
				KEYCODE_APOSTROPHE      = 75,
				KEYCODE_SLASH           = 76,
				KEYCODE_AT              = 77,
				KEYCODE_NUM             = 78,
				KEYCODE_HEADSETHOOK     = 79,
				KEYCODE_FOCUS           = 80,   // *Camera* focus
				KEYCODE_PLUS            = 81,
				KEYCODE_MENU            = 82,
				KEYCODE_NOTIFICATION    = 83,
				KEYCODE_SEARCH          = 84,
				KEYCODE_MEDIA_PLAY_PAUSE= 85,
				KEYCODE_MEDIA_STOP      = 86,
				KEYCODE_MEDIA_NEXT      = 87,
				KEYCODE_MEDIA_PREVIOUS  = 88,
				KEYCODE_MEDIA_REWIND    = 89,
				KEYCODE_MEDIA_FAST_FORWARD = 90,
				KEYCODE_MUTE            = 91,
				KEYCODE_PAGE_UP         = 92,
				KEYCODE_PAGE_DOWN       = 93,
				KEYCODE_PICTSYMBOLS     = 94,   // switch symbol-sets (Emoji, Kao-moji)
				KEYCODE_SWITCH_CHARSET  = 95,   // switch char-sets (Kanji, Katakana)
			};
	}

	AndroidKeyboardInput::AndroidKeyboardInput()
		: KeyboardInput()
	{
	}

	AndroidKeyboardInput::~AndroidKeyboardInput()
	{
	}

	KeyboardInput::KeyboardKeys AndroidKeyboardInput::translateKeycode(int keycode)
	{
		switch(AndroidKeyCodes(keycode))
		{
		case KEYCODE_UNKNOWN:
			return KeyboardInput::KEY_unknown;
		case KEYCODE_CALL:
			return KeyboardInput::KEY_start_call;
		case KEYCODE_ENDCALL:
			return KeyboardInput::KEY_end_call;
		case KEYCODE_SYM:
			return KeyboardInput::KEY_alt_symbol1;
		case KEYCODE_PICTSYMBOLS:
			return KeyboardInput::KEY_alt_symbol2;
		case KEYCODE_HEADSETHOOK:
			return KeyboardInput::KEY_media_audio;
		case KEYCODE_FOCUS:
			return KeyboardInput::KEY_focus;
		case KEYCODE_NOTIFICATION:
			return KeyboardInput::KEY_notification;
		case KEYCODE_SWITCH_CHARSET:
			return KeyboardInput::KEY_mode_switch;

		case KEYCODE_SOFT_LEFT:
			return KeyboardInput::KEY_left;
		case KEYCODE_SOFT_RIGHT:
			return KeyboardInput::KEY_right;
		case KEYCODE_HOME:
			return KeyboardInput::KEY_home;
		case KEYCODE_BACK:
			return KeyboardInput::KEY_escape;
		case KEYCODE_0:
			return KeyboardInput::KEY_0;
		case KEYCODE_1:
			return KeyboardInput::KEY_1;
		case KEYCODE_2:
			return KeyboardInput::KEY_2;
		case KEYCODE_3:
			return KeyboardInput::KEY_3;
		case KEYCODE_4:
			return KeyboardInput::KEY_4;
		case KEYCODE_5:
			return KeyboardInput::KEY_5;
		case KEYCODE_6:
			return KeyboardInput::KEY_6;
		case KEYCODE_7:
			return KeyboardInput::KEY_7;
		case KEYCODE_8:
			return KeyboardInput::KEY_8;
		case KEYCODE_9:
			return KeyboardInput::KEY_9;
		case KEYCODE_STAR:
			return KeyboardInput::KEY_asterisk;
		case KEYCODE_POUND:
			return KeyboardInput::KEY_numbersign; // CHECKME! Do they really mean number or GPB style pound?
		case KEYCODE_DPAD_UP:
			return KeyboardInput::KEY_keypad_up;
		case KEYCODE_DPAD_DOWN:
			return KeyboardInput::KEY_keypad_down;
		case KEYCODE_DPAD_LEFT:
			return KeyboardInput::KEY_keypad_left;
		case KEYCODE_DPAD_RIGHT:
			return KeyboardInput::KEY_keypad_right;
		case KEYCODE_DPAD_CENTER:
			return KeyboardInput::KEY_keypad_center;
		case KEYCODE_VOLUME_UP:
			return KeyboardInput::KEY_volume_up;
		case KEYCODE_VOLUME_DOWN:
			return KeyboardInput::KEY_volume_down;
		case KEYCODE_POWER:
			return KeyboardInput::KEY_power;
		case KEYCODE_CLEAR:
			return KeyboardInput::KEY_clear;
		case KEYCODE_A:
			return KeyboardInput::KEY_a;
		case KEYCODE_B:
			return KeyboardInput::KEY_b;
		case KEYCODE_C:
			return KeyboardInput::KEY_c;
		case KEYCODE_D:
			return KeyboardInput::KEY_d;
		case KEYCODE_E:
			return KeyboardInput::KEY_e;
		case KEYCODE_F:
			return KeyboardInput::KEY_f;
		case KEYCODE_G:
			return KeyboardInput::KEY_g;
		case KEYCODE_H:
			return KeyboardInput::KEY_h;
		case KEYCODE_I:
			return KeyboardInput::KEY_i;
		case KEYCODE_J:
			return KeyboardInput::KEY_j;
		case KEYCODE_K:
			return KeyboardInput::KEY_k;
		case KEYCODE_L:
			return KeyboardInput::KEY_l;
		case KEYCODE_M:
			return KeyboardInput::KEY_m;
		case KEYCODE_N:
			return KeyboardInput::KEY_n;
		case KEYCODE_O:
			return KeyboardInput::KEY_o;
		case KEYCODE_P:
			return KeyboardInput::KEY_p;
		case KEYCODE_Q:
			return KeyboardInput::KEY_q;
		case KEYCODE_R:
			return KeyboardInput::KEY_r;
		case KEYCODE_S:
			return KeyboardInput::KEY_s;
		case KEYCODE_T:
			return KeyboardInput::KEY_t;
		case KEYCODE_U:
			return KeyboardInput::KEY_u;
		case KEYCODE_V:
			return KeyboardInput::KEY_v;
		case KEYCODE_W:
			return KeyboardInput::KEY_w;
		case KEYCODE_X:
			return KeyboardInput::KEY_x;
		case KEYCODE_Y:
			return KeyboardInput::KEY_y;
		case KEYCODE_Z:
			return KeyboardInput::KEY_z;
		case KEYCODE_COMMA:
			return KeyboardInput::KEY_comma;
		case KEYCODE_PERIOD:
			return KeyboardInput::KEY_period;
		case KEYCODE_ALT_LEFT:
			return KeyboardInput::KEY_alt_left;
		case KEYCODE_ALT_RIGHT:
			return KeyboardInput::KEY_alt_right;
		case KEYCODE_SHIFT_LEFT:
			return KeyboardInput::KEY_shift_left;
		case KEYCODE_SHIFT_RIGHT:
			return KeyboardInput::KEY_shift_right;
		case KEYCODE_TAB:
			return KeyboardInput::KEY_tab;
		case KEYCODE_SPACE:
			return KeyboardInput::KEY_space;
		case KEYCODE_ENTER:
			return KeyboardInput::KEY_return;
		case KEYCODE_DEL:
			return KeyboardInput::KEY_delete;
		case KEYCODE_GRAVE:
			return KeyboardInput::KEY_backquote; // CHECKME!
		case KEYCODE_MINUS:
			return KeyboardInput::KEY_minus;
		case KEYCODE_EQUALS:
			return KeyboardInput::KEY_equal;
		case KEYCODE_LEFT_BRACKET:
			return KeyboardInput::KEY_bracket_left;
		case KEYCODE_RIGHT_BRACKET:
			return KeyboardInput::KEY_bracket_right;
		case KEYCODE_BACKSLASH:
			return KeyboardInput::KEY_backslash;
		case KEYCODE_SEMICOLON:
			return KeyboardInput::KEY_semicolon;
		case KEYCODE_APOSTROPHE:
			return KeyboardInput::KEY_apostrophe;
		case KEYCODE_SLASH:
			return KeyboardInput::KEY_slash;
		case KEYCODE_AT:
			return KeyboardInput::KEY_at;
		case KEYCODE_NUM:
			return KeyboardInput::KEY_numbersign;
		case KEYCODE_PLUS:
			return KeyboardInput::KEY_plus;
		case KEYCODE_MENU:
			return KeyboardInput::KEY_menu;
		case KEYCODE_SEARCH:
			return KeyboardInput::KEY_find;
		case KEYCODE_MEDIA_PLAY_PAUSE:
			return KeyboardInput::KEY_media_play;
		case KEYCODE_MEDIA_STOP:
			return KeyboardInput::KEY_media_stop;
		case KEYCODE_MEDIA_NEXT:
			return KeyboardInput::KEY_media_next;
		case KEYCODE_MEDIA_PREVIOUS:
			return KeyboardInput::KEY_media_previous;
		case KEYCODE_MEDIA_REWIND:
			return KeyboardInput::KEY_media_rewind;
		case KEYCODE_MEDIA_FAST_FORWARD:
			return KeyboardInput::KEY_media_forward;
		case KEYCODE_MUTE:
			return KeyboardInput::KEY_volume_mute;
		case KEYCODE_PAGE_UP:
			return KeyboardInput::KEY_page_up;
		case KEYCODE_PAGE_DOWN:
			return KeyboardInput::KEY_page_down;
		case KEYCODE_ENVELOPE:
			return KeyboardInput::KEY_mail;
		case KEYCODE_CAMERA:
			return KeyboardInput::KEY_camera;
		case KEYCODE_EXPLORER:
			return KeyboardInput::KEY_explore_computer;
		}
		BLOCXX_THROW(KeyboardException, Format("Invalid keycode: %1", keycode).c_str());
	}

	KeyboardInput::KeyboardModifiers AndroidKeyboardInput::translateMetastate(int metastate)
	{
		int state = 0;
		if( metastate & META_ALT_ON )
		{
			state |= KeyboardInput::keyboard_alt;
		}
		if( metastate & META_ALT_LEFT_ON )
		{
			state |= KeyboardInput::keyboard_left_alt;
		}
		if( metastate & META_SHIFT_ON )
		{
			state |= KeyboardInput::keyboard_shift;
		}
		if( metastate & META_SHIFT_LEFT_ON )
		{
			state |= KeyboardInput::keyboard_left_shift;
		}
		if( metastate & META_SHIFT_RIGHT_ON )
		{
			state |= KeyboardInput::keyboard_right_shift;
		}

		/*
		  These are not yet explicitly handled:
		  META_SYM_ON
		  FLAG_WOKE_HERE
		  FLAG_SOFT_KEYBOARD
		  FLAG_KEEP_TOUCH_MODE
		  FLAG_FROM_SYSTEM
		  FLAG_EDITOR_ACTION
		  FLAG_CANCELED
		  FLAG_VIRTUAL_HARD_KEY
		  FLAG_LONG_PRESS
		  FLAG_CANCELED_LONG_PRESS
		  FLAG_TRACKING
		  FLAG_START_TRACKING
		*/
		return KeyboardInput::KeyboardModifiers(state);
	}

	void AndroidKeyboardInput::keyDown(int keycode, int metastate)
	{
		KeyboardInput::handle_keyboard_event(
			translateKeycode(keycode),
			translateMetastate(metastate) | KeyboardInput::keyboard_key_pressed);
	}
	void AndroidKeyboardInput::keyUp(int keycode, int metastate)
	{
		KeyboardInput::handle_keyboard_event(
			translateKeycode(keycode),
			translateMetastate(metastate) & ~KeyboardInput::keyboard_key_pressed);
	}

}
