/*
 * $Id: PPMLoader.hpp,v 1.1.2.2 2009/02/23 04:56:13 kpharris Exp $
 *
 * Part of GraphLib -- a framework for graphics production.
 * Copyright (C) 2010 Kevin Harris
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if       !defined(GRAPHLIB_PPM_LOADER_HPP)
#define            GRAPHLIB_PPM_LOADER_HPP

/**
 * @author Kevin Harris
 */

#include "Image.hpp"

namespace graphlib
{
	// Load a PPM image.  P3 (text only) and P6 (binary) are supported.
	bool loadPPM(const char* data, unsigned len, Image<unsigned char>& image);

	// Save a text-based ppm
	bool savePPM_P3(const Image<unsigned char>& image, const char* filename);

	// Save a binary (P6) ppm.  This saves a lot of space over the P3,
	// but cannot be easily incorporated into source code.
	bool savePPM(const Image<unsigned char>& image, const char* filename);
} // namespace graphlib

#endif /* !defined(GRAPHLIB_PPM_LOADER_HPP) */
