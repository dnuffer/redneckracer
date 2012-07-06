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
#include "TextureLibrary.hpp"
#include "Resources.hpp"
#include "Resource.hpp"
#include "Log.hpp"
#include "TextureLoader.hpp"
#include "miniblocxx/String.hpp"
#include "boost/range/algorithm_ext/push_back.hpp"
#include "boost/next_prior.hpp"


using namespace boost;
using namespace boost::lambda;
using namespace boost::adaptors;
using namespace boost::range;


namespace engine
{
BLOCXX_DEFINE_EXCEPTION(TextureLibrary);

	TextureLibrary::~TextureLibrary() {}

	StringArray TextureLibrary::listAtlases(String prefixFilter) const
{
	StringArray files = Resources::listResources();
	StringArray results;
	for( size_t i = 0; i < files.size(); ++i )
	{
		if( files[i].endsWith(".atlas") && files[i].startsWith(prefixFilter) )
		{
			results.append(files[i]);
		}
	}
	return results;
}

	StringArray TextureLibrary::listQuads(String prefixFilter) const
{
	StringArray results;

	for( quadLibrary_t::const_iterator quad = quadLibrary.begin(); quad != quadLibrary.end(); ++quad )
	{
		const std::string& name = quad->first;

		if( prefixFilter.empty() || (name.find(prefixFilter.c_str()) == 0) )
		{
			results.append(name);
		}
	}
	
	sort(results.begin(), results.end());
		 
	return results;
}

	void TextureLibrary::loadAllAtlases(const std::tr1::function<void (float)>& progressCallback)
{
	StringArray atlases = listAtlases();
	for( size_t i = 0; i < atlases.size(); ++i )
	{
		if (progressCallback != NULL)
		{
			progressCallback(1.0f);
		}
		loadAtlasData(atlases[i]);
	}
}

	void TextureLibrary::loadAtlasData(const String& name)
{
	// load the atlas description
	String atlasFilename = name;
	if( !name.endsWith(".atlas") )
	{
		atlasFilename += ".atlas";
	}
	ResourcePtr atlasResource = Resources::loadResourceFromAssets(atlasFilename.c_str());
	String atlasStr = String(
		reinterpret_cast<const char*>(&*atlasResource->begin()),
		distance(atlasResource->begin(), atlasResource->end()));

	Atlas atlas;

	StringArray lines = atlasStr.tokenize("\r\n");
	for (StringArray::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		const String& line(*it);
		if (line.startsWith("image:"))
		{
			atlas.imageFilename = line.tokenize(": \t").at(1);
			LOGD("parsed image filename %s", atlas.imageFilename.c_str());
		}
		else if (line.startsWith("group:"))
		{
			atlas.group = line.tokenize(": \t").at(1);
			LOGD("parsed group %s", atlas.group.c_str());
		}
		else if (line.startsWith("quad:"))
		{
			StringArray toks = line.tokenize(": \t");
			atlas.quads.push_back(Quad(
					toks.at(1),
					toks.at(2).toUInt16(),
					toks.at(3).toUInt16(),
					toks.at(4).toUInt16(),
					toks.at(5).toUInt16(),
					toks.at(6).toFloat(),
					toks.at(7).toFloat(),
					atlas.size,
					atlas.texture));
			LOGD("parsed quad line: %s", line.c_str());
		}
		else if (line.startsWith("size:"))
		{
			StringArray toks = line.tokenize(": \t");
			atlas.size.width() = toks.at(1).toUInt16();
			atlas.size.height() = toks.at(2).toUInt16();
			LOGD("parsed size line: %dx%d", (int)atlas.size.width(), (int)atlas.size.height());
		}
	}

	groups[atlas.group].push_back(name);

	for (vector<Quad>::const_iterator it = atlas.quads.begin(); it != atlas.quads.end(); ++it)
		quadLibrary.insert(make_pair((*it).name, *it));

	atlases[name] = atlas;
}

