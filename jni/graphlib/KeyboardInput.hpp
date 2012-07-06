/*
 * $Id: keyboard_input.hpp,v 1.5 2010/08/31 04:10:07 kpharris Exp $
 *
 * Part of "Many Games" - A nearly infinitely expandable gaming framework
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


#if       !defined(GRAPHLIB_KEYBOARD_INPUT_HPP)
#define            GRAPHLIB_KEYBOARD_INPUT_HPP


/*
  TODO:
  Allow iterations over name overrides, for storing in config file, etc.
  Allow removal of name overrides.
  Allow iteration over currently pressed keys...
*/

#include "miniblocxx/Function.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/stl/vector"
#include <map>

namespace graphlib
{
	/**
	 *
	 * A basic class for keyboard input (to be subclassed by the class which
	 * actually has the capability to detect keyboard stuff).
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.5 $
	 *
	 */
	class KeyboardInput
	{
	public:
		enum KeyboardModifiers { keyboard_left_control  = 1 << 0,
										 keyboard_left_shift    = 1 << 1,
										 keyboard_left_alt      = 1 << 2,
										 keyboard_numlock       = 1 << 3,
										 keyboard_left_mod3     = 1 << 4,
										 keyboard_left_super    = 1 << 5,
										 keyboard_left_mod5     = 1 << 6,
										 keyboard_right_control = 1 << 8,
										 keyboard_right_shift   = 1 << 9,
										 keyboard_right_alt     = 1 << 10,
										 keyboard_right_mod3    = 1 << 12,
										 keyboard_right_super   = 1 << 13,
										 keyboard_right_mod5    = 1 << 14,
										 keyboard_capslock      = 1 << 15,
										 keyboard_key_pressed   = 1 << 16,
										 keyboard_control       = keyboard_left_control | keyboard_right_control,
										 keyboard_shift         = keyboard_left_shift   | keyboard_right_shift,
										 keyboard_alt           = keyboard_left_alt     | keyboard_right_alt,
										 keyboard_mod3          = keyboard_left_mod3    | keyboard_right_mod3,
										 keyboard_super         = keyboard_left_super   | keyboard_right_super,
										 keyboard_mod5          = keyboard_left_mod5    | keyboard_right_mod5
		};
	private:
		bool allow_repeat;

		// A mask to ignore when generating text for a keypress.  For example, you
		// may want to ignore the modifier flags that say the numlock key is on, as
		// it also changes the key codes that are sent.
		unsigned modifier_ignore_mask;

	protected:
		virtual bool handle_keyboard_event(unsigned button, unsigned modifiers);

		// Map a key (currently held) to the modifiers held when it was pressed.
		std::map<unsigned, unsigned> buttons_currently_held;

		struct key_and_mod
		{
			unsigned key;
			unsigned mod;
			key_and_mod(unsigned k, unsigned m): key(k), mod(m) { }
			bool operator<(const key_and_mod& kam) const
			{
				if( key < kam.key)
				{
					return true;
				}
				else if( key == kam.key )
				{
					return mod < kam.mod;
				}
				return false;
			}
			bool operator==(const key_and_mod& kam) const
			{
				return (key == kam.key) && (mod == kam.mod);
			}
		};

		std::map<key_and_mod, blocxx::String> keyname_overrides_with_modifier;
		std::map<unsigned, blocxx::String> keyname_overrides;
		std::map<unsigned, blocxx::String> modifier_name_overrides;

	public:
		KeyboardInput();
		virtual ~KeyboardInput();
		KeyboardInput(const KeyboardInput& old);
		KeyboardInput& operator= (const KeyboardInput& old);

		// Would return something like "A", "W", "Tab", etc.  That is, for
		// printable characters, it SHOULD return the single character that it
		// represents.  For longer keys, it would be the name of the key.
		virtual blocxx::String keyname(unsigned key) const;
		// would return something like "<CTRL>+A" (short) or "<Shift>+Z", "<Control>+<Alt>+I" (normal)
		virtual blocxx::String keyname(unsigned key, unsigned modifiers, bool shortened) const;

