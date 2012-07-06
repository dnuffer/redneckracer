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

#ifndef TextureLoader_HPP_INCLUDED_
#define TextureLoader_HPP_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "GL.hpp"
#include "miniblocxx/Exception.hpp"

extern "C"
{
#include "libpng/png.h"
}

namespace engine
{

namespace TextureLoader
{

BLOCXX_DECLARE_EXCEPTION(TextureLoader);

TexturePtr loadTexture(const ResourcePtr& data);

// Filename will be looked up in the apk (should start with assets/ or res/
// returns 0 on error and the gl texture id on success.
GLuint loadTextureFromPNG(const ResourcePtr& pngData, int &width, int &height);
GLuint loadTextureFromPKM(const ResourcePtr& pkmData, int &width, int &height);

png_byte* loadImageFromPNG(const ResourcePtr& pngData, int &width, int &height);
GLuint loadTextureFromPNGData(png_byte* image_data, int &width, int &height);

}

}

#endif 
