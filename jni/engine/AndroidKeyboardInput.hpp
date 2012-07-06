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

#if !defined(ANDROID_KEYBOARD_INPUT_HPP_INCLUDED)
#define ANDROID_KEYBOARD_INPUT_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "graphlib/KeyboardInput.hpp"
#include "miniblocxx/Exception.hpp"

namespace engine
{
	BLOCXX_DECLARE_EXCEPTION(Keyboard);

	class AndroidKeyboardInput : public KeyboardInput
	{
	public:
		AndroidKeyboardInput();
		virtual ~AndroidKeyboardInput();

		// Functions to set the state of the keyboard.  All of the useful
		// callbacks and state checking are handled in the base class.
		void keyDown(int keycode, int metastate);
		void keyUp(int keycode, int metastate);

		// Translate from the android keycode
		static KeyboardInput::KeyboardKeys translateKeycode(int keycode);
		static KeyboardInput::KeyboardModifiers translateMetastate(int metastate);
	};
}

#endif
