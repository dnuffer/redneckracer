/*
 * $Id: KeyboardInput.cpp,v 1.4 2010/08/31 04:10:09 kpharris Exp $
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

#include "KeyboardInput.hpp"
#include <stdio.h>

namespace graphlib
{
	// No internationalization yet
#define intl(x) x

	namespace
	{
		bool stub_pressed(unsigned key, unsigned modifiers)
		{
			return false;
		}
		bool stub_released(unsigned key, unsigned press_modifiers, unsigned release_modifiers)
		{
			return false;
		}
	}

	KeyboardInput::KeyboardInput():
		allow_repeat(true),
		modifier_ignore_mask(0),
		keyname_overrides_with_modifier(),
		keyname_overrides(),
		modifier_name_overrides(),
		key_pressed(blocxx::ptrfun(&stub_pressed)),
		key_released(blocxx::ptrfun(&stub_released))
	{

	} // KeyboardInput()

	KeyboardInput::~KeyboardInput()
	{

	} // ~KeyboardInput()

	KeyboardInput::KeyboardInput(const KeyboardInput& old) :
		allow_repeat(old.allow_repeat),
		modifier_ignore_mask(old.modifier_ignore_mask),
		buttons_currently_held(old.buttons_currently_held),
		keyname_overrides_with_modifier(old.keyname_overrides_with_modifier),
		keyname_overrides(old.keyname_overrides),
		modifier_name_overrides(old.modifier_name_overrides),
		key_pressed(old.key_pressed),
		key_released(old.key_released)
	{
	} // KeyboardInput(KeyboardInput)

	KeyboardInput& KeyboardInput::operator= (const KeyboardInput& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{

			modifier_ignore_mask = old.modifier_ignore_mask;
			allow_repeat = old.allow_repeat;
			buttons_currently_held = old.buttons_currently_held;

			keyname_overrides_with_modifier = old.keyname_overrides_with_modifier;
			keyname_overrides = old.keyname_overrides;
			modifier_name_overrides = old.modifier_name_overrides;

		}
		return (*this);
	} // KeyboardInput::operator=(KeyboardInput)


	bool KeyboardInput::handle_keyboard_event(unsigned button, unsigned modifiers)
	{
		std::map<unsigned,unsigned>::iterator current_location = buttons_currently_held.find(button);

		// Not previously held down...
		if( current_location == buttons_currently_held.end() )
		{
			if( modifiers & KeyboardInput::keyboard_key_pressed )
			{
				buttons_currently_held[button] = modifiers;
				return key_pressed(button, modifiers);
			}
			else
			{
				// CHECKME!
				// Should the released signal be relayed, even though there were no buttons down?
				return key_released(button, 0, modifiers);
			}
		}
		else if( modifiers & KeyboardInput::keyboard_key_pressed ) // down, but repeated
		{
			if( allow_repeat )
			{
				buttons_currently_held[button] = modifiers;
				return key_pressed(button, modifiers);
			}
			return true;
		}
		else // currently down, and being released...
		{
			unsigned press_mods = current_location->second;

			buttons_currently_held.erase(current_location);

			return key_released(button, press_mods, modifiers);
		}
	} // handle_keyboard_event()

	bool KeyboardInput::key_down(unsigned key) const
	{
		return buttons_currently_held.find(key) != buttons_currently_held.end();
	}

	bool KeyboardInput::key_down(unsigned key, unsigned& modifiers) const
	{
		std::map<unsigned,unsigned>::const_iterator i = buttons_currently_held.find(key);

		if( i != buttons_currently_held.end() )
		{
			modifiers = i->second;
			return true;
		}
		return false;
	}

	unsigned KeyboardInput::num_keys_down() const
	{
		return buttons_currently_held.size();
	}

	std::vector<std::pair<unsigned,unsigned> > KeyboardInput::get_all_held_keys() const
	{

		std::vector<std::pair<unsigned,unsigned> > retvec(buttons_currently_held.begin(),
			buttons_currently_held.end());
		return retvec;
	}

#include <ctype.h>

	blocxx::String get_key_name(unsigned key)
	{
		// I noticed that the isprint function (or macro) ignores the upper bits,
		// so I have to qualify the if statement, checking to see if it is truly an
		// ascii printable character
		if( (key < 0x7F) && isprint(key) )
		{
			char s[2];
			s[0] = (char)key;
			s[1] = '\0';
			return blocxx::String(s);
		}

		//----------------------------------------------
		// Non-Printable (or non-US keyboard) characters
		//----------------------------------------------
		switch(key)
		{
		case KeyboardInput::KEY_unknown:        return intl("Unknown");
		case KeyboardInput::KEY_backspace:      return intl("Backspace");
		case KeyboardInput::KEY_tab:            return intl("Tab");
		case KeyboardInput::KEY_return:         return intl("Return");

		case KeyboardInput::KEY_insert:         return intl("Insert");
		case KeyboardInput::KEY_delete:         return intl("Delete");
		case KeyboardInput::KEY_home:           return intl("Home");
		case KeyboardInput::KEY_end:            return intl("End");
		case KeyboardInput::KEY_page_up:        return intl("PageUp");
		case KeyboardInput::KEY_page_down:      return intl("PageDown");

		case KeyboardInput::KEY_escape:         return intl("Escape");
		case KeyboardInput::KEY_print:          return intl("Print");
		case KeyboardInput::KEY_sys_req:        return intl("SysReq");
		case KeyboardInput::KEY_scroll_lock:    return intl("ScrollLock");
		case KeyboardInput::KEY_pause:          return intl("Pause");
		case KeyboardInput::KEY_break:          return intl("Break");
		case KeyboardInput::KEY_num_lock:       return intl("NumLock");

		case KeyboardInput::KEY_left:           return intl("Left");
		case KeyboardInput::KEY_right:          return intl("Right");
		case KeyboardInput::KEY_up:             return intl("Up");
		case KeyboardInput::KEY_down:           return intl("Down");


			// Function keys...:
		case KeyboardInput::KEY_F1:             return intl("F1");
		case KeyboardInput::KEY_F2:             return intl("F2");
		case KeyboardInput::KEY_F3:             return intl("F3");
		case KeyboardInput::KEY_F4:             return intl("F4");
		case KeyboardInput::KEY_F5:             return intl("F5");
		case KeyboardInput::KEY_F6:             return intl("F6");
		case KeyboardInput::KEY_F7:             return intl("F7");
		case KeyboardInput::KEY_F8:             return intl("F8");
		case KeyboardInput::KEY_F9:             return intl("F9");
		case KeyboardInput::KEY_F10:            return intl("F10");
		case KeyboardInput::KEY_F11:            return intl("F11");
		case KeyboardInput::KEY_F12:            return intl("F12");
		case KeyboardInput::KEY_F13:            return intl("F13");
		case KeyboardInput::KEY_F14:            return intl("F14");
		case KeyboardInput::KEY_F15:            return intl("F15");
		case KeyboardInput::KEY_F16:            return intl("F16");
		case KeyboardInput::KEY_F17:            return intl("F17");
		case KeyboardInput::KEY_F18:            return intl("F18");
		case KeyboardInput::KEY_F19:            return intl("F19");
		case KeyboardInput::KEY_F20:            return intl("F20");
		case KeyboardInput::KEY_F21:            return intl("F21");
		case KeyboardInput::KEY_F22:            return intl("F22");
		case KeyboardInput::KEY_F23:            return intl("F23");
		case KeyboardInput::KEY_F24:            return intl("F24");
		case KeyboardInput::KEY_F25:            return intl("F25");
		case KeyboardInput::KEY_F26:            return intl("F26");
		case KeyboardInput::KEY_F27:            return intl("F27");
		case KeyboardInput::KEY_F28:            return intl("F28");
		case KeyboardInput::KEY_F29:            return intl("F29");
		case KeyboardInput::KEY_F30:            return intl("F30");
		case KeyboardInput::KEY_F31:            return intl("F31");
		case KeyboardInput::KEY_F32:            return intl("F32");
		case KeyboardInput::KEY_F33:            return intl("F33");
		case KeyboardInput::KEY_F34:            return intl("F34");
		case KeyboardInput::KEY_F35:            return intl("F35");

			//-----------------
			// Modifier keys...
			//-----------------
		case KeyboardInput::KEY_shift_left:     return intl("Shift-L");
		case KeyboardInput::KEY_shift_right:    return intl("Shift-R");
		case KeyboardInput::KEY_control_left:   return intl("Control-L");
		case KeyboardInput::KEY_control_right:  return intl("Control-R");
		case KeyboardInput::KEY_caps_lock:      return intl("CapsLock");
		case KeyboardInput::KEY_shift_Lock:     return intl("ShiftLock");

		case KeyboardInput::KEY_meta_left:      return intl("Meta-L");
		case KeyboardInput::KEY_meta_right:     return intl("Meta-R");
		case KeyboardInput::KEY_alt_left:       return intl("Alt-L");
		case KeyboardInput::KEY_alt_right:      return intl("Alt-R");
		case KeyboardInput::KEY_super_left:     return intl("Super-L");
		case KeyboardInput::KEY_super_right:    return intl("Super-R");
		case KeyboardInput::KEY_hyper_left:     return intl("Hyper-L");
		case KeyboardInput::KEY_hyper_right:    return intl("Hyper-R");


			// Keypad keys
		case KeyboardInput::KEY_keypad_0:         return intl("kp_0");
		case KeyboardInput::KEY_keypad_1:         return intl("kp_1");
		case KeyboardInput::KEY_keypad_2:         return intl("kp_2");
		case KeyboardInput::KEY_keypad_3:         return intl("kp_3");
		case KeyboardInput::KEY_keypad_4:         return intl("kp_4");
		case KeyboardInput::KEY_keypad_5:         return intl("kp_5");
		case KeyboardInput::KEY_keypad_6:         return intl("kp_6");
		case KeyboardInput::KEY_keypad_7:         return intl("kp_7");
		case KeyboardInput::KEY_keypad_8:         return intl("kp_8");
		case KeyboardInput::KEY_keypad_9:         return intl("kp_9");

		case KeyboardInput::KEY_keypad_subtract:  return intl("kp_Subtract");
		case KeyboardInput::KEY_keypad_multiply:  return intl("kp_Multiply");
		case KeyboardInput::KEY_keypad_add:       return intl("kp_Add");
		case KeyboardInput::KEY_keypad_divide:    return intl("kp_Divide");
		case KeyboardInput::KEY_keypad_decimal:   return intl("kp_Decimal");
		case KeyboardInput::KEY_keypad_equal:     return intl("kp_Equal");

		case KeyboardInput::KEY_keypad_left:      return intl("kp_Left");
		case KeyboardInput::KEY_keypad_right:     return intl("kp_Right");
		case KeyboardInput::KEY_keypad_up:        return intl("kp_Up");
		case KeyboardInput::KEY_keypad_down:      return intl("kp_Down");

		case KeyboardInput::KEY_keypad_home:      return intl("kp_Home");
		case KeyboardInput::KEY_keypad_end:       return intl("kp_End");
		case KeyboardInput::KEY_keypad_page_up:   return intl("kp_PageUp");
		case KeyboardInput::KEY_keypad_page_down: return intl("kp_PageDown");
		case KeyboardInput::KEY_keypad_insert:    return intl("kp_Insert");
		case KeyboardInput::KEY_keypad_delete:    return intl("kp_Delete");
		case KeyboardInput::KEY_keypad_enter:     return intl("kp_Enter");

			//These are less common...
		case KeyboardInput::KEY_keypad_space:     return intl("kp_space");
		case KeyboardInput::KEY_keypad_tab:       return intl("kp_tab");
		case KeyboardInput::KEY_keypad_F1:        return intl("kp_F1");
		case KeyboardInput::KEY_keypad_F2:        return intl("kp_F2");
		case KeyboardInput::KEY_keypad_F3:        return intl("kp_F3");
		case KeyboardInput::KEY_keypad_F4:        return intl("kp_F4");
		case KeyboardInput::KEY_keypad_begin:     return intl("kp_begin");
		case KeyboardInput::KEY_keypad_separator: return intl("kp_separator");

			// Buttons I don't have, but others might...
		case KeyboardInput::KEY_nobreakspace:   return intl("nobreakspace");
		case KeyboardInput::KEY_exclamdown:     return intl("exclamdown");
		case KeyboardInput::KEY_cent:           return intl("cent");
		case KeyboardInput::KEY_sterling:       return intl("sterling");
		case KeyboardInput::KEY_currency:       return intl("currency");
		case KeyboardInput::KEY_yen:            return intl("yen");
		case KeyboardInput::KEY_brokenbar:      return intl("brokenbar");
		case KeyboardInput::KEY_section:        return intl("section");
		case KeyboardInput::KEY_diaeresis:      return intl("diaeresis");
		case KeyboardInput::KEY_copyright:      return intl("copyright");
		case KeyboardInput::KEY_registered:     return intl("registered");
		case KeyboardInput::KEY_ordfeminine:    return intl("ordfeminine");
		case KeyboardInput::KEY_guillemotleft:  return intl("guillemoleft");
		case KeyboardInput::KEY_notsign:        return intl("notsign");
		case KeyboardInput::KEY_hyphen:         return intl("hyphen");
		case KeyboardInput::KEY_macron:         return intl("macron");
		case KeyboardInput::KEY_degree:         return intl("degree");
		case KeyboardInput::KEY_plusminus:      return intl("plusminus");
		case KeyboardInput::KEY_onesuperior:    return intl("onesuperior");
		case KeyboardInput::KEY_twosuperior:    return intl("twosuperior");
		case KeyboardInput::KEY_threesuperior:  return intl("threesuperior");
		case KeyboardInput::KEY_acute:          return intl("acute");
		case KeyboardInput::KEY_mu:             return intl("mu");
		case KeyboardInput::KEY_paragraph:      return intl("paragraph");
		case KeyboardInput::KEY_periodcentered: return intl("periodcentered");
		case KeyboardInput::KEY_cedilla:        return intl("cedilla");
		case KeyboardInput::KEY_masculine:      return intl("masculine");
		case KeyboardInput::KEY_guillemotright: return intl("guillemotright");
		case KeyboardInput::KEY_onequarter:     return intl("onequarter");
		case KeyboardInput::KEY_onehalf:        return intl("onehalf");
		case KeyboardInput::KEY_threequarters:  return intl("threequarters");
		case KeyboardInput::KEY_questiondown:   return intl("questiondown");
		case KeyboardInput::KEY_Agrave:         return intl("Agrave");
		case KeyboardInput::KEY_Aacute:         return intl("Aacute");
		case KeyboardInput::KEY_Acircumflex:    return intl("Acircumflex");
		case KeyboardInput::KEY_Atilde:         return intl("Atilde");
		case KeyboardInput::KEY_Adiaeresis:     return intl("Adiaeresis");
		case KeyboardInput::KEY_Aring:          return intl("Aring");
		case KeyboardInput::KEY_AE:             return intl("AE");
		case KeyboardInput::KEY_Ccedilla:       return intl("Ccedilla");
		case KeyboardInput::KEY_Egrave:         return intl("Egrave");
		case KeyboardInput::KEY_Eacute:         return intl("Eacute");
		case KeyboardInput::KEY_Ecircumflex:    return intl("Ecircumflex");
		case KeyboardInput::KEY_Ediaeresis:     return intl("Ediaeresis");
		case KeyboardInput::KEY_Igrave:         return intl("Igrave");
		case KeyboardInput::KEY_Iacute:         return intl("Iacute");
		case KeyboardInput::KEY_Icircumflex:    return intl("Icircumflex");
		case KeyboardInput::KEY_Idiaeresis:     return intl("Idiaeresis");
		case KeyboardInput::KEY_ETH:            return intl("ETH");
		case KeyboardInput::KEY_Ntilde:         return intl("Ntilde");
		case KeyboardInput::KEY_Ograve:         return intl("Ograve");
		case KeyboardInput::KEY_Oacute:         return intl("Oacute");
		case KeyboardInput::KEY_Ocircumflex:    return intl("Ocircumflex");
		case KeyboardInput::KEY_Otilde:         return intl("Otilde");
		case KeyboardInput::KEY_Odiaeresis:     return intl("Odiaeresis");
		case KeyboardInput::KEY_multiply:       return intl("multiply");
		case KeyboardInput::KEY_Ooblique:       return intl("Ooblique");
		case KeyboardInput::KEY_Ugrave:         return intl("Ugrave");
		case KeyboardInput::KEY_Uacute:         return intl("Uacute");
		case KeyboardInput::KEY_Ucircumflex:    return intl("Ucircumflex");
		case KeyboardInput::KEY_Udiaeresis:     return intl("Udiaeresis");
		case KeyboardInput::KEY_Yacute:         return intl("Yacute");
		case KeyboardInput::KEY_THORN:          return intl("THORN");
		case KeyboardInput::KEY_ssharp:         return intl("ssharp");
		case KeyboardInput::KEY_agrave:         return intl("agrave");
		case KeyboardInput::KEY_aacute:         return intl("aacute");
		case KeyboardInput::KEY_acircumflex:    return intl("acircumflex");
		case KeyboardInput::KEY_atilde:         return intl("atilde");
		case KeyboardInput::KEY_adiaeresis:     return intl("adiaeresis");
		case KeyboardInput::KEY_aring:          return intl("aring");
		case KeyboardInput::KEY_ae:             return intl("ae");
		case KeyboardInput::KEY_ccedilla:       return intl("ccedilla");
		case KeyboardInput::KEY_egrave:         return intl("egrave");
		case KeyboardInput::KEY_eacute:         return intl("eacute");
		case KeyboardInput::KEY_ecircumflex:    return intl("ecircumflex");
		case KeyboardInput::KEY_ediaeresis:     return intl("ediaeresis");
		case KeyboardInput::KEY_igrave:         return intl("igrave");
		case KeyboardInput::KEY_iacute:         return intl("iacute");
		case KeyboardInput::KEY_icircumflex:    return intl("icircumflex");
		case KeyboardInput::KEY_idiaeresis:     return intl("idiaeresis");
		case KeyboardInput::KEY_eth:            return intl("eth");
		case KeyboardInput::KEY_ntilde:         return intl("ntilde");
		case KeyboardInput::KEY_ograve:         return intl("ograve");
		case KeyboardInput::KEY_oacute:         return intl("oacute");
		case KeyboardInput::KEY_ocircumflex:    return intl("ocircumflex");
		case KeyboardInput::KEY_otilde:         return intl("otilde");
		case KeyboardInput::KEY_odiaeresis:     return intl("odiaeresis");
		case KeyboardInput::KEY_division:       return intl("division");
		case KeyboardInput::KEY_oslash:         return intl("oslash");
		case KeyboardInput::KEY_ugrave:         return intl("ugrave");
		case KeyboardInput::KEY_uacute:         return intl("uacute");
		case KeyboardInput::KEY_ucircumflex:    return intl("ucircumflex");
		case KeyboardInput::KEY_udiaeresis:     return intl("udiaeresis");
		case KeyboardInput::KEY_yacute:         return intl("yacute");
		case KeyboardInput::KEY_thorn:          return intl("thorn");
		case KeyboardInput::KEY_ydiaeresis:     return intl("ydiaeresis");

			// Not on my keyboard, but on some HPs, Suns, or SGIs, these exist.
		case KeyboardInput::KEY_linefeed:         return intl("Linefeed");
		case KeyboardInput::KEY_clear:            return intl("Clear");
		case KeyboardInput::KEY_begin:            return intl("Begin");
		case KeyboardInput::KEY_select:           return intl("Select");
		case KeyboardInput::KEY_execute:          return intl("Execute");
		case KeyboardInput::KEY_undo:             return intl("Undo");
		case KeyboardInput::KEY_redo:             return intl("Redo");
		case KeyboardInput::KEY_menu:             return intl("Menu");
		case KeyboardInput::KEY_find:             return intl("Find");
		case KeyboardInput::KEY_cancel:           return intl("Cancel");
		case KeyboardInput::KEY_help:             return intl("Help");
		case KeyboardInput::KEY_mode_switch:      return intl("ModeSwitch");


		case KeyboardInput::KEY_volume_up: return intl("VolumeUp");
		case KeyboardInput::KEY_volume_down: return intl("VolumeDown");
		case KeyboardInput::KEY_volume_mute: return intl("VolumeMute");
		case KeyboardInput::KEY_media_next: return intl("MediaNext");
		case KeyboardInput::KEY_media_previous: return intl("MediaPrevious");
		case KeyboardInput::KEY_media_rewind: return intl("MediaRewind");
		case KeyboardInput::KEY_media_forward: return intl("MediaForward");
		case KeyboardInput::KEY_media_stop: return intl("MediaStop");
		case KeyboardInput::KEY_media_play: return intl("MediaPlay");
		case KeyboardInput::KEY_media_pause: return intl("MediaPause");
		case KeyboardInput::KEY_media_audio: return intl("MediaAudio");

		case KeyboardInput::KEY_power: return intl("Power");
		case KeyboardInput::KEY_mail: return intl("Mail");
		case KeyboardInput::KEY_camera: return intl("Camera");
		case KeyboardInput::KEY_explore_computer: return intl("ExploreComputer");

		case KeyboardInput::KEY_focus: return intl("Focus");
		case KeyboardInput::KEY_notification:  return intl("Notification");
		case KeyboardInput::KEY_start_call: return intl("StartCall");
		case KeyboardInput::KEY_end_call: return intl("EndCall");
		case KeyboardInput::KEY_alt_symbol1: return intl("AltSym1");
		case KeyboardInput::KEY_alt_symbol2: return intl("AltSym2");
		}

		//FIXME! Add in everything else...
		char buffer[128];
		snprintf(buffer, 128, "Unknown-0x%04x", key);

		return buffer;
	}

	blocxx::String get_mod_names(unsigned key, unsigned modifiers, const blocxx::String& merger = "+", bool shortened = false)
	{
		blocxx::String ret_string;
#define ADD_MERGER() if( !ret_string.empty() ) { ret_string += merger; }

		if( modifiers & KeyboardInput::keyboard_control )
		{
			if( (key != KeyboardInput::KEY_control_left) &&
				(key != KeyboardInput::KEY_control_right) )
			{
				ADD_MERGER();

				if( !modifiers & KeyboardInput::keyboard_right_control )
				{
					if( !shortened )
					{
						ret_string += "<Control-L>";
					}
					else
					{
						ret_string += "<CTRL-L>";
					}
				}
				else if( !modifiers & KeyboardInput::keyboard_left_control )
				{
					if( !shortened )
					{
						ret_string += "<Control-R>";
					}
					else
					{
						ret_string += "<CTRL-R>";
					}
				}
				else
				{
					if( !shortened )
					{
						ret_string += "<Control>";
					}
					else
					{
						ret_string += "<CTRL>";
					}
				}
			} // key NOT a control
		} // control modifier set

		if( modifiers & KeyboardInput::keyboard_shift )
		{
			if( (key != KeyboardInput::KEY_shift_left) &&
				(key != KeyboardInput::KEY_shift_right) )
			{
				ADD_MERGER();
				if( !modifiers & KeyboardInput::keyboard_right_shift )
				{
					if( !shortened )
					{
						ret_string += "<Shift-L>";
					}
					else
					{
						ret_string += "<SHFT-L>";
					}
				}
				else if( !modifiers & KeyboardInput::keyboard_left_shift )
				{
					if( !shortened )
					{
						ret_string += "<Shift-R>";
					}
					else
					{
						ret_string += "<SHFT-R>";
					}
				}
				else
				{
					if( !shortened )
					{
						ret_string += "<Shift>";
					}
					else
					{
						ret_string += "<SHFT>";
					}
				}
			} // key NOT a shift
		} // shift modifier set

		if( modifiers & KeyboardInput::keyboard_super )
		{
			if( (key != KeyboardInput::KEY_super_left) &&
				(key != KeyboardInput::KEY_super_right) )
			{
				ADD_MERGER();
				if( !modifiers & KeyboardInput::keyboard_right_super )
				{
					if( !shortened )
					{
						ret_string += "<Super-L>";
					}
					else
					{
						ret_string += "<SPR-L>";
					}
				}
				else if( !modifiers & KeyboardInput::keyboard_left_super )
				{
					if( !shortened )
					{
						ret_string += "<Super-R>";
					}
					else
					{
						ret_string += "<SPR-R>";
					}
				}
				else
				{
					if( !shortened )
					{
						ret_string += "<Super>";
					}
					else
					{
						ret_string += "<SPR>";
					}
				}
			} // key is NOT a super
		} // super modifier is set

		if( modifiers & KeyboardInput::keyboard_alt )
		{
			if( (key != KeyboardInput::KEY_alt_left) &&
				(key != KeyboardInput::KEY_alt_right) )
			{
				ADD_MERGER();
				if( !modifiers & KeyboardInput::keyboard_right_alt )
				{
					ret_string += "<Alt-L>";
				}
				else if( !modifiers & KeyboardInput::keyboard_left_alt )
				{
					ret_string += "<Alt-R>";
				}
				else
				{
					ret_string += "<Alt>";
				}
			} // key NOT an alt
		} // alt modifier set

		if( modifiers & KeyboardInput::keyboard_numlock )
		{
			ADD_MERGER();
			ret_string += "<Numlock>";
		}

		if( modifiers & KeyboardInput::keyboard_mod3 )
		{
			ADD_MERGER();
			ret_string += "<Mod3>";
		}

		if( modifiers & KeyboardInput::keyboard_mod5 )
		{
			ADD_MERGER();
			ret_string += "<Mod5>";
		}
		return ret_string;
	}

	blocxx::String KeyboardInput::keyname(unsigned key) const
	{
		std::map<unsigned, blocxx::String>::const_iterator override;

		override = keyname_overrides.find(key);

		if( override != keyname_overrides.end() )
		{
			return override->second;
		}

		// No override...
		return get_key_name(key);
	}

	blocxx::String KeyboardInput::keyname(unsigned key, unsigned modifiers, bool shortened) const
	{
		// Try to find it in the modded override map...
		key_and_mod km(key,modifiers);

		std::map<key_and_mod, blocxx::String>::const_iterator modded_override;

		modded_override = keyname_overrides_with_modifier.find(km);

		if( modded_override != keyname_overrides_with_modifier.end() )
		{
			// It was found, so just return the overridden name.
			return modded_override->second;
		}

		blocxx::String mod_names;
		// No override... Merge the modifiers and the normal keyname (even if it
		// has a non-modded override).
		if( modifiers & ~keyboard_key_pressed )
		{
			std::map<unsigned, blocxx::String>::const_iterator mod_overrides = modifier_name_overrides.find(modifiers&~keyboard_key_pressed);
			if( mod_overrides != modifier_name_overrides.end() )
			{
				mod_names = mod_overrides->second;
			}
			else
			{
				unsigned modifier_mask = ~modifier_ignore_mask & ~keyboard_key_pressed;

				if( !shortened )
				{
					mod_names = get_mod_names(key, modifiers & modifier_mask, "+", shortened);
				}
				else
				{
					// No "+" in the concatenation.
					mod_names = get_mod_names(key, modifiers & modifier_mask, "", shortened);
				}
			}
		}
		if( !mod_names.empty() )
		{
			return mod_names + "+" + keyname(key);
		}
		return keyname(key);
	}

	// Force the key name for the given key to be displayed as new_name
	// (only while the overridden class is still active).
	void KeyboardInput::override_keyname(unsigned key, const blocxx::String& new_name)
	{
		keyname_overrides[key] = new_name;
	}
	// Force the key name for the given key, ONLY when the given modifiers are attached.
	void KeyboardInput::override_keyname(unsigned key, unsigned modifiers, const blocxx::String& new_name)
	{
		keyname_overrides_with_modifier[key_and_mod(key,modifiers&~keyboard_key_pressed)] = new_name;
	}

	void KeyboardInput::override_modifier(unsigned modifier, const blocxx::String& new_name)
	{
		modifier_name_overrides[modifier&~keyboard_key_pressed] = new_name;
	}

	void KeyboardInput::clear_modifier_ignore_mask()
	{
		modifier_ignore_mask = 0;
	}

	void KeyboardInput::add_modifier_ignore_mask(unsigned mask)
	{
		modifier_ignore_mask |= mask;
	}

	unsigned KeyboardInput::get_modifier_ignore_mask() const
	{
		return modifier_ignore_mask;
	}


} // namespace graphlib

