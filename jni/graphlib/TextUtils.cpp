/*
 * Part of GraphLib
 * Copyright (C) 2009 Kevin Harris
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
#include "TextUtils.hpp"

namespace graphlib
{
	void skipSpaces(const char*& text)
	{
		if( text )
		{
			while( isspace(*text) )
			{
				++text;
			}
		}
	}

	void skipSpaces(const char*& text, unsigned& len)
	{
		if( text )
		{
			while( (len > 0) && isspace(*text) )
			{
				++text;
				--len;
			}
		}
	}

	unsigned lengthToNewline(const char* str)
	{
		unsigned length = 0;

		if( str )
		{
			for(; str[length] && (str[length] != '\n'); ++length )
			{
			}
		}

		return length;
	}

	unsigned lengthToNewline(const char* str, unsigned len)
	{
		unsigned length = 0;

		if( str )
		{
			for(; (length < len) && str[length] && (str[length] != '\n'); ++length )
			{
			}
		}

		return length;
	}

	unsigned long dehexify(const char* str, unsigned length)
	{
		unsigned long retval = 0;

		if( str )
		{
			while(*str && length > 0)
			{
				if( (*str >= '0') && (*str <= '9') )
				{
					retval = (retval << 4) | (0 + unsigned(*str - '0'));
				}
				else if( (*str >= 'a') && (*str <= 'f') )
				{
					retval = (retval << 4) | (10 + unsigned(*str - 'a'));
				}
				else if( (*str >= 'A') && (*str <= 'F') )
				{
					retval = (retval << 4) | (10 + unsigned(*str - 'A'));
				}
				else
				{
					break;
				}
				++str;
				--length;
			}
		}
		return retval;
	}
}
