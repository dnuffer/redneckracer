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

#if !defined(TEXTURED_FONT_HPP_INCLUDED)
#define TEXTURED_FONT_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "TextureLibrary.hpp"
#include "miniblocxx/String.hpp"
#include "miniblocxx/Array.hpp"

namespace engine
{
	class TexturedFont : public blocxx::IntrusiveCountableBase
	{
	public:
		TexturedFont(const TextureLibraryPtr& library, const blocxx::String& name);

		virtual TexturedQuadPtr getQuadForCharacter(char c) const;
	private:
		TextureLibraryPtr m_textureLibrary;
		blocxx::String m_baseFontPath;
	};

	blocxx::StringArray findFonts(const TextureLibraryPtr& library);
}

#endif
