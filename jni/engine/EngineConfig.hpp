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

#ifndef EngineConfig_HPP_INCLUDED
#define EngineConfig_HPP_INCLUDED

#include "miniblocxx/BLOCXX_config.h"

#ifdef HAVE_CONFIG_H
#include "config.hpp"
#endif

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

namespace graphlib
{

}

namespace engine
{
using namespace blocxx;
using namespace blocxx::Time;
using namespace graphlib;
using namespace std;
}

#endif
