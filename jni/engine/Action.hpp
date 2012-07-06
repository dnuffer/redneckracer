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

#ifndef engine_Action_hpp_INCLUDED_
#define engine_Action_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "Drawable.hpp"

namespace engine
{

class Action : public virtual IntrusiveCountableBase
{
public:
	virtual void apply(const DrawablePtr& target, const DateTime& thisFrameStartTime, const TimeDuration& deltaTime) = 0;
};

}

#endif
