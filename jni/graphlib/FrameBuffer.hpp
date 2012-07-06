/*
 * $Id: FrameBuffer.hpp,v 1.3 2003/11/10 09:32:27 kpharris Exp $
 *
 * Part of "Graphlib" - A nearly infinitely expandable gaming framework
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

#if       !defined(GRAPHLIB_FRAMEBUFFER_HPP)
#define            GRAPHLIB_FRAMEBUFFER_HPP

#include "GenericRGBColor.hpp"
#include "GenericRGBAColor.hpp"
#include "Raster.hpp"
#include "Image.hpp"
#include "MaskedImage.hpp"
#include "BaseClass.hpp"

namespace graphlib
{
	template <typename SourceTraits, typename DestTraits, typename T1, typename T2>
	T2 convertRangeUsingTraits(T1 t)
	{
		return T2(typename blocxx::Larger<T2>::type(DestTraits::max()) * t / SourceTraits::max());
	}

	template <typename T, typename ColorTraits = RGBColorTraits<T> >
	class FrameBuffer : public BaseClass
	{
	public:
		typedef GenericRGBColor<T, ColorTraits> color_type;

		FrameBuffer();
		virtual ~FrameBuffer();
		FrameBuffer(const FrameBuffer& old);
		FrameBuffer& operator= (const FrameBuffer& old);

		/** Get the current foreground image buffer (read only) */
		const Raster<color_type>& foreground() const { return *m_foreground; }
		/** Get the current background image buffer (read only) */
		const Raster<color_type>& background() const { return *m_background; }

		/** Swap the foreground and background buffers. */
		void flip() { std::swap(m_foreground, m_background); }

		/** Get the width of the screen */
		unsigned getWidth()  const { return m_buffer1.getWidth(); }
		/** Get the height of the screen */
		unsigned getHeight() const { return m_buffer1.getHeight(); }

		/**
		 * Called to tell any subclasses that they need to update the buffers, and
		 * flip if required.
		 *
		 * This function *MUST* be overridden (pure virtual).
		 */
		virtual void update() = 0;

		/**
		 * Called to tell any subclasses that they needs to draw the current
		 * foreground buffer in whatever method they can (platform specific
		 * subclasses can do this).
		 *
		 * This function *MUST* be overridden (pure virtual).
		 */
		virtual void draw() = 0;

		/**
		 * Resize the current images to the given width and height.  If preserve is
		 * given, the current data (if any), is copied across. If either width or
		 * height is zero, the current images will be resized to 0.
		 *
		 */
		void resize(unsigned width, unsigned height, bool preserve = true);

		/**
		 * Resize the current images to the given width and height, shifting old
		 * data to be centered at cx,cy in the newly sized images, and filling the
		 * rest with the given fill data.
		 *
		 */
		void resize(unsigned width, unsigned height, int cx, int cy, const color_type& fill);


		/** Set the pixel at x,y to the given color (in the background buffer). */
		virtual void setpixel_back(unsigned x, unsigned y, const color_type& c)
		{
			(*m_background)(x,y) = c;
		}

		/** Set the pixel at x,y to the given color (in the foreground buffer) */
		virtual void setpixel(unsigned x, unsigned y, const color_type& c)
		{
			(*m_foreground)(x,y) = c;
		}

		template <typename T2, typename ColorTraits2>
		T alphaBlendComponent(T c1, T2 c2, T alpha)
		{
			typedef typename blocxx::Larger<T>::type bigger;
			T convertedComponent = convertRangeUsingTraits<ColorTraits2, ColorTraits, T2, T>(c2);

			return T(
				( bigger(alpha) * c1 +
					bigger(ColorTraits::max() - alpha) * convertedComponent
				) / ColorTraits::max());
		}

		template <typename T2, typename ColorTraits2>
		color_type alphaBlend(const color_type& c1, const GenericRGBAColor<T2, ColorTraits2>& c2)
		{
			T convertedAlpha = convertRangeUsingTraits<ColorTraits2, ColorTraits, T2, T>(c2.a());
			return color_type(
				alphaBlendComponent<T2,ColorTraits2>(c1.r(), c2.r(), convertedAlpha),
				alphaBlendComponent<T2,ColorTraits2>(c1.g(), c2.g(), convertedAlpha),
				alphaBlendComponent<T2,ColorTraits2>(c1.b(), c2.b(), convertedAlpha)
			);
		}

		/** Set the pixel at x,y to the given color (in the background buffer). */
		template <typename T2, typename ColorTraits2>
		void setpixel_back(unsigned x, unsigned y, const GenericRGBAColor<T2,ColorTraits2>& c)
		{
			color_type cur_color = getpixel_back(x,y);
			setpixel_back(x,y, alphaBlend(cur_color, c));
		}

		/** Set the pixel at x,y to the given color (in the foreground buffer) */
		template <typename T2, typename ColorTraits2>
		void setpixel(unsigned x, unsigned y, const GenericRGBAColor<T2,ColorTraits2>& c)
		{
			color_type cur_color = getpixel(x,y);
			setpixel(x,y, alphaBlend(cur_color, c));
		}

		/** Get the pixel at x,y (from the background buffer). */
		virtual color_type getpixel_back(unsigned x, unsigned y)
		{
			return ((*m_background)(x,y));
		}

		/** Get the pixel at x,y (from the foreground buffer) */
		virtual color_type getpixel(unsigned x, unsigned y)
		{
			return ((*m_foreground)(x,y));
		}

		void fg_draw_image(const Raster<color_type>& img, int x, int y);
		void bg_draw_image(const Raster<color_type>& img, int x, int y);

		void fg_draw_image(const Image<T>& img, int x, int y);
		void bg_draw_image(const Image<T>& img, int x, int y);

		void fg_draw_image(const MaskedImage<T>& img, int x, int y);
		void bg_draw_image(const MaskedImage<T>& img, int x, int y);

		template <typename T2, typename ColorTraits2>
		void fg_draw_image(const Raster<GenericRGBAColor<T2, ColorTraits2> >& img, int x, int y);
		template <typename T2, typename ColorTraits2>
		void bg_draw_image(const Raster<GenericRGBAColor<T2, ColorTraits2> >& img, int x, int y);

		virtual CloneableRef clone() const = 0;
		blocxx::String name() const;
		void collectInternalMembers(MemberStringDumpCollector& collector) const;

	protected:
		/**
		 * Two buffers to be used as foreground and background... They will
		 * *always* have the same size.
		 */
		Raster<color_type> m_buffer1, m_buffer2;

		Raster<color_type> *m_foreground; ///< The current foreground buffer
		Raster<color_type> *m_background; ///< The current background buffer
	};

	template <typename T, typename ColorTraits>
	FrameBuffer<T, ColorTraits>::FrameBuffer()
		: m_buffer1()
		, m_buffer2()
		, m_foreground(&m_buffer1)
		, m_background(&m_buffer2)
	{
	}

	template <typename T, typename ColorTraits>
	FrameBuffer<T, ColorTraits>::~FrameBuffer()
	{
		m_foreground = NULL;
		m_background = NULL;

		// Everything else has a destructor.
	}

	template <typename T, typename ColorTraits>
	FrameBuffer<T, ColorTraits>::FrameBuffer(const FrameBuffer<T, ColorTraits>& old)
	{
		// Easier than rewriting everything.  Raster has a fairly simple default
		// constructor, so this won't have a lot of penalty.
		*this = old;
	}

	template <typename T, typename ColorTraits>
	FrameBuffer<T,ColorTraits>& FrameBuffer<T, ColorTraits>::operator= (const FrameBuffer<T,ColorTraits>& old)
	{
		m_buffer1 = old.m_buffer1;
		m_buffer2 = old.m_buffer2;

		// Preserve the fore-background order of the old buffers.
		if( old.m_foreground == &old.m_buffer1 )
		{
			m_foreground = &m_buffer1;
			m_background = &m_buffer2;
		}
		else if( old.m_foreground == &old.m_buffer2 )
		{
			m_foreground = &m_buffer2;
			m_background = &m_buffer1;
		}
		else
		{
			// Something is fooed up! [FIXME] Potentially report an error.
			m_foreground = &m_buffer1;
			m_background = &m_buffer2;
		}

		return *this;
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T, ColorTraits>::resize(unsigned width, unsigned height, bool preserve)
	{
		m_buffer1.resize(width, height, preserve);
		m_buffer2.resize(width, height, preserve);
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T, ColorTraits>::resize(unsigned width, unsigned height, int cx, int cy, const color_type& fill)
	{
		m_buffer1.resize(width, height, cx, cy, fill);
		m_buffer2.resize(width, height, cx, cy, fill);
	}

	template <typename T, typename ColorTraits>
	blocxx::String FrameBuffer<T, ColorTraits>::name() const
	{
		return "FrameBuffer";
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T, ColorTraits>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("width", m_buffer1.getWidth());
		collector.addMember("height", m_buffer1.getHeight());
		collector.addMember("foreground", "...");
		collector.addMember("background", "...");
	}


	template <typename RasterType1, typename RasterType2>
	void getBoundaries(const RasterType1& buffer, const RasterType2& img, int x, int y, unsigned& x_min, unsigned& y_min, unsigned& x_max, unsigned& y_max)
	{
		unsigned width = buffer.getWidth();
		unsigned height = buffer.getHeight();

		x_min = std::min(width, unsigned(std::max(0, x)));
		x_max = std::min(width, unsigned(std::max(0u, unsigned(img.getWidth()) + x)));
		y_min = std::min(height, unsigned(std::max(0, y)));
		y_max = std::min(height, unsigned(std::max(0u, unsigned(img.getHeight()) + y)));
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::fg_draw_image(const Raster<color_type>& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_foreground, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					(*m_foreground)(x1,y1) = img(x1 - x, y1 - y);
				}
			}
		}
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::bg_draw_image(const Raster<color_type>& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_background, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					(*m_background)(x1,y1) = img(x1 - x, y1 - y);
				}
			}
		}
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::fg_draw_image(const Image<T>& img, int x, int y)
	{
		fb_draw_image(reinterpret_cast<const Raster<color_type>&>(img), x, y);
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::bg_draw_image(const Image<T>& img, int x, int y)
	{
		bg_draw_image(reinterpret_cast<const Raster<color_type>&>(img), x, y);
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::fg_draw_image(const MaskedImage<T>& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_foreground, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					if( img.mask(x1 - x, y1 - y) )
					{
						(*m_foreground)(x1,y1) = img(x1 - x, y1 - y);
					}
				}
			}
		}
	}

	template <typename T, typename ColorTraits>
	void FrameBuffer<T,ColorTraits>::bg_draw_image(const MaskedImage<T>& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_foreground, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					if( img.mask(x1 - x, y1 - y) )
					{
						(*m_background)(x1,y1) = img(x1 - x, y1 - y);
					}
				}
			}
		}
	}

	template <typename T, typename ColorTraits>
	template <typename T2, typename ColorTraits2>
	void FrameBuffer<T,ColorTraits>::fg_draw_image(const Raster<GenericRGBAColor<T2, ColorTraits2> >& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_foreground, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					(*m_foreground)(x1,y1) = alphaBlend((*m_foreground)(x1,y1), img(x1 - x, y1 - y));
				}
			}
		}
	}

	template <typename T, typename ColorTraits>
	template <typename T2, typename ColorTraits2>
	void FrameBuffer<T,ColorTraits>::bg_draw_image(const Raster<GenericRGBAColor<T2, ColorTraits2> >& img, int x, int y)
	{
		if( !img.empty() )
		{
			unsigned x_min, y_min, x_max, y_max;
			getBoundaries(*m_background, img, x, y, x_min, y_min, x_max, y_max);

			for(unsigned y1 = y_min; y1 < y_max; ++y1)
			{
				for(unsigned x1 = x_min; x1 < x_max; ++x1)
				{
					(*m_background)(x1,y1) = alphaBlend((*m_background)(x1,y1), img(x1 - x, y1 - y));
				}
			}
		}
	}
}

#endif /* !defined(GRAPHLIB_FRAMEBUFFER_HPP) */
