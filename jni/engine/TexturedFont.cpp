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

#include "TexturedFont.hpp"
#include "miniblocxx/SortedVectorSet.hpp"

namespace engine
{
	using namespace blocxx;

	TexturedFont::TexturedFont(const TextureLibraryPtr& library, const String& name)
		: m_textureLibrary(library)
		, m_baseFontPath("fonts/" + name)
	{
	}

	TexturedQuadPtr TexturedFont::getQuadForCharacter(char c) const
	{
		return m_textureLibrary->texturedQuad(Format("%1/%<2:03>", m_baseFontPath, int(c)).toString());
	}

	StringArray findFonts(const TextureLibraryPtr& library)
	{
		const String fonts("fonts/");
		StringArray fontQuads = library->listQuads(fonts);
		SortedVectorSet<String> uniqueFontNames;

		for( StringArray::const_iterator i = fontQuads.begin(); i != fontQuads.end(); ++i )
		{
			String name = i->substring(fonts.length(), i->lastIndexOf('/') - fonts.length());
			uniqueFontNames.insert(name);
		}
		return StringArray(uniqueFontNames.begin(), uniqueFontNames.end());
	}
}
