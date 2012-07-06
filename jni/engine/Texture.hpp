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

#ifndef engine_Texture_hpp_INCLUDED_
#define engine_Texture_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "Drawable.hpp"
#include "GL.hpp"
#include "miniblocxx/Exception.hpp"

extern "C"
{
#include "libpng/png.h"
}

namespace engine
{

BLOCXX_DECLARE_EXCEPTION(Texture);

class Texture : public Drawable
{
public:
	Texture() : glTextureId(gl::INVALID_TEXTURE) {}
	Texture(GLuint glTextureId) : glTextureId(glTextureId) {}
	~Texture();

	void loadFromResource(const char* name);
	void loadFromPngData(png_byte* imageData, int width, int height);
	void loadFromPkmData(ResourcePtr pkmData);

	template <typename StrT>
	void loadFromResource(const StrT& name)
	{
		return loadFromResource(name.c_str());
	}

	bool loaded() { return glTextureId != gl::INVALID_TEXTURE; }

	void unload();

	virtual void draw(const Rectangle& screen);
private:
	GLuint glTextureId;


};

}

#endif
