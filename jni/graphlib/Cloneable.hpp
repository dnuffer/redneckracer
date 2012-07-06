/*
 * $Id: Cloneable.hpp,v 1.1.2.1 2009/02/22 10:09:56 kpharris Exp $
 *
 * Part of GraphLib
 *
 * Copyright (C) 2008 Kevin Harris
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

#if       !defined(GRAPHLIB_CLONEABLE_HPP)
#define            GRAPHLIB_CLONEABLE_HPP

/**
 * @author Kevin Harris
 */

#include "miniblocxx/IntrusiveReference.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
namespace graphlib
{
	/**
	 *
	 * The base class for cloneable objects.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1.2.1 $
	 *
	 */
	class Cloneable : public blocxx::IntrusiveCountableBase
	{
	public:
		Cloneable();

		virtual ~Cloneable();

		virtual blocxx::IntrusiveReference<Cloneable> clone() const = 0;

	}; // class Cloneable
	typedef blocxx::IntrusiveReference<Cloneable> CloneableRef;


} // namespace graphlib


#endif /* !defined(GRAPHLIB_CLONEABLE_HPP) */

