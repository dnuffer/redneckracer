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
#include "EngineFwd.hpp"
#include "TextureLoader.hpp"
#include "GL.hpp"
extern "C"
{
#include "libpng/png.h"
}
#include "Log.hpp"
#include "miniblocxx/ScopeGuard.hpp"
#include "miniblocxx/stl/vector"
#include "Resource.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "Texture.hpp"

namespace engine
{

namespace TextureLoader
{

BLOCXX_DEFINE_EXCEPTION(TextureLoader);

using namespace gl;
using namespace boost::algorithm;


TexturePtr loadTexture(const ResourcePtr& data)
{
	if (iends_with(data->name(), ".png"))
	{
		int width, height;
		GLint tex = loadTextureFromPNG(data, width, height);
		return new Texture(tex);
	}
	else if (iends_with(data->name(), ".pkm"))
	{
		int width, height;
		GLint tex = loadTextureFromPKM(data, width, height);
		return new Texture(tex);
	}
	BLOCXX_THROW(TextureLoaderException,
			"unable to determine type of resource from filename extension. Only .png and .pkm are valid");

	return NULL;
}

const GLuint TEXTURE_LOAD_ERROR = INVALID_TEXTURE;

//Taken from http://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
/** loadTexture
 *   loads a png file into an opengl texture object, using cstdio , libpng, and opengl.
 *
 *   \param filename : the png file to be loaded
 *   \param width : width of png, to be updated as a side effect of this function
 *   \param height : height of png, to be updated as a side effect of this function
 *
 *   \return GLuint : an opengl texture id.  Will be 0 if there is a major error,
 *           should be validated by the client of this function.
 *
 */

namespace
{
png_bytep sourceData;
extern "C" void TextureLoader_png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
//	LOGD("TextureLoader_png_read_data(data: %p, length: %d", data, length);
	memcpy(data, sourceData, length);
	sourceData += length;
}
}


png_byte* loadImageFromPNG(const ResourcePtr& pngData, int &width, int &height)
{
	//  g_file = zip_fopen(archive, filename, 0);
	//  if (!g_file) {
	//    LOGE("Error opening %s from APK", filename);
	//    return TEXTURE_LOAD_ERROR;
	//  }

	//  BLOCXX_ON_BLOCK_EXIT(zip_fclose, g_file);

	const unsigned PNG_HEADER_SIZE = 8;
	if (pngData->data().size() <= PNG_HEADER_SIZE)
	{
		LOGE("pngData is not the right size. expected > %u, is: %zu", PNG_HEADER_SIZE, pngData->data().size());
		return TEXTURE_LOAD_ERROR;
	}

	//header for testing if it is a png
	png_byte* header(&pngData->data()[0]);

	//read the header
	//  zip_fread(g_file, header, 8);

	//test if png
	int is_png = !png_sig_cmp(header, 0, PNG_HEADER_SIZE);
	if (!is_png)
	{
		LOGE("Not a png file");
		return TEXTURE_LOAD_ERROR;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		LOGE("Unable to create png struct");
		return (TEXTURE_LOAD_ERROR);
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		LOGE("Unable to create png info");
		return (TEXTURE_LOAD_ERROR);
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		LOGE("Unable to create png end info");
		return (TEXTURE_LOAD_ERROR);
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		LOGE("Error during setjmp");
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return (TEXTURE_LOAD_ERROR);
	}

	//init png reading
	//png_init_io(png_ptr, fp);
	png_set_read_fn(png_ptr, NULL, TextureLoader_png_read_data);
	// global used by png_zip_read to keep track of data read by the lib
	sourceData = &pngData->data()[PNG_HEADER_SIZE];

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type, NULL, NULL, NULL);

	//update width and height based on png info
	width = twidth;
	LOGD("loadTextureFromPNG width: %d", width);
	height = theight;
	LOGD("loadTextureFromPNG height: %d", height);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	LOGD("loadTextureFromPNG rowbytes: %d", rowbytes);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];

	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[i] = image_data + i * rowbytes;

		// The following loads the image upside down, which may be useful since gl draws textures vertically
		// inverted.
		//row_pointers[height - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	return image_data;
}


GLuint loadTextureFromPNGData(png_byte* image_data, int &width, int &height)
{
	if (image_data == NULL) LOGE("loadTextureFromPNGData:Image data == NULL");
	GLuint texture = genTexture();
	bindTexture2D(texture);
	texImage2D(width, height, static_cast<GLvoid*>(image_data));
	texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return texture;
}

