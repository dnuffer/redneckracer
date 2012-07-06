/*
 * $Id: AlphaImage.hpp,v 1.7 2008/06/21 22:25:10 kpharris Exp $
 *
 * Part of "Graphlib" a playground for graphics development
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

#if       !defined(GRAPHLIB_ALPHA_IMAGE_HPP)
#define            GRAPHLIB_ALPHA_IMAGE_HPP

#include "graphlib/Raster.hpp"
#include "graphlib/GenericRGBAColor.hpp"

namespace graphlib
{
	template <typename T, typename ColorTraits = typename GenericRGBAColor<T>::color_traits_type>
	class AlphaImage : public Raster<GenericRGBAColor<T,ColorTraits> >
	{
	public:
		typedef GenericRGBAColor<T,ColorTraits> color_type;
		typedef Raster<color_type> parent;

		AlphaImage();
		AlphaImage(const parent& r);
		AlphaImage(unsigned width, unsigned height);
		virtual ~AlphaImage();
		AlphaImage(const AlphaImage& old);
		AlphaImage& operator= (const AlphaImage& old);

		CloneableRef clone() const { return CloneableRef(new AlphaImage(*this)); }
		blocxx::String name() const { return "AlphaImage"; }
		void collectInternalMembers(MemberStringDumpCollector& collector) const;

		// Return if the pixel at x,y is visible.  For most uses, you really
		// don't want to do this for each pixel.  Know what image type you have
		// before you try to draw it.
		virtual bool visible(int x, int y) const;

		AlphaImage subImage(const GenericRectangle<unsigned>& rect) const
		{
			return AlphaImage(parent::subRaster(rect));
		}
	}; // class AlphaImage

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>::AlphaImage()
		: parent()
	{

	} // AlphaImage()

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>::AlphaImage(const parent& r)
		: parent(r)
	{
	}

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>::AlphaImage(unsigned width, unsigned height)
		: parent(width, height)
	{

	} // AlphaImage(w,h)

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>::~AlphaImage()
	{

	} // ~AlphaImage()

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>::AlphaImage(const AlphaImage<T,ColorTraits>& old)
		: parent(old)
	{

	} // AlphaImage(AlphaImage)

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits>& AlphaImage<T,ColorTraits>::operator= (const AlphaImage<T,ColorTraits>& old)
	{
		// Generic check for self-assignment
		if( &old != this )
		{
			// The class currently has no members, but if they are added, they need
			// to be assigned here.

			parent::operator=(old);
		}
		return(*this);
	} // AlphaImage::operator=(AlphaImage)

	template <typename T, typename ColorTraits>
	void AlphaImage<T,ColorTraits>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		parent::collectInternalMembers(collector);
	}

	template <typename T, typename ColorTraits>
	bool AlphaImage<T,ColorTraits>::visible(int x, int y) const
	{
		return parent::inside(x,y) && ((*this)(x,y).a() < ColorTraits::max());
	}

	template <typename T, typename ColorTraits>
	AlphaImage<T,ColorTraits> scaleImage(const AlphaImage<T,ColorTraits>& img, unsigned scale)
	{

		AlphaImage<T,ColorTraits> ret_image(img.getWidth() * scale, img.getHeight() * scale);

		for(unsigned y = 0; y < ret_image.getHeight(); ++y)
		{
			unsigned y_norm = y / scale;
			for(unsigned x = 0; x < ret_image.getWidth(); ++x)
			{
				unsigned x_norm = x / scale;
				ret_image(x,y) = img(x_norm, y_norm);
			}
		}
		return ret_image;
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB_ALPHA_IMAGE_HPP) */

