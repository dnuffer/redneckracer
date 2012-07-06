/*
 * $Id: TextUtils.hpp,v 1.1.2.2 2009/02/23 04:56:13 kpharris Exp $
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

#if       !defined(GRAPHLIB_TEXT_UTILS_HPP)
#define            GRAPHLIB_TEXT_UTILS_HPP

/**
 * @author Kevin Harris
 */

#include <ctype.h>

namespace graphlib
{
	// Advance the pointer to the next spot beyond whitespace.
	void skipSpaces(const char*& text);
	void skipSpaces(const char*& text, unsigned& len);
	unsigned lengthToNewline(const char* str);
	unsigned lengthToNewline(const char* str, unsigned len);
	unsigned long dehexify(const char* str, unsigned length);

	template<typename T>
	bool readInteger(const char*& text, T& value)
	{
		if( text )
		{
			skipSpaces(text);
			value = T(0);
			const char* current = text;

			while( *current && isdigit(*current) )
			{
				value = value * 10 + (*current - '0');
				++current;
			}
			// If we hit the end of the text or hit some whitespace, we ate it completely.
			if( (current != text) && (!*current || isspace(*current)) )
			{
				text = current;
				return true;
			}
		}
		return false;
	}

	template<typename T>
	bool readInteger(const char*& text, unsigned& length, T& value)
	{
		if( text && length > 0 )
		{
			skipSpaces(text);
			value = T(0);
			const char* current = text;
			unsigned len = length;

			while( (len > 0) && *current && isdigit(*current) )
			{
				value = value * 10 + (*current - '0');
				++current;
				--len;
			}
			// If we hit the end of the text or hit some whitespace, we ate it completely.
			if( (current != text) && (!*current || isspace(*current)) )
			{
				length = len;
				text = current;
				return true;
			}
		}
		return false;
	}
} // namespace graphlib

#endif /* !defined(GRAPHLIB_TEXT_UTILS_HPP) */
