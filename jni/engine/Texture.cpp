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

#include "EngineConfig.hpp"
#include "Texture.hpp"
#include "GL.hpp"
#include "Log.hpp"
#include "Resources.hpp"
#include "TextureLoader.hpp"

#include "boost/range/algorithm/copy.hpp"


namespace engine
{

BLOCXX_DEFINE_EXCEPTION(Texture);

Texture::~Texture()
{
	unload();
}

void Texture::loadFromResource(const char* name)
{
	LOGD("Texture::loadFromResource: %s", name);
	int h, w;
	glTextureId = String(name).endsWith(".png")
			? Resources::loadTextureFromPNG(name, w, h) : Resources::loadTextureFromPKM(name, w, h);
	if (glTextureId == gl::INVALID_TEXTURE)
	{
		LOGE("failed to load %s", name);
		abort();
	}
	LOGD("loaded %s", name);
}

void Texture::loadFromPngData(png_byte* imageData, int width, int height)
{
	glTextureId = TextureLoader::loadTextureFromPNGData(imageData, width, height);
	if (glTextureId == gl::INVALID_TEXTURE)
	{
		LOGE("failed to load texture from preloaded png data");
		abort();
	}
	LOGD("loaded texture from preloaded png data");
}

void Texture::loadFromPkmData(ResourcePtr pkmData)
{
	int h, w;
	glTextureId = TextureLoader::loadTextureFromPKM(pkmData, w, h);
	if (glTextureId == gl::INVALID_TEXTURE)
	{
		LOGE("failed to load texture from preloaded pkm data");
		abort();
	}
	LOGD("loaded texture from preloaded pkm data");
}

void Texture::draw(const Rectangle& screen)
{
	if (!loaded())
	{
		BLOCXX_THROW(TextureException, "Attempting to draw a Texture that isn't loaded");
		return;
	}
	//	LOGD("texture(%p)::Draw drawing texture %d", this, glTextureId);

	using namespace gl;
	enableTexture2D();
	bindTexture2D(glTextureId);
}

void Texture::unload()
{
	if( loaded() )
	{
		LOGD("unload(%p) called for texture id %d", this, glTextureId);
		gl::deleteTextures(1, &glTextureId);
		glTextureId = gl::INVALID_TEXTURE;
	}
}


}
