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

#ifndef engine_Resources_hpp_INCLUDED_
#define engine_Resources_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "GL.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/String.hpp"

namespace engine
{

namespace Resources
{
BLOCXX_DECLARE_EXCEPTION(Resource);

void init(const char* apkPath);

// Get the file names of the currently loaded APK as set in init().  Throws a ResourceException on error.
StringArray listResources();

ResourcePtr loadResource(const char* filename);
// loads from assets/ on Android
ResourcePtr loadResourceFromAssets(const char* filename);


// Filename will be looked up in the apk (should start with assets/ or res/
// returns 0 on error and the gl texture id on success.
GLuint loadTextureFromPNG(const char* filename, int& width, int& height);
GLuint loadTextureFromPKM(const char* filename, int& width, int& height);

}

}

#endif
