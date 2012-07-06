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

#ifndef engine_Resource_hpp_INCLUDED_
#define engine_Resource_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/Array.hpp"
#include <string>

namespace engine
{

class Resource : public virtual IntrusiveCountableBase
{
public:
	Resource(const Array<UInt8> data, const std::string& name);

	Array<UInt8>& data();
	typedef Array<UInt8>::const_iterator const_iterator;

	const_iterator begin() const;
	const_iterator end() const;

	const std::string& name() const;
	~Resource();
private:
	// TODO: replace this with an iostream that reads from the .apk
	Array<UInt8> data_;
	std::string name_;
};

}

#endif
