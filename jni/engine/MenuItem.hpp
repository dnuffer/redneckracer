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

#ifndef MENU_ITEM_HPP_INCLUDED
#define MENU_ITEM_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "miniblocxx/Function.hpp"
#include "miniblocxx/String.hpp"

namespace engine
{
class MenuItem : public Drawable
{
public:
	MenuItem(const char* text, const Function0<void>& selectHandler)
		: _text(text)
		, _selectHandler(selectHandler)
	{}

	virtual void draw(const Rectangle& screen);
private:
	String _text;
	Function0<void> _selectHandler;
};
}

#endif