GLuint loadTextureFromPNG(const ResourcePtr& pngData, int &width, int &height)
{
	//Now generate the OpenGL texture object
	GLuint texture = genTexture();
	bindTexture2D(texture);
	png_byte* image_data = loadImageFromPNG(pngData, width, height);
	// got this logic from android_source/frameworks/base/cmds/bootanimation/BootAnimation.cpp,
	// but it doesn't seem to work right. The odd-sized texture loads, but it is squished. I could probably fix
	// that, but I'm not going to worry about it for now.
//    int tw = 1 << (31 - __builtin_clz(width));
//    int th = 1 << (31 - __builtin_clz(height));
//    if (tw < width) tw <<= 1;
//    if (th < height) th <<= 1;
//
//    if (tw != width || th != height)
//    {
//        GLint crop[4] = { 0, height, width, -height };
//    	texImage2D(tw, th, 0);
//    	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid*>(image_data));
//    	checkGLError("glTexSubImage2D");
//        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
//        checkGLError("glTexParameteriv");
//    }
//    else
//    {
    	texImage2D(width, height, static_cast<GLvoid*>(image_data));
//    }

	texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// I thought these might help with random black pixels on the edge of a rotated texture, but they didn't.
//	texParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	texParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//clean up memory and close stuff
	//png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	//delete[] image_data;
	//delete[] row_pointers;
	delete[] image_data;
	return texture;
}

typedef unsigned char etc1_byte;
typedef int etc1_bool;
typedef unsigned int etc1_uint32;

static const char kMagic[] =
{ 'P', 'K', 'M', ' ', '1', '0' };

static const etc1_uint32 ETC1_PKM_FORMAT_OFFSET = 6;
static const etc1_uint32 ETC1_PKM_ENCODED_WIDTH_OFFSET = 8;
static const etc1_uint32 ETC1_PKM_ENCODED_HEIGHT_OFFSET = 10;
static const etc1_uint32 ETC1_PKM_WIDTH_OFFSET = 12;
static const etc1_uint32 ETC1_PKM_HEIGHT_OFFSET = 14;

static const etc1_uint32 ETC1_RGB_NO_MIPMAPS = 0;

static etc1_uint32 readBEUint16(const etc1_byte* pIn)
{
	return (pIn[0] << 8) | pIn[1];
}

// Check if a PKM header is correctly formatted.

etc1_bool etc1_pkm_is_valid(const etc1_byte* pHeader)
{
	if (memcmp(pHeader, kMagic, sizeof(kMagic)))
	{
		return false;
	}
	etc1_uint32 format = readBEUint16(pHeader + ETC1_PKM_FORMAT_OFFSET);
	etc1_uint32 encodedWidth = readBEUint16(pHeader + ETC1_PKM_ENCODED_WIDTH_OFFSET);
	etc1_uint32 encodedHeight = readBEUint16(pHeader + ETC1_PKM_ENCODED_HEIGHT_OFFSET);
	etc1_uint32 width = readBEUint16(pHeader + ETC1_PKM_WIDTH_OFFSET);
	etc1_uint32 height = readBEUint16(pHeader + ETC1_PKM_HEIGHT_OFFSET);
	return format == ETC1_RGB_NO_MIPMAPS && encodedWidth >= width && encodedWidth - width < 4 && encodedHeight >= height && encodedHeight - height < 4;
}

// Read the image width from a PKM header

etc1_uint32 etc1_pkm_get_width(const etc1_byte* pHeader)
{
	return readBEUint16(pHeader + ETC1_PKM_WIDTH_OFFSET);
}

// Read the image height from a PKM header

etc1_uint32 etc1_pkm_get_height(const etc1_byte* pHeader)
{
	return readBEUint16(pHeader + ETC1_PKM_HEIGHT_OFFSET);
}

// Return the size of the encoded image data (does not include size of PKM header).

etc1_uint32 etc1_get_encoded_data_size(etc1_uint32 width, etc1_uint32 height)
{
	return (((width + 3) & ~3) * ((height + 3) & ~3)) >> 1;
}

GLuint loadTextureFromPKM(const ResourcePtr& pkmData, int &width, int &height)
{
#ifdef GL_ETC1_RGB8_OES
	const unsigned ETC_PKM_HEADER_SIZE = 16;
	if (pkmData->data().size() <= ETC_PKM_HEADER_SIZE)
	{
		LOGE("pkmData is not the right size. expected > %u, is: %zu", ETC_PKM_HEADER_SIZE, pkmData->data().size());
		return TEXTURE_LOAD_ERROR;
	}

	etc1_byte* header = &pkmData->data()[0];

	if (!etc1_pkm_is_valid(header))
		return TEXTURE_LOAD_ERROR;

	width = etc1_pkm_get_width(header);
	height = etc1_pkm_get_height(header);

	size_t encodedDataSize = etc1_get_encoded_data_size(width, height);

	if (pkmData->data().size() != ETC_PKM_HEADER_SIZE + encodedDataSize)
	{
		LOGE("pkmData is not the right size. expected: %zu, is: %zu", ETC_PKM_HEADER_SIZE + encodedDataSize, pkmData->data().size());
		return TEXTURE_LOAD_ERROR;
	}

	//Now generate the OpenGL texture object
	GLuint texture = genTexture();
	bindTexture2D(texture);
	compressedTexImage2D(GL_ETC1_RGB8_OES, width, height, encodedDataSize, static_cast<GLvoid*> (&pkmData->data()[ETC_PKM_HEADER_SIZE]));
	texParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return texture;
#else
	LOGE("PKM texture format not supported by this platform");
	return TEXTURE_LOAD_ERROR;
#endif
}

}

}