		// Force the key name for the given key to be displayed as new_name
		// (only while the overridden class is still active).
		virtual void override_keyname(unsigned key, const blocxx::String& new_name);
		// Force the key name for the given key, ONLY when the given modifiers are attached.
		virtual void override_keyname(unsigned key, unsigned modifiers, const blocxx::String& new_name);
		// Change the name of a modifier...
		virtual void override_modifier(unsigned modifier, const blocxx::String& new_name);

		virtual void clear_modifier_ignore_mask();
		virtual void add_modifier_ignore_mask(unsigned mask);
		virtual unsigned get_modifier_ignore_mask() const;

		virtual void disableRepeat() { allow_repeat = false; }
		virtual void enableRepeat()  { allow_repeat = true; }
		bool repeat_enabled() const { return allow_repeat; }

		bool key_down(unsigned key) const;
		bool key_down(unsigned key, unsigned& modifiers) const;
		unsigned num_keys_down() const;
		std::vector<std::pair<unsigned,unsigned> > get_all_held_keys() const;

		// bool keyboard_pressed(unsigned key, unsigned modifiers)
		// bool keyboard_released(unsigned key, unsigned press_modifiers, unsigned release_modifiers)
		blocxx::Function2<bool,unsigned,unsigned> key_pressed;
		blocxx::Function3<bool,unsigned,unsigned,unsigned> key_released;



		/* These are constants some of which (anything non-ascii) have been taken
			from the X header keysymdef.h, so any base classes that have key access
			that differ from X will need to translate the keycodes before sending
			them to the 'handle_keyboard_event' function */
		enum KeyboardKeys
			{
				KEY_unknown = 0,
				//-----------
				// printables
				//-----------
				// Numbers
				KEY_0                   = '0',
				KEY_1                   = '1',
				KEY_2                   = '2',
				KEY_3                   = '3',
				KEY_4                   = '4',
				KEY_5                   = '5',
				KEY_6                   = '6',
				KEY_7                   = '7',
				KEY_8                   = '8',
				KEY_9                   = '9',
				// Uppercase alphas
				KEY_A                   = 'A',
				KEY_B                   = 'B',
				KEY_C                   = 'C',
				KEY_D                   = 'D',
				KEY_E                   = 'E',
				KEY_F                   = 'F',
				KEY_G                   = 'G',
				KEY_H                   = 'H',
				KEY_I                   = 'I',
				KEY_J                   = 'J',
				KEY_K                   = 'K',
				KEY_L                   = 'L',
				KEY_M                   = 'M',
				KEY_N                   = 'N',
				KEY_O                   = 'O',
				KEY_P                   = 'P',
				KEY_Q                   = 'Q',
				KEY_R                   = 'R',
				KEY_S                   = 'S',
				KEY_T                   = 'T',
				KEY_U                   = 'U',
				KEY_V                   = 'V',
				KEY_W                   = 'W',
				KEY_X                   = 'X',
				KEY_Y                   = 'Y',
				KEY_Z                   = 'Z',
				// Lowercase alphas
				KEY_a                   = 'a',
				KEY_b                   = 'b',
				KEY_c                   = 'c',
				KEY_d                   = 'd',
				KEY_e                   = 'e',
				KEY_f                   = 'f',
				KEY_g                   = 'g',
				KEY_h                   = 'h',
				KEY_i                   = 'i',
				KEY_j                   = 'j',
				KEY_k                   = 'k',
				KEY_l                   = 'l',
				KEY_m                   = 'm',
				KEY_n                   = 'n',
				KEY_o                   = 'o',
				KEY_p                   = 'p',
				KEY_q                   = 'q',
				KEY_r                   = 'r',
				KEY_s                   = 's',
				KEY_t                   = 't',
				KEY_u                   = 'u',
				KEY_v                   = 'v',
				KEY_w                   = 'w',
				KEY_x                   = 'x',
				KEY_y                   = 'y',
				KEY_z                   = 'z',
				// Punctuation/symbols
				KEY_space               = ' ',
				KEY_exclam              = '!',
				KEY_quote_double        = '"',
				KEY_numbersign          = '#',
				KEY_dollar              = '$',
				KEY_percent             = '%',
				KEY_ampersand           = '&',
				KEY_apostrophe          = '\'',
				KEY_paren_left          = '(',
				KEY_paren_right         = ')',
				KEY_asterisk            = '*',
				KEY_plus                = '+',
				KEY_comma               = ',',
				KEY_minus               = '-',
				KEY_period              = '.',
				KEY_slash               = '/',
				KEY_colon               = ':',
				KEY_semicolon           = ';',
				KEY_less                = '<',
				KEY_equal               = '=',
				KEY_greater             = '>',
				KEY_question            = '?',
				KEY_at                  = '@',
				KEY_bracket_left        = '[',
				KEY_backslash           = '\\',
				KEY_bracket_right       = ']',
				KEY_carrot              = '^',
				KEY_underscore          = '_',
				KEY_backquote           = '`',
				KEY_brace_left          = '{',
				KEY_bar                 = '|',
				KEY_brace_right         = '}',
				KEY_tilde               = '~',

