/*
 * KeyboardInputTests.cpp
 *
 *  Created on: Jun 22, 2011
 *      Author: Matthew
 */

#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "graphlib/KeyboardInput.hpp"

using namespace graphlib;
using namespace blocxx;

AUTO_UNIT_TEST(KeyboardInputKeynameAndOverrideKeyname)
{
	// Variable
	KeyboardInput keyboard;
	
	char answer[2];
	answer[1] = '\0';
	for(unsigned letter = 'a'; letter != 'z'; letter++)
	{
		answer[0] = letter;
		blocxx::String ans(answer);
		unitAssert(ans == keyboard.keyname(letter));
	}
	for(unsigned letter = 'A'; letter != 'Z'; letter++)
	{
		answer[0] = letter;
		blocxx::String ans(answer);
		unitAssert(ans == keyboard.keyname(letter));
	}
	blocxx::String letter("ZZZ");
	keyboard.override_keyname('Z', letter);
	unitAssert("Z" != keyboard.keyname('Z') && letter == keyboard.keyname('Z'));
}

AUTO_UNIT_TEST(KeyboardInputModifierFunctions)
{
	// Variable
	KeyboardInput keyboard;
	
	unitAssert(0 == keyboard.get_modifier_ignore_mask());
	keyboard.add_modifier_ignore_mask(1);
	unitAssert(0 != keyboard.get_modifier_ignore_mask());
	unitAssert(1 == keyboard.get_modifier_ignore_mask());
	keyboard.clear_modifier_ignore_mask();
	unitAssert(1 != keyboard.get_modifier_ignore_mask());
	unitAssert(0 == keyboard.get_modifier_ignore_mask());
}

// TODO
/*AUTO_UNIT_TEST(KeyboardInputHandleKeyboardEvent)
{
	// Variable
	KeyboardInput keyboard;
}*/