	void TextureLibrary::loadGroup(const std::string& group, const std::tr1::function<void (float)>& progressCallback)
{
	LOGD("TextureLibrary::loadGroup(group: %s,...)", group.c_str());

	groupMap_t::const_iterator git = groups.find(group);
	if (git == groups.end())
		BLOCXX_THROW(TextureLibraryException, Format("invalid group: %1", group).c_str());

	const vector<string>& atlasNames = git->second;

	for (vector<string>::const_iterator it = atlasNames.begin(); it != atlasNames.end(); ++it)
	{
		loadAtlasTexture(*it);
		if (progressCallback != NULL) progressCallback((float)distance(atlasNames.begin(), next(it)) / (float)distance(atlasNames.begin(), atlasNames.end()));
	}
}

	void TextureLibrary::loadAtlasTexture(const std::string& name)
{
	LOGD("loadAtlasTexture loading atlas %s", name.c_str());
	atlasMap_t::const_iterator loc = atlases.find(name);
	if( loc == atlases.end() )
	{
		BLOCXX_THROW(TextureLibraryException, Format("Failed to find atlas %1", name).c_str());
	}
	const Atlas& atlas = loc->second;
	bool isTextureLoaded = false;
	if (!atlas.texture->loaded())
	{
		// Search for preloaded data
		if (atlas.imageFilename.endsWith(".png"))
		{
			texImagesLibrary_t::iterator im = texImagesLibrary.find(atlas.imageFilename);
			if (im != texImagesLibrary.end())
			{
				LOGI("Loading texture from preloaded PNG image.");
				// Load texture from preloaded png data
				atlas.texture->loadFromPngData(im->second.data, im->second.width, im->second.height);
				delete[] im->second.data;
				texImagesLibrary.erase(im);
				isTextureLoaded = true;
			}
		}
		else
		{
			texPKMLibrary_t::iterator im = texPKMLibrary.find(atlas.imageFilename);
			if (im != texPKMLibrary.end())
			{
				LOGI("Loading texture from preloaded PKM image.");
				atlas.texture->loadFromPkmData(im->second);
				//delete im->second.data();
				texPKMLibrary.erase(im);
				isTextureLoaded = true;
			}
		}

		if (!isTextureLoaded)
		{
			LOGI("Loading texture from file.");
			atlas.texture->loadFromResource(atlas.imageFilename.c_str());
		}
	}
	else
	{
		LOGD("Already loaded. Doing nothing");
	}
}

	void TextureLibrary::unloadAtlasTexture(const std::string& name)
{
	LOGD("unloadAtlasTexture unloading atlas %s", name.c_str());
	atlasMap_t::const_iterator loc = atlases.find(name);
	if( loc == atlases.end() )
	{
		BLOCXX_THROW(TextureLibraryException, Format("Failed to find atlas %1", name).c_str());
	}
	const Atlas& atlas = loc->second;
	atlas.texture->unload();
}

	void TextureLibrary::unloadGroup(const std::string& group)
{
	LOGD("TextureLibrary::unloadGroup(group: %s)", group.c_str());

	groupMap_t::const_iterator git = groups.find(group);
	if (git == groups.end())
		BLOCXX_THROW(TextureLibraryException, Format("invalid group: %1", group).c_str());

	const vector<string>& atlasNames = git->second;

	for (vector<string>::const_iterator it = atlasNames.begin(); it != atlasNames.end(); ++it)
	{
		LOGD("unloadGroup from atlas %s", (*it).c_str());
		Atlas& atlas(atlases[*it]);
		atlas.texture->unload();
	}
}
	
	AnimationPtr TextureLibrary::animation(const std::string& quadNamesPrefix, Animation::ELoopsOption loops, float speed) const
	{
		return animationFromAtlasKeys(listQuads(quadNamesPrefix), loops, speed);
	}

