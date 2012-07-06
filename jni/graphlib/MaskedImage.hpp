/*
 * $Id: MaskedImage.hpp,v 1.3 2005/03/28 08:34:57 kpharris Exp $
 *
 * Part of "Graphlib"
 * Copyright (C) 2003 Kevin Harris
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

#if       !defined(GRAPHLIB_MASKEDIMAGE_HPP)
#define            GRAPHLIB_MASKEDIMAGE_HPP

#include "graphlib/Image.hpp"
#include "graphlib/Raster.hpp"
#include "graphlib/GenericRectangle.hpp"

namespace graphlib
{

	/**
	 *
	 * An Image which has a mask for use in determining weather or not the color
	 * value should be used.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.3 $
	 *
	 */
	template <typename T, typename ColorTraits = typename GenericRGBColor<T>::color_traits_type, typename MaskType = bool>
	class MaskedImage : public Image<T,ColorTraits>, public virtual StringDumpable
	{
	private:
		typedef Image<T,ColorTraits> parent;
	public:
		typedef typename parent::color_type color_type;

		MaskedImage();
		MaskedImage(const Image<T,ColorTraits>& img, const color_type& transparent);
		MaskedImage(const Image<T,ColorTraits>& img, const Raster<MaskType>& msk);
		MaskedImage(unsigned width, unsigned height);
		MaskedImage(unsigned width, unsigned height,	const color_type* pdata, const MaskType* mdata);

		virtual ~MaskedImage();
		MaskedImage(const MaskedImage& old);
		MaskedImage& operator= (const MaskedImage& old);

		color_type& operator()(unsigned x, unsigned y) { return parent::operator()(x,y); }
		const color_type& operator()(unsigned x, unsigned y) const { return parent::operator()(x,y); }

		bool empty() const { return parent::empty(); }
		unsigned getWidth() const { return parent::getWidth(); }
		unsigned getHeight() const { return parent::getHeight(); }

		/**
		 * Get the mask element at the given (x,y)
		 * @param x The column.
		 * @param y The row.
		 */
		MaskType& mask(unsigned x, unsigned y)
		{
			return my_mask(x,y);
		}

		/**
		 * Get the mask element at the given (x,y)
		 * @param x The column.
		 * @param y The row.
		 */
		MaskType mask(unsigned x, unsigned y) const
		{ return my_mask(x,y); }

		/**
		 * Resize the current masked Image to the given width and height.  If
		 * preserve is given, the current data (if any), is copied across. If
		 * either width or height is zero, the current masked Image will be resized
		 * to 0.
		 */
		void resize(unsigned width, unsigned height, bool preserve = true)
		{
			Image<T,ColorTraits>::resize(width, height, preserve);
			my_mask.resize(width, height, preserve);
		}

		/**
		 * Resize the current masked Image to the given width and height, shifting
		 * old data to be centered at cx,cy in the newly sized masked Image, and
		 * filling the rest with the given fill data.
		 */
		void resize(unsigned width, unsigned height,
			int cx, int cy, const GenericRGBColor<T,ColorTraits>& fill1, const MaskType& fill2)
		{
			Raster<GenericRGBColor<T,ColorTraits> >::resize(width, height, cx, cy, fill1);
			my_mask.resize(width, height, cx, cy, fill2);
		}


		/**
		 * Return a Image which contains the elements in the range
		 * (x1,y1) to (x2,y2) inclusive.
		 * @returns a new Image that contains the data from the specified region.
		 */
		MaskedImage subImage(unsigned x1, unsigned y1,
			unsigned x2, unsigned y2) const
		{
			return MaskedImage<T,MaskType>(Image<T,ColorTraits>::subImage(x1,y1,x2,y2),
				my_mask.subRaster(x1,y1,x2,y2));
		}

		/**
		 * Return a Image which contains the elements in the range
		 * (rect.left, rect.top) to (rect.right, rect.bottom) inclusive.
		 * @returns a new Image that contains the data from the specified region.
		 */
		MaskedImage subImage(const GenericRectangle<unsigned>& rect) const
		{
			return MaskedImage<T,MaskType>(
				Image<T,ColorTraits>::subImage(rect),
				my_mask.subRaster(rect));
		}

		bool visible(int x, int y) const;

		CloneableRef clone() const { return CloneableRef(new MaskedImage(*this)); }
		blocxx::String name() const { return "MaskedImage"; }
		void collectInternalMembers(MemberStringDumpCollector& collector) const;

	private:
		Raster<MaskType> my_mask;
	}; // class MaskedImage

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage():
		Image<T,ColorTraits>(),
		my_mask()
	{

	} // MaskedImage()

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage(unsigned w, unsigned h):
		Image<T,ColorTraits>(w, h),
		my_mask(w, h)
	{

	} // MaskedImage(w,h)

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage(const Image<T,ColorTraits>& img,
		const Raster<MaskType>& msk):
		Image<T,ColorTraits>(img),
		my_mask(msk)
	{

		// Make sure the sizes are the same....
		my_mask.resize(this->getWidth(), this->getHeight(), true);

	} // MaskedImage(Image,raster)

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage(unsigned width, unsigned height,
		const color_type* pdata,
		const MaskType* mdata):
		Image<T,ColorTraits>(width, height, pdata),
		my_mask(width, height, mdata)
	{

	} // MaskedImage(unsigned,unsigned,rgbcolor*,MaskType*)

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::~MaskedImage()
	{

	} // ~MaskedImage()

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage(const MaskedImage& old):
		Image<T,ColorTraits>(old),
		my_mask(old.my_mask)
	{

	} // MaskedImage(MaskedImage)

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>& MaskedImage<T,ColorTraits,MaskType>::operator= (const MaskedImage& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{
			my_mask = old.my_mask;

			Image<T,ColorTraits>::operator=(old);
		}
		return (*this);
	} // MaskedImage::operator=(MaskedImage)


	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType>::MaskedImage(const Image<T,ColorTraits>& image, const color_type& transparent)
		: Image<T,ColorTraits>(image)
		, my_mask(image.getWidth(), image.getHeight())
	{
		for( unsigned y = 0; y < getHeight(); ++y )
		{
			for(unsigned x = 0; x < getWidth(); ++x )
			{
				const color_type& c = (*this)(x,y);
				mask(x,y) = !((c.r() == transparent.r()) && (c.g() == transparent.g()) && (c.b() == transparent.b()));
			}
		}
	}

	template <typename T, typename ColorTraits, typename MaskType>
	MaskedImage<T,ColorTraits,MaskType> scaleImage(const MaskedImage<T,ColorTraits,MaskType>& image, unsigned scale)
	{

		MaskedImage<T,ColorTraits,MaskType> ret_image(image.getWidth() * scale,
			image.getHeight() * scale);

		for(unsigned y = 0; y < ret_image.getHeight(); ++y)
		{
			unsigned y_norm = y / scale;
			for(unsigned x = 0; x < ret_image.getWidth(); ++x)
			{
				unsigned x_norm = x / scale;
				ret_image(x,y) = image(x_norm, y_norm);
				ret_image.mask(x,y) = image.mask(x_norm, y_norm);
			}
		}
		return ret_image;
	}

	template <typename T, typename ColorTraits, typename MaskType>
	bool MaskedImage<T,ColorTraits,MaskType>::visible(int x, int y) const
	{
		return (
			(x > 0)
			&& (y > 0)
			&& (x < int(my_mask.getWidth()))
			&& (y < int(my_mask.getHeight()))
			&& my_mask(x,y));
	}

	template <typename T, typename ColorTraits, typename MaskType>
	void MaskedImage<T,ColorTraits,MaskType>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		parent::collectInternalMembers(collector);
		collector.addMember("mask", "...");
	}

} // namespace graphlib


#endif /* !defined(GRAPHLIB_MASKEDIMAGE_HPP) */