				//----------------------------------------------
				// Non-Printable (or non-US keyboard) characters
				//----------------------------------------------
				KEY_nobreakspace        = 0x0a0,
				KEY_exclamdown          = 0x0a1,
				KEY_cent                = 0x0a2,
				KEY_sterling            = 0x0a3,
				KEY_currency            = 0x0a4,
				KEY_yen                 = 0x0a5,
				KEY_brokenbar           = 0x0a6,
				KEY_section             = 0x0a7,
				KEY_diaeresis           = 0x0a8,
				KEY_copyright           = 0x0a9,
				KEY_ordfeminine         = 0x0aa,
				KEY_guillemotleft       = 0x0ab, /* left angle quotation mark */
				KEY_notsign             = 0x0ac,
				KEY_hyphen              = 0x0ad,
				KEY_registered          = 0x0ae,
				KEY_macron              = 0x0af,
				KEY_degree              = 0x0b0,
				KEY_plusminus           = 0x0b1,
				KEY_twosuperior         = 0x0b2,
				KEY_threesuperior       = 0x0b3,
				KEY_acute               = 0x0b4,
				KEY_mu                  = 0x0b5,
				KEY_paragraph           = 0x0b6,
				KEY_periodcentered      = 0x0b7,
				KEY_cedilla             = 0x0b8,
				KEY_onesuperior         = 0x0b9,
				KEY_masculine           = 0x0ba,
				KEY_guillemotright      = 0x0bb, /* right angle quotation mark */
				KEY_onequarter          = 0x0bc,
				KEY_onehalf             = 0x0bd,
				KEY_threequarters       = 0x0be,
				KEY_questiondown        = 0x0bf,
				KEY_Agrave              = 0x0c0,
				KEY_Aacute              = 0x0c1,
				KEY_Acircumflex         = 0x0c2,
				KEY_Atilde              = 0x0c3,
				KEY_Adiaeresis          = 0x0c4,
				KEY_Aring               = 0x0c5,
				KEY_AE                  = 0x0c6,
				KEY_Ccedilla            = 0x0c7,
				KEY_Egrave              = 0x0c8,
				KEY_Eacute              = 0x0c9,
				KEY_Ecircumflex         = 0x0ca,
				KEY_Ediaeresis          = 0x0cb,
				KEY_Igrave              = 0x0cc,
				KEY_Iacute              = 0x0cd,
				KEY_Icircumflex         = 0x0ce,
				KEY_Idiaeresis          = 0x0cf,
				KEY_ETH                 = 0x0d0,
				KEY_Ntilde              = 0x0d1,
				KEY_Ograve              = 0x0d2,
				KEY_Oacute              = 0x0d3,
				KEY_Ocircumflex         = 0x0d4,
				KEY_Otilde              = 0x0d5,
				KEY_Odiaeresis          = 0x0d6,
				KEY_multiply            = 0x0d7,
				KEY_Ooblique            = 0x0d8,
				KEY_Ugrave              = 0x0d9,
				KEY_Uacute              = 0x0da,
				KEY_Ucircumflex         = 0x0db,
				KEY_Udiaeresis          = 0x0dc,
				KEY_Yacute              = 0x0dd,
				KEY_THORN               = 0x0de,
				KEY_ssharp              = 0x0df,
				KEY_agrave              = 0x0e0,
				KEY_aacute              = 0x0e1,
				KEY_acircumflex         = 0x0e2,
				KEY_atilde              = 0x0e3,
				KEY_adiaeresis          = 0x0e4,
				KEY_aring               = 0x0e5,
				KEY_ae                  = 0x0e6,
				KEY_ccedilla            = 0x0e7,
				KEY_egrave              = 0x0e8,
				KEY_eacute              = 0x0e9,
				KEY_ecircumflex         = 0x0ea,
				KEY_ediaeresis          = 0x0eb,
				KEY_igrave              = 0x0ec,
				KEY_iacute              = 0x0ed,
				KEY_icircumflex         = 0x0ee,
				KEY_idiaeresis          = 0x0ef,
				KEY_eth                 = 0x0f0,
				KEY_ntilde              = 0x0f1,
				KEY_ograve              = 0x0f2,
				KEY_oacute              = 0x0f3,
				KEY_ocircumflex         = 0x0f4,
				KEY_otilde              = 0x0f5,
				KEY_odiaeresis          = 0x0f6,
				KEY_division            = 0x0f7,
				KEY_oslash              = 0x0f8,
				KEY_ugrave              = 0x0f9,
				KEY_uacute              = 0x0fa,
				KEY_ucircumflex         = 0x0fb,
				KEY_udiaeresis          = 0x0fc,
				KEY_yacute              = 0x0fd,
				KEY_thorn               = 0x0fe,
				KEY_ydiaeresis          = 0x0ff,