	TexturedQuadPtr TextureLibrary::texturedQuad(const string& name) const
	{
		//LOGD("texturedQuad name: %s", name.c_str());
		quadLibrary_t::const_iterator it = quadLibrary.find(name);
		if (it == quadLibrary.end())
			BLOCXX_THROW(TextureLibraryException, name.c_str());

		const Quad& quad((*it).second);

		realBoundLibrary_t::const_iterator rb = realBoundLibrary.find(name);
		if (rb != realBoundLibrary.end())
		{
			LOGD("Loading real bounding rectangle: %s", (rb->first).c_str());
			return new TexturedQuad(float(quad.left) / quad.atlasSize.width(),
									float(quad.bottom) / quad.atlasSize.height(),
									float(quad.left + quad.width) / quad.atlasSize.width(),
									float(quad.bottom + quad.height) / quad.atlasSize.height(),
									quad.atlasTexture,
									quad.width * quad.widthScaleFactor,
									quad.height * quad.heightScaleFactor,
									rb->second);

		}
		else
		{
			return new TexturedQuad(float(quad.left) / quad.atlasSize.width(),
									float(quad.bottom) / quad.atlasSize.height(),
									float(quad.left + quad.width) / quad.atlasSize.width(),
									float(quad.bottom + quad.height) / quad.atlasSize.height(),
									quad.atlasTexture,
									quad.width * quad.widthScaleFactor,
									quad.height * quad.heightScaleFactor);
		}
	}

	void TextureLibrary::loadRealBounds(const String& fileName)
	{
		ResourcePtr boundsResource = Resources::loadResourceFromAssets(fileName.c_str());
		String boundsStr = String(reinterpret_cast<const char*>(&*boundsResource->begin()), distance(boundsResource->begin(), boundsResource->end()));
		StringArray lines = boundsStr.tokenize("\r\n");
		Rectangle boundRect(0.0f, 0.0f, 0.0f, 0.0f);
		for (StringArray::const_iterator it = lines.begin(); it != lines.end(); ++it)
		{
			const String& line(*it);
			if (line.startsWith("image:"))
			{
				StringArray toks = line.tokenize(": \t");
				float scaleX = toks.at(6).toFloat();
				float scaleY = toks.at(7).toFloat();

				boundRect.left   = toks.at(2).toFloat() * scaleX;
				boundRect.top    = toks.at(3).toFloat() * scaleY;
				boundRect.right  = toks.at(4).toFloat() * scaleX;
				boundRect.bottom = toks.at(5).toFloat() * scaleY;

				realBoundLibrary.insert(make_pair(toks.at(1), boundRect));
				LOGD("Bounding rectangle changed to real at %s", toks.at(1).c_str());
				LOGD("Rectangle: %f %f %f %f",boundRect.left,boundRect.right, boundRect.top,boundRect.bottom );
			}
		}
	}

	void TextureLibrary::preloadTexImages(const std::tr1::function<void (float)>& progressCallback)
	{
		for (groupMap_t::const_iterator git = groups.begin(); git != groups.end(); ++git)
		{
			const vector<string>& atlasNames = git->second;
			for (vector<string>::const_iterator it = atlasNames.begin(); it != atlasNames.end(); ++it)
			{
				//LOGD("loading AtlasPNGImage loading atlas %s", it->c_str());
				atlasMap_t::const_iterator loc = atlases.find(*it);
				if( loc == atlases.end() )
				{
					BLOCXX_THROW(TextureLibraryException, Format("Failed to find atlas %1", *it).c_str());
				}
				const Atlas& atlas = loc->second;
				if (!atlas.texture->loaded())
				{
					int w, h;
					if (atlas.imageFilename.endsWith(".png"))
					{
						LOGD("Preloading png image %s", atlas.imageFilename.c_str() );
						ResourcePtr pngData  = Resources::loadResourceFromAssets(atlas.imageFilename.c_str());
						png_byte* image = TextureLoader::loadImageFromPNG(pngData, w, h);
						texImagesLibrary.insert(make_pair(atlas.imageFilename, PngImageData(image, w, h)));
					}
					else // PKM file.
					{
						LOGD("Preloading pkm image %s", atlas.imageFilename.c_str());
						texPKMLibrary.insert(make_pair(atlas.imageFilename, Resources::loadResourceFromAssets(atlas.imageFilename.c_str())));
					}

					progressCallback(0);
				}
			}
		}
	}

}
