/*
 * AndroidKeyboardInputTests.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: Matthew Ricks
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "graphlib/KeyboardInput.hpp"
#include "engine/AndroidKeyboardInput.hpp"

using namespace blocxx;
using namespace engine;

const KeyboardInput::KeyboardKeys keysAnswers[] = {
	KeyboardInput::KEY_unknown,
	KeyboardInput::KEY_left,
	KeyboardInput::KEY_right,
	KeyboardInput::KEY_home,
	KeyboardInput::KEY_escape,
	KeyboardInput::KEY_start_call,
	KeyboardInput::KEY_end_call,
	KeyboardInput::KEY_0,
	KeyboardInput::KEY_1,
	KeyboardInput::KEY_2,
	KeyboardInput::KEY_3,
	KeyboardInput::KEY_4,
	KeyboardInput::KEY_5,
	KeyboardInput::KEY_6,
	KeyboardInput::KEY_7,
	KeyboardInput::KEY_8,
	KeyboardInput::KEY_9,
	KeyboardInput::KEY_asterisk,
	KeyboardInput::KEY_numbersign,
	KeyboardInput::KEY_keypad_up,
	KeyboardInput::KEY_keypad_down,
	KeyboardInput::KEY_keypad_left,
	KeyboardInput::KEY_keypad_right,
	KeyboardInput::KEY_keypad_center,
	KeyboardInput::KEY_volume_up,
	KeyboardInput::KEY_volume_down,
	KeyboardInput::KEY_power,
	KeyboardInput::KEY_camera,
	KeyboardInput::KEY_clear,
	KeyboardInput::KEY_a,
	KeyboardInput::KEY_b,
	KeyboardInput::KEY_c,
	KeyboardInput::KEY_d,
	KeyboardInput::KEY_e,
	KeyboardInput::KEY_f,
	KeyboardInput::KEY_g,
	KeyboardInput::KEY_h,
	KeyboardInput::KEY_i,
	KeyboardInput::KEY_j,
	KeyboardInput::KEY_k,
	KeyboardInput::KEY_l,
	KeyboardInput::KEY_m,
	KeyboardInput::KEY_n,
	KeyboardInput::KEY_o,
	KeyboardInput::KEY_p,
	KeyboardInput::KEY_q,
	KeyboardInput::KEY_r,
	KeyboardInput::KEY_s,
	KeyboardInput::KEY_t,
	KeyboardInput::KEY_u,
	KeyboardInput::KEY_v,
	KeyboardInput::KEY_w,
	KeyboardInput::KEY_x,
	KeyboardInput::KEY_y,
	KeyboardInput::KEY_z,
	KeyboardInput::KEY_comma,
	KeyboardInput::KEY_period,
	KeyboardInput::KEY_alt_left,
	KeyboardInput::KEY_alt_right,
	KeyboardInput::KEY_shift_left,
	KeyboardInput::KEY_shift_right,
	KeyboardInput::KEY_tab,
	KeyboardInput::KEY_space,
	KeyboardInput::KEY_alt_symbol1,
	KeyboardInput::KEY_explore_computer,
	KeyboardInput::KEY_mail,
	KeyboardInput::KEY_return,
	KeyboardInput::KEY_delete,
	KeyboardInput::KEY_backquote,
	KeyboardInput::KEY_minus,
	KeyboardInput::KEY_equal,
	KeyboardInput::KEY_bracket_left,
	KeyboardInput::KEY_bracket_right,
	KeyboardInput::KEY_backslash,
	KeyboardInput::KEY_semicolon,
	KeyboardInput::KEY_apostrophe,
	KeyboardInput::KEY_slash,
	KeyboardInput::KEY_at,
	KeyboardInput::KEY_numbersign,
	KeyboardInput::KEY_media_audio,
	KeyboardInput::KEY_focus,
	KeyboardInput::KEY_plus,
	KeyboardInput::KEY_menu,
	KeyboardInput::KEY_notification,
	KeyboardInput::KEY_find,
	KeyboardInput::KEY_media_play,
	KeyboardInput::KEY_media_stop,
	KeyboardInput::KEY_media_next,
	KeyboardInput::KEY_media_previous,
	KeyboardInput::KEY_media_rewind,
	KeyboardInput::KEY_media_forward,
	KeyboardInput::KEY_volume_mute,
	KeyboardInput::KEY_page_up,
	KeyboardInput::KEY_page_down,
	KeyboardInput::KEY_alt_symbol2,
	KeyboardInput::KEY_mode_switch,
};

const KeyboardInput::KeyboardModifiers modifiersAnswers[] = {
	KeyboardInput::keyboard_alt,
	KeyboardInput::keyboard_left_alt,
	KeyboardInput::keyboard_shift,
	KeyboardInput::keyboard_left_shift,
	KeyboardInput::keyboard_right_shift
};

AUTO_UNIT_TEST(AndroidKeyboardInputTranslateKeycode)
{
	// Variable
	AndroidKeyboardInput keyboard = AndroidKeyboardInput();
	
	// Tests
	for(int code = 0; code != 96; code++)
	{
		unitAssert(keyboard.translateKeycode(code) == keysAnswers[code]);
	}

	// I want to have a test case for the exception,
	//  but I'm not sure how to write it with this code
	//  because we'd have to catch a BLOCXX exception.
	// I recommend testing -1 and 96 for exceptions.
}

AUTO_UNIT_TEST(AndroidKeyboardInputTranslateMetastate)
{
	// Variable
	AndroidKeyboardInput keyboard = AndroidKeyboardInput();
	
	// Tests (this is everything we've accounted for)
	unitAssert(keyboard.translateMetastate(2) == modifiersAnswers[0]);
	unitAssert(keyboard.translateMetastate(16) == modifiersAnswers[1]);
	unitAssert(keyboard.translateMetastate(1) == modifiersAnswers[2]);
	unitAssert(keyboard.translateMetastate(64) == modifiersAnswers[3]);
	unitAssert(keyboard.translateMetastate(128) == modifiersAnswers[4]);
}