				// Function keys...
				KEY_F1                  = 0xFFBE,
				KEY_F2                  = 0xFFBF,
				KEY_F3                  = 0xFFC0,
				KEY_F4                  = 0xFFC1,
				KEY_F5                  = 0xFFC2,
				KEY_F6                  = 0xFFC3,
				KEY_F7                  = 0xFFC4,
				KEY_F8                  = 0xFFC5,
				KEY_F9                  = 0xFFC6,
				KEY_F10                 = 0xFFC7,
				KEY_F11                 = 0xFFC8,
				KEY_F12                 = 0xFFC9,
				KEY_F13                 = 0xFFCA,
				KEY_F14                 = 0xFFCB,
				KEY_F15                 = 0xFFCC,
				KEY_F16                 = 0xFFCD,
				KEY_F17                 = 0xFFCE,
				KEY_F18                 = 0xFFCF,
				KEY_F19                 = 0xFFD0,
				KEY_F20                 = 0xFFD1,
				KEY_F21                 = 0xFFD2,
				KEY_F22                 = 0xFFD3,
				KEY_F23                 = 0xFFD4,
				KEY_F24                 = 0xFFD5,
				KEY_F25                 = 0xFFD6,
				KEY_F26                 = 0xFFD7,
				KEY_F27                 = 0xFFD8,
				KEY_F28                 = 0xFFD9,
				KEY_F29                 = 0xFFDA,
				KEY_F30                 = 0xFFDB,
				KEY_F31                 = 0xFFDC,
				KEY_F32                 = 0xFFDD,
				KEY_F33                 = 0xFFDE,
				KEY_F34                 = 0xFFDF,
				KEY_F35                 = 0xFFE0,

