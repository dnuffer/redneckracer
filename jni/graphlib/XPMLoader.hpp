/*
 * $Id: XPMLoader.hpp,v 1.1.2.2 2009/02/23 04:56:13 kpharris Exp $
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

#if       !defined(GRAPHLIB_XPM_LOADER_HPP)
#define            GRAPHLIB_XPM_LOADER_HPP

/**
 * @author Kevin Harris
 */

#include "MaskedImage.hpp"

namespace graphlib
{
	bool loadXPM(const char* data, MaskedImage<unsigned char>& image);

	// The c-style xpm data array
	bool loadXPM(const char** data, MaskedImage<unsigned char>& image);
} // namespace graphlib


#endif /* !defined(GRAPHLIB_XPM_LOADER_HPP) */
