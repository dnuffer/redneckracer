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

#if !defined(BOUNDABLE_HPP_INCLUDED)
#define BOUNDABLE_HPP_INCLUDED

#include "EngineConfig.hpp"
#include "Rectangle.hpp"
#include "Size.hpp"
#include "Named.hpp"

namespace engine
{
	class Boundable : public virtual Named
	{
	public:
		Boundable();
		Boundable(const Size& size);
		Boundable(const Rectangle& boundingRect);
		virtual ~Boundable();

		const Rectangle& getBoundingRect() const { return m_boundingRect; }

		// Override this as needed to update the bounding rectangle.
		virtual void updateBoundingRect() { };

		// Sets the local bounding rectangle and fires off boundingRectChanged()
		// if a change actually occurred.
		void setBoundingRect(const Rectangle& rect);

	protected:
		virtual void boundingRectChanged() { }

	private:
		Rectangle m_boundingRect;
	};
}

#endif /* !defined(BOUNDABLE_HPP_INCLUDED) */
