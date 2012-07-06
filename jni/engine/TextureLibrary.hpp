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

#ifndef engine_TextureLibrary_hpp_INCLUDED_
#define engine_TextureLibrary_hpp_INCLUDED_

#include "EngineConfig.hpp"
#include "EngineFwd.hpp"
#include "Animation.hpp"
#include "GL.hpp"
#include "Size.hpp"
#include <string>
#include <tr1/unordered_map>
#include <tr1/functional>
#include "boost/range/adaptor/transformed.hpp"
#include "boost/range/algorithm_ext/for_each.hpp"
#include "boost/lambda/bind.hpp"
#include "miniblocxx/Exception.hpp"
#include "boost/noncopyable.hpp"
#include "Texture.hpp"
#include "TexturedQuad.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "Rectangle.hpp"

extern "C"
{
#include "libpng/png.h"
}

namespace engine
{

BLOCXX_DECLARE_EXCEPTION(TextureLibrary);

/**
 * The TextureLibrary consists of a number of atlases. The atlases are only used to load the library.
 * Retrieving textures will be based on texture names.
 * Loading and unloading groups is based on the group name.
 *
 * Each atlas has:
 *   name (the atlas description filename is based on this, by appending .atlas)
 *   group name (can be used to load/unload levels or areas if all textures won't fit in memory)
 *   OpenGL texture name
 *   quads
 *
 * Each quad has:
 *   name (this needs to be unique as that is how each is retrieved)
 *   rectangle identifying the texture in the atlas
 *   scale factor (this is typically used for compressed textures to minimize artifacts)
 *
 * Loading an atlas loads the <name>.atlas file. Then the image name is read from it and the image is loaded
 * into a GL texture. The quad data is loaded from a file named <name>.atlas.
 *
 * Format of an .atlas file
 * Each line has a prefix and a colon identifying the type.
 * The quad line is a space delimited sequence:
 *   quad:<name> <left> <bottom> <width> <height> <x scale factor> <y scale factor>
 * The group line is:
 *   group:<group name>
 * The image line is:
 *   image:<image filename>
 * The size line is:
 *   size:<atlas image width> <atlas image height>
 */


class TextureLibrary : boost::noncopyable, public virtual IntrusiveCountableBase
{
public:
	~TextureLibrary();

	void loadAllAtlases(const std::tr1::function<void (float)>& progressCallback = NULL);

	StringArray listAtlases(String prefixFilter = String()) const;
	StringArray listQuads(String prefixFilter = String()) const;
	void loadAtlasData(const String& name);
	void loadGroup(const std::string& group, const std::tr1::function<void (float)>& progressCallback = NULL);
	void unloadGroup(const std::string& group);

	void loadAtlasTexture(const std::string& name);
	void unloadAtlasTexture(const std::string& name);

	template <typename KeysT>
	AnimationPtr animationFromAtlasKeys(const KeysT keys, Animation::ELoopsOption loops = Animation::E_PLAY_ONCE, float speed = 15.0) const;

	// Don't call this in the game loop. This function is slow, it compares against all the quad names.
	template <typename OutputContainerT>
	void animationFrames(const std::string& prefixFilter, OutputContainerT& output) const;
	
	// Don't call this in the game loop. This function is slow, it compares against all the quad names.
	AnimationPtr animation(const std::string& quadNamesPrefix, Animation::ELoopsOption loops = Animation::E_PLAY_ONCE, float speed = 15.0) const;

	TexturedQuadPtr texturedQuad(const std::string& name) const;

	template <typename KeysT, typename OutputContainerT>
	void texturedQuads(const KeysT keys, OutputContainerT& output) const;

	void loadRealBounds(const String& fileName);

	// Load all PNG images and PKM data (from all loaded atlase) in memory.
	void preloadTexImages(const std::tr1::function<void (float)>& progressCallback);


private:


	struct Quad
	{
		Quad(const String& name, UInt16 left, UInt16 bottom, UInt16 width, UInt16 height, float widthScaleFactor,
			 float heightScaleFactor, const Size& atlasSize, const TexturePtr& atlasTexture)
		: name(name), left(left), bottom(bottom), width(width), height(height), widthScaleFactor(widthScaleFactor),
		heightScaleFactor(heightScaleFactor), atlasSize(atlasSize), atlasTexture(atlasTexture)
		{}
		// quad:<name> <left> <bottom> <width> <height> <x scale factor> <y scale factor>
		String name;
		UInt16 left;
		UInt16 bottom;
		UInt16 width;
		UInt16 height;
		float widthScaleFactor;
		float heightScaleFactor;
		Size atlasSize;
		TexturePtr atlasTexture;
	};

	struct PngImageData
	{
		PngImageData(): data(NULL), width(0), height(0) {}
		PngImageData(png_byte* data, int w, int h): data(data), width(w), height(h){}
		png_byte* data;
		int width;
		int height;
	};

	typedef tr1::unordered_map<string, Quad> quadLibrary_t;
	typedef tr1::unordered_map<string, Rectangle> realBoundLibrary_t;
	typedef tr1::unordered_map<string, PngImageData> texImagesLibrary_t;
	typedef tr1::unordered_map<string, ResourcePtr> texPKMLibrary_t;

	// key is the quad name, value is the associated Quad
	quadLibrary_t quadLibrary;

	// key is the quad name, value is the associated real bounding rectangle
	realBoundLibrary_t realBoundLibrary;

	texImagesLibrary_t texImagesLibrary; // used for store preloaded PNG images data.

	texPKMLibrary_t texPKMLibrary; // used for store preloaded PKM images data.

	// key is the group name. value is the quad names in that group
	typedef tr1::unordered_map<string, std::vector<string> > groupMap_t;
	groupMap_t groups;

	struct Atlas
	{
		Atlas() : texture(new Texture) {}

		String imageFilename;
		String group;
		Size size;
		vector<Quad> quads;
		TexturePtr texture;
	};
	// key is atlas name. value is data loaded from the atlas file
	typedef tr1::unordered_map<string, Atlas> atlasMap_t;
	atlasMap_t atlases;
};

template <typename KeysT>
AnimationPtr TextureLibrary::animationFromAtlasKeys(const KeysT keys, Animation::ELoopsOption loops, float speed) const
{
	using namespace boost::lambda;
	using namespace boost::adaptors;
	return new Animation(keys | transformed(ret<TexturedQuadPtr>(boost::lambda::bind(&TextureLibrary::texturedQuad, this, boost::lambda::_1))), loops, speed);
}

	template <typename KeysT, typename OutputContainerT>
	void TextureLibrary::texturedQuads(const KeysT keys, OutputContainerT& output) const
	{
		using namespace boost::lambda;
		using namespace boost::adaptors;
		push_back(output, keys | transformed(ret<TexturedQuadPtr>(boost::lambda::bind(&TextureLibrary::texturedQuad, this, boost::lambda::_1))));
	}

	template <typename OutputContainerT>
	void TextureLibrary::animationFrames(const std::string& prefixFilter, OutputContainerT& output) const
	{
		texturedQuads(listQuads(prefixFilter), output);
	}

}

#endif