				KEY_L1                  = 0xFFC8,
				KEY_L2                  = 0xFFC9,
				KEY_L3                  = 0xFFCA,
				KEY_L4                  = 0xFFCB,
				KEY_L5                  = 0xFFCC,
				KEY_L6                  = 0xFFCD,
				KEY_L7                  = 0xFFCE,
				KEY_L8                  = 0xFFCF,
				KEY_L9                  = 0xFFD0,
				KEY_L10                 = 0xFFD1,
				KEY_R1                  = 0xFFD2,
				KEY_R2                  = 0xFFD3,
				KEY_R3                  = 0xFFD4,
				KEY_R4                  = 0xFFD5,
				KEY_R5                  = 0xFFD6,
				KEY_R6                  = 0xFFD7,
				KEY_R7                  = 0xFFD8,
				KEY_R8                  = 0xFFD9,
				KEY_R9                  = 0xFFDA,
				KEY_R10                 = 0xFFDB,
				KEY_R11                 = 0xFFDC,
				KEY_R12                 = 0xFFDD,
				KEY_R13                 = 0xFFDE,
				KEY_R14                 = 0xFFDF,
				KEY_R15                 = 0xFFE0,

				//-----------------
				// Modifier keys...
				//-----------------
				KEY_shift_left          = 0xFFE1, /* Left shift */
				KEY_shift_right         = 0xFFE2, /* Right shift */
				KEY_control_left        = 0xFFE3, /* Left control */
				KEY_control_right       = 0xFFE4, /* Right control */
				KEY_caps_lock           = 0xFFE5, /* Caps lock */
				KEY_shift_Lock          = 0xFFE6, /* Shift lock */

				KEY_meta_left           = 0xFFE7, /* Left meta */
				KEY_meta_right          = 0xFFE8, /* Right meta */
				KEY_alt_left            = 0xFFE9, /* Left alt */
				KEY_alt_right           = 0xFFEA, /* Right alt */
				KEY_super_left          = 0xFFEB, /* Left super */
				KEY_super_right         = 0xFFEC, /* Right super */
				KEY_hyper_left          = 0xFFED, /* Left hyper */
				KEY_hyper_right         = 0xFFEE, /* Right hyper */


				//--------------------
				// Special keys
				//--------------------

				KEY_backspace           = 0xFF08, /* back space, back char */
				KEY_tab                 = 0xFF09,
				KEY_return              = 0xFF0D, /* Return, enter */

				KEY_insert              = 0xFF63, /* Insert, insert here */
				KEY_delete              = 0xFFFF, /* Delete, rubout */
				KEY_home                = 0xFF50,
				KEY_end                 = 0xFF57, /* EOL */
				KEY_page_up             = 0xFF55,
				KEY_page_down           = 0xFF56,

				KEY_escape              = 0xFF1B,
				KEY_print               = 0xFF61,
				KEY_sys_req             = 0xFF15,
				KEY_scroll_lock         = 0xFF14,
				KEY_pause               = 0xFF13, /* Pause, hold */
				KEY_break               = 0xFF6B,
				KEY_num_lock            = 0xFF7F,

				KEY_left                = 0xFF51, /* Move left, left arrow */
				KEY_right               = 0xFF53, /* Move right, right arrow */
				KEY_up                  = 0xFF52, /* Move up, up arrow */
				KEY_down                = 0xFF54, /* Move down, down arrow */

				// Not on my keyboard, but on some HPs, Suns, or SGIs, these exist.
				KEY_linefeed            = 0xFF0A, /* Linefeed, LF */
				KEY_clear               = 0xFF0B,
				KEY_prior               = 0xFF55, /* Prior, previous */
				KEY_next                = 0xFF56, /* Next */
				KEY_begin               = 0xFF58, /* BOL */
				KEY_select              = 0xFF60, /* Select, mark */
				KEY_execute             = 0xFF62, /* Execute, run, do */
				KEY_undo                = 0xFF65, /* Undo, oops */
				KEY_redo                = 0xFF66, /* redo, again */
				KEY_menu                = 0xFF67,
				KEY_find                = 0xFF68, /* Find, search */
				KEY_cancel              = 0xFF69, /* Cancel, stop, abort, exit */
				KEY_help                = 0xFF6A, /* Help */
				KEY_mode_switch         = 0xFF7E, /* Character set switch */
				KEY_script_switch       = 0xFF7E,  /* Alias for mode_switch */

