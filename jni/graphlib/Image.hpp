/*
 * $Id: image.hpp,v 1.7 2008/06/21 22:25:10 kpharris Exp $
 *
 * Part of "Graphlib" a playground for graphics development
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

#if       !defined(GRAPHLIB_IMAGE_HPP)
#define            GRAPHLIB_IMAGE_HPP

// --------------------------------------
// Default include of parent class header
// --------------------------------------
#include "graphlib/Raster.hpp"
#include "graphlib/GenericRGBColor.hpp"

namespace graphlib
{

	// Sorry about these comments, as they are meant to be viewed in html form.
	/**
	 *
	 * A generic class which combines the capabilities of Raster and RGBColor.
	 * This class has almost no functionality of its own.
	 *
	 * Note the neat feature of this class: it can be reinterpreted as a pointer
	 * to the base type given to it.
	 *
	 * For example:<br>
	 * <font color="#008800">Image</font>&lt;<font color="#000088">unsigned
	 * char</font>&gt im(640, 480);
	 *
	 * <font color="#880000">
	 * // This next variable can be treated as a single array of size      <br>
	 * // im.numpixels( ) in length, which would be 307200 in this case.   <br>
	 * </font>
	 * <font color="#000088">unsigned char</font>*
	 * image_data</font> =
	 * im.<font color="#008800">reinterpret</font>
	 * &lt;<font color="#000088">unsigned char</font>*&gt; ( );<br>
	 * ...<br>
	 * <font color="#880000">
	 * // Act on the data, being careful not to go beyond bounds on either <br>
	 * // end, or (whether intentional or not) deallocate the pointer.     <br>
	 * </font>
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.7 $
	 * @see Raster
	 * @see RGBColor
	 *
	 */
	template <typename T, typename ColorTraits = typename GenericRGBColor<T>::color_traits_type>
	class Image : public Raster<GenericRGBColor<T,ColorTraits> >
	{
	public:
		typedef GenericRGBColor<T,ColorTraits> color_type;
		typedef Raster<color_type> parent;

		Image();
		Image(const parent& r);
		Image(unsigned width, unsigned height);
		virtual ~Image();
		Image(const Image& old);
		Image& operator= (const Image& old);

		CloneableRef clone() const { return CloneableRef(new Image(*this)); }
		blocxx::String name() const { return "Image"; }
		void collectInternalMembers(MemberStringDumpCollector& collector) const;

		// Return if the pixel at x,y is visible.  For most uses, you really
		// don't want to do this for each pixel.  Know what image type you have
		// before you try to draw it.
		virtual bool visible(int x, int y) const;

		Image subImage(const GenericRectangle<unsigned>& rect) const
		{
			return Image(parent::subRaster(rect));
		}
	}; // class Image

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>::Image()
		: parent()
	{

	} // Image()

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>::Image(const parent& r) :
		parent(r)
	{
	}

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>::Image(unsigned width, unsigned height):
		parent(width, height)
	{

	} // Image(w,h)

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>::~Image()
	{

	} // ~Image()

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>::Image(const Image<T,ColorTraits>& old):
		parent(old)
	{

	} // Image(Image)

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits>& Image<T,ColorTraits>::operator= (const Image<T,ColorTraits>& old)
	{
		// Generic check for self-assignment
		if( &old != this )
		{
			// The class currently has no members, but if they are added, they need
			// to be assigned here.

			parent::operator=(old);
		}
		return(*this);
	} // Image::operator=(Image)

	template <typename T, typename ColorTraits>
	void Image<T,ColorTraits>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		parent::collectInternalMembers(collector);
	}

	template <typename T, typename ColorTraits>
	bool Image<T,ColorTraits>::visible(int x, int y) const
	{
		return parent::inside(x,y);
	}

	template <typename T, typename ColorTraits>
	Image<T,ColorTraits> scaleImage(const Image<T,ColorTraits>& img, unsigned scale)
	{

		Image<T,ColorTraits> ret_image(img.getWidth() * scale, img.getHeight() * scale);

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


#endif /* !defined(GRAPHLIB_IMAGE_HPP) */

