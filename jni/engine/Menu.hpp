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

#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "Layer.hpp"

namespace engine
{

class Menu : public Layer
{
public:
	Menu(const MenuItem& item1);
	Menu(const MenuItem& item1, const MenuItem& item2);
	Menu(const MenuItem& item1, const MenuItem& item2, const MenuItem& item3);
	Menu(const MenuItem& item1, const MenuItem& item2, const MenuItem& item3, const MenuItem& item4);

	virtual void draw(const Rectangle& screen);
};

}

#endif