				//--------------------
				// Keypad buttons
				//--------------------
				// Note: these (that are printable) can be translated to their ascii
				// equiv by anding by keypad_ascii_mask.  Make sure that anding them
				// with keypad_mask returns keypad_mask first!
				KEY_keypad_mask         = 0xFF80,
				KEY_keypad_ascii_mask   = 0x007F, /* and with these to get ascii */

				KEY_keypad_0            = 0xFFB0,
				KEY_keypad_1            = 0xFFB1,
				KEY_keypad_2            = 0xFFB2,
				KEY_keypad_3            = 0xFFB3,
				KEY_keypad_4            = 0xFFB4,
				KEY_keypad_5            = 0xFFB5,
				KEY_keypad_6            = 0xFFB6,
				KEY_keypad_7            = 0xFFB7,
				KEY_keypad_8            = 0xFFB8,
				KEY_keypad_9            = 0xFFB9,

				KEY_keypad_subtract     = 0xFFAD,
				KEY_keypad_multiply     = 0xFFAA,
				KEY_keypad_add          = 0xFFAB,
				KEY_keypad_divide       = 0xFFAF,
				KEY_keypad_decimal      = 0xFFAE,
				KEY_keypad_equal        = 0xFFBD, /* equals */

				KEY_keypad_left         = 0xFF96,
				KEY_keypad_right        = 0xFF98,
				KEY_keypad_up           = 0xFF97,
				KEY_keypad_down         = 0xFF99,
				KEY_keypad_center       = KEY_keypad_5,

				KEY_keypad_home         = 0xFF95,
				KEY_keypad_end          = 0xFF9C,
				KEY_keypad_page_up      = 0xFF9A,
				KEY_keypad_page_down    = 0xFF9B,
				KEY_keypad_insert       = 0xFF9E,
				KEY_keypad_delete       = 0xFF9F,
				KEY_keypad_enter        = 0xFF8D, /* enter */

				// These are less common...
				KEY_keypad_space        = 0xFF80, /* space */
				KEY_keypad_tab          = 0xFF89,
				KEY_keypad_F1           = 0xFF91, /* PF1, keypad_A, ... */
				KEY_keypad_F2           = 0xFF92,
				KEY_keypad_F3           = 0xFF93,
				KEY_keypad_F4           = 0xFF94,
				KEY_keypad_prior        = 0xFF9A,
				KEY_keypad_next         = 0xFF9B,
				KEY_keypad_begin        = 0xFF9D,
				KEY_keypad_separator    = 0xFFAC,  /* separator, often comma */

				// Media keys
				KEY_volume_up = 0x1008ff13,
				KEY_volume_down = 0x1008ff11,
				KEY_volume_mute = 0x1008ff12,
				KEY_media_next = 0x1008ff17,
				KEY_media_previous = 0x1008ff16,
				KEY_media_rewind = 0x1008ff3e,
				KEY_media_forward = 0x1008ff97,
				KEY_media_stop = 0x1008ff15,
				KEY_media_play = 0x1008ff14,
				KEY_media_pause = 0x1008ff31, // Don't rely on this too much.
				KEY_media_audio = 0x1008ff32, // Don't know what this is for, but I'm hooking it in.

				// Random keys
				KEY_power = 0x1008ff2a,
				KEY_mail = 0x1008ff19,
				KEY_camera = 0x1008ff8f,
				KEY_explore_computer = 0x1008ff33,

				// Keys which may have no other mapping
				KEY_focus = 0x10000,
				KEY_notification,
				KEY_start_call,
				KEY_end_call,
				KEY_alt_symbol1,
				KEY_alt_symbol2,
			};


	}; // class KeyboardInput
}

#endif /* !defined(GRAPHLIB_KEYBOARD_INPUT_HPP) */
