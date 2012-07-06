/*
 * $Id: BaseClass.hpp,v 1.1.2.1 2009/02/22 10:09:56 kpharris Exp $
 *
 * Part of GraphLib -- A framework for graphics production.
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

#if       !defined(GRAPHLIB_BASECLASS_HPP)
#define            GRAPHLIB_BASECLASS_HPP

/*
 * @author Kevin Harris
 */

#include "StringDumpable.hpp"
#include "Cloneable.hpp"

namespace graphlib
{
	/**
	 * The root (base class) of most graphlib structures.  This contains no
	 * useful interface or data in itself, but forces multiple pure virtual
	 * classes as ancestors.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1.2.1 $
	 *
	 */
	class BaseClass : public virtual StringDumpable, public virtual Cloneable
	{
	public:
		BaseClass();
		virtual ~BaseClass();
		BaseClass(const BaseClass& old);
		BaseClass& operator= (const BaseClass& old);
	}; // class BaseClass

	typedef blocxx::IntrusiveReference<BaseClass> BaseClassRef;

} // namespace graphlib

#endif /* !defined(GRAPHLIB_BASECLASS_HPP) */

