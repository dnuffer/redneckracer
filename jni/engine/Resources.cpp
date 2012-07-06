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
#include "Resources.hpp"
#include "TextureLoader.hpp"
#if TARGET_OS_IPHONE != 1
#include "libzip/zip.h"
#endif
#if TARGET_OS_IPHONE == 1
#include "platform/CCFileUtils.h"
#endif
#include "Log.hpp"
#include <stdlib.h>
#include "Resource.hpp"
#include "miniblocxx/ScopeGuard.hpp"
#include "miniblocxx/Format.hpp"
#include <string>

namespace engine
{

namespace Resources
{
#if TARGET_OS_IPHONE == 1
	using namespace cocos2d;
#endif
	
BLOCXX_DEFINE_EXCEPTION(Resource);

namespace
{
#if TARGET_OS_IPHONE != 1
zip* archive;
#endif
}

void init(const char* rootPath)
{
#if TARGET_OS_IPHONE != 1
	LOGI("Loading APK %s", rootPath);
	archive = zip_open(rootPath, 0, NULL);
	if (archive == NULL) {
		LOGE("Error loading APK");
		abort();
	}

	//Just for debug, print APK contents.
	StringArray files = listResources();
	for( size_t i = 0; i < files.size(); ++i )
	{
		LOGI("File %u : %s", (unsigned)i, files[i].c_str());
	}
#else
	// do nothing
#endif
}

#if TARGET_OS_IPHONE != 1
StringArray listResources()
{
	StringArray results;
	int numFiles = zip_get_num_files(archive);

	String assets = "assets/";
	for (int i=0; i<numFiles; i++)
	{
		const char* name = zip_get_name(archive, i, 0);
		if (name == NULL)
		{
			BLOCXX_THROW(ResourceException, Format("Error reading zip file name at index %1 : %2", i, zip_strerror(archive)).c_str());
		}
		String strname(name);
		if( strname.startsWith(assets) )
		{
			results.push_back(strname.substring(assets.length()));
		}
	}
	return results;
}
#endif

ResourcePtr loadResource(const char* filename)
{
#if TARGET_OS_IPHONE != 1
	zip_file* file = zip_fopen(archive, filename, 0);
	if (!file)
	{
		LOGE("Error opening %s from APK: %s", filename, zip_strerror(archive));
		return NULL;
	}
	BLOCXX_ON_BLOCK_EXIT(zip_fclose, file);
	struct zip_stat fileStat;
	zip_stat_init(&fileStat);
	if (zip_stat(archive, filename, 0, &fileStat) == -1)
	{
		LOGE("Error getting stat on %s from APK: %s", filename, zip_strerror(archive));
		return NULL;
	}

	Array<UInt8> data(fileStat.size);
	if (zip_fread(file, &(data[0]), fileStat.size) != fileStat.size)
		return NULL;

	return new Resource(data, filename);
#else
	unsigned long size = 0;
	unsigned char* data = CCFileUtils::getFileData(filename, "rb", &size);
	// TODO: fill out the array directly instead of copying the data
	Array<UInt8> array(data, data + size);
	delete[] data;
	return new Resource(array, filename);
#endif
}


ResourcePtr loadResourceFromAssets(const char* filename)
{
	std::string name(filename);
	if( name.find("assets/") != 0 )
	{
		return loadResource(("assets/" + name).c_str());
	}
	return loadResource(filename);
}

// Filename will be looked up in the apk (should start with assets/ or res/
// returns 0 on error and the gl texture id on success.
GLuint loadTextureFromPNG(const char* filename, int& width, int& height)
{
	return TextureLoader::loadTextureFromPNG(loadResourceFromAssets(filename), width, height);
}

GLuint loadTextureFromPKM(const char* filename, int& width, int& height)
{
	return TextureLoader::loadTextureFromPKM(loadResourceFromAssets(filename), width, height);
}

}

}
