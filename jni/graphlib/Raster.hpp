/*
 * $Id: Raster.hpp,v 1.1.2.2 2009/02/23 04:56:13 kpharris Exp $
 *
 * Part of GraphLib -- a framework for graphics production.
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

#if       !defined(GRAPHLIB_RASTER_HPP)
#define            GRAPHLIB_RASTER_HPP

/**
 * @author Kevin Harris
 */

#include "BaseClass.hpp"
#include "GenericRectangle.hpp"
#include "miniblocxx/stl/algorithm.hpp"
#include "miniblocxx/Exception.hpp"

namespace graphlib
{
	template <class T> class Scanline;

	/**
	 *
	 * A generic class to represent a raster (2d matrix).  This is useful for
	 * raster graphics and possibly lots of other fun stuff.  Note:  It does NOT
	 * have iterators (at this time).
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.1.2.2 $
	 *
	 */
	template <typename T>
	class Raster : public BaseClass
	{
	public:
		/** Create an empty raster */
		Raster();

		/** Size-specified constructor.  Creates a width by height sized raster */
		Raster(unsigned width, unsigned height);

		virtual ~Raster();
		Raster(const Raster& old);
		Raster& operator= (const Raster& old);

		/**
		 * Get the element at the given (x,y)
		 * @param x The column.
		 * @param y The row.
		 */
		inline T& operator()(unsigned x, unsigned y);
		/**
		 * Get the element at the given (x,y)
		 * @param x The column.
		 * @param y The row.
		 */
		inline const T& operator()(unsigned x, unsigned y) const;

		/**
		 * Get the Scanline at the given row (y)
		 * @param y The row of the Scanline.
		 * @returns A Scanline for the desired row.
		 */
		Scanline<T> operator[](unsigned y);

		/**
		 * Get the Scanline at the given row (y)
		 * @param y The row of the Scanline.
		 * @returns A Scanline for the desired row.
		 */
		const Scanline<T> operator[](unsigned y)const;

		bool inside(int x, int y) const
		{
			return (y >= 0) && (y < int(height)) && (x >= 0) && (x < int(width));
		}

		/**
		 * Return a Raster which contains the elements in the range
		 * (x1,y1) to (x2,y2) inclusive.
		 * @returns a new Raster that contains the data from the specified region.
		 */
		Raster<T> subRaster(unsigned x1, unsigned y1,
			unsigned x2, unsigned y2) const;

		Raster<T> subRaster(const GenericRectangle<unsigned>& rect) const;

		/**
		 * Resize the current Raster to the given width and height.  If preserve is
		 * given, the current data (if any), is copied across. If either width or
		 * height is zero, the current Raster will be resized to 0.
		 */
		void resize(unsigned width, unsigned height, bool preserve = true);

		/**
		 * Resize the current raster to the given width and height, shifting old
		 * data to have the upper left at ux,uy in the newly sized raster, and
		 * filling the rest with the given fill data.
		 *
		 * If ux or uy are out of range (abs(ux) >= width || abs(uy) >= height),
		 * no data will be copied and the entire raster will be filled.
		 */
		void resize(unsigned width, unsigned height,
			int ux, int uy, const T& fill);

		/**
		 *
		 * Perform a reinterpretation of the data that this class is using.
		 * This is NOT good programming practice, however, it is VERY useful for
		 * getting data out in a method that can be used for display/conversion in
		 * pre-existing graphics libraries.
		 *
		 * WARNING! Do not delete or otherwise attempt to deallocate the return
		 * value of this function.  Doing so will cause serious problems.  Use this
		 * function at your own peril.  Don't blame me when something bad happens
		 * if you use this.
		 *
		 * NOTE: The ability to successfully reinterpret the data contained within
		 * depends on the type used for this class.  Use of built-in types is
		 * strongly encouraged, or possibly the rgbcolor class in rgbcolor.hpp
		 * could be used, as it was written specifically for this purpose.
		 *
		 */
		template <class U>
		U reinterpret();

		/*
		 * Normally a bad thing to do, but this gives huge performance benefits
		 * for read-only operations.  Do not write to, free, or do any other
		 * mutating operations to the returned pointer.
		 */
		const T* getRawData() const
		{
			return raster_data;
		}

		/** Get the width of the raster */
		unsigned getWidth() const
		{
			return width;
		}
		/** Get the height of the raster */
		unsigned getHeight() const
		{
			return height;
		}
		/** Get the number of pixels in the raster */
		unsigned getNumPixels() const
		{
			return width * height;
		}
		/** Return if the raster has no data. */
		bool empty() const
		{
			return(raster_data == NULL);
		}

		// For some reason I can't yet explain, the covariant return types don't
		// work (apple gcc 4.0.1) when templates are in the mix, so this returns
		// an IntrusiveReference<Cloneable> instead of an
		// IntrusiveReference<Raster<T> >.
		CloneableRef clone() const;
		blocxx::String name() const;
		void collectInternalMembers(MemberStringDumpCollector& collector) const;

	private:
		/** The width and height of the raster */
		unsigned width;
		unsigned height;
		/** The actual raster data. */
		T* raster_data;

		void swap(Raster<T>& rast);
	}; // class Raster

	/**
	 * An internal class of class Raster which is used to provide simple
	 * multi-dimisionality.
	 *
	 * This class CANNOT be used seperately from class Raster.
	 *
	 * This makes it so that when the [] operator of Raster is called, the []
	 * operator can then be applied again (to the resulting Scanline) to get data
	 * from the 'second dimension'.
	 *
	 * Once a Scanline is created, it will ALWAYS refer to the Scanline it was
	 * created for.  It cannot be changed to modify another Raster, nor can the
	 * row number be changed.
	 *
	 * @author Kevin Harris
	 * @version $Revision: 1.1.2.2 $
	 */
	template <class T>
	class Scanline
	{
	public:
		friend class Raster<T>;

		/**
		 * Return an element of the Raster.
		 * @param x The index of the element on this Scanline.
		 * @returns The element requested.
		 */
		T& operator[](unsigned x);


		/**
		 * Return an element of the Raster.
		 * @param x The index of the element on this Scanline.
		 * @returns The element requested.
		 */
		const T& operator[](unsigned x) const;

		/**
		 * Tricky assignment operator, will copy one row (Scanline) of any Raster
		 * to another (if they are the same size).
		 * NOTE: The reference is NOT copied, and this Scanline will ALWAYS
		 * belong to the Raster that created it.
		 * @param line Another Scanline to use for copying data.  This is NOT
		 *   kept like the natural way of thinking for a real assignment.  Both
		 *   references remain exactly as they were, just the Raster has had its
		 *   data modified.
		 * @returns A reference to this Scanline, for chained assignments.
		 *   the same.
		 */
		Scanline& operator=(const Scanline& line);

	private:
		/**
		 * The constructor that only class Raster can invoke.
		 * @param ras A reference to a Raster for which this instance will ALWAYS
		 *   contain.  Any actions on this Scanline will always affect the given
		 *   Raster.
		 * @param row The row that this Scanline represents within the Raster.
		 */
		Scanline(Raster<T>& ras, unsigned row);

		/**
		 * The reference to the class which created this particular instance.
		 * Once this is set in the constructor, it will not ever be changed.
		 */
		Raster<T>& my_raster;
		/**
		 * The row (line) that this Scanline is supposed to represent.
		 * This is, like the above, permanent.
		 */
		unsigned my_row;
	}; // class Scanline


	// **********************************************************************
	// Member functions for class Raster
	// **********************************************************************

	template <class T>
	Raster<T>::Raster():
		width(0), height(0),
		raster_data(NULL)
	{
	} // Raster()

	template <class T>
	Raster<T>::Raster(unsigned w, unsigned h):
		width(w), height(h),
		raster_data(NULL)
	{
		unsigned linear_size = w * h;

		if( linear_size > 0 )
		{
			raster_data = new T[linear_size];
		}
		else
		{
			width = 0;
			height = 0;
		}
	} // Raster(unsigned,unsigned)

	template <class T>
	Raster<T>::~Raster()
	{
		// note: deletion of NULL is supposed to be safe.
		delete[] raster_data;
		raster_data = NULL;
		width = 0;
		height = 0;
	} // ~Raster()

	template <class T>
	Raster<T>::Raster(const Raster<T>& old)
		: BaseClass(old)
		, width(old.width)
		, height(old.height)
		, raster_data(NULL)
	{
		unsigned linear_size = width * height;

		if( linear_size > 0 )
		{
			raster_data = new T[linear_size];

			if( old.raster_data )
			{
				for( unsigned i = 0; i < linear_size; ++i )
				{
					raster_data[i] = old.raster_data[i];
				}
			}
		}
		else
		{
			width = 0;
			height = 0;
		}
	} // Raster(Raster)

	template <class T>
	Raster<T>& Raster<T>::operator= (const Raster<T>& old)
	{
		// Generic check for self-assignment
		if( &old != this )
		{
			// Instead of doing actual assignment, do a copy/swap.  This gives
			// strong exception safety.
			Raster<T> copy = old;
			this->swap(copy);

			// This doesn't do anything for this class, but is good to do in general.
			BaseClass::operator=(old);
		}
		return(*this);
	} // Raster::operator=(Raster)

#if defined(BOUNDS_CHECKING)
#define X_WITHIN_WIDTH(x) \
	do \
	{ \
		if( (x) >= width ) \
		{ \
			BLOCXX_THROW(OutOfBoundsException, blocxx::Format("X value %1 exceeds width of %2", (x), width).c_str()); \
		} \
	} \
	while(0)
#define Y_WITHIN_HEIGHT(y) \
	do \
	{ \
		if( (y) >= height ) \
		{ \
			BLOCXX_THROW(OutOfBoundsException, blocxx::Format("Y value %1 exceeds height of %2", (y), height).c_str()); \
		} \
	} \
	while(0)
#else
#define X_WITHIN_WIDTH(x)
#define Y_WITHIN_HEIGHT(y)
#endif

	template<class T>
	inline T& Raster<T>::operator()(unsigned x, unsigned y)
	{
		X_WITHIN_WIDTH(x);
		Y_WITHIN_HEIGHT(y);

		return raster_data[x + (y * width)];
	}

	template <class T>
	inline const T& Raster<T>::operator()(unsigned x, unsigned y) const
	{
		X_WITHIN_WIDTH(x);
		Y_WITHIN_HEIGHT(y);

		return raster_data[x + (y * width)];
	}

	template <class T>
	inline Scanline<T> Raster<T>::operator[](unsigned y)
	{
		Y_WITHIN_HEIGHT(y);

		return Scanline<T>(*(this), y);
	}

	template <class T>
	inline const Scanline<T> Raster<T>::operator[](unsigned y) const

	{
		Y_WITHIN_HEIGHT(y);

		return Scanline<T>(*const_cast<Raster<T>*>(this), y);
	}

	template <class T>
	Raster<T> Raster<T>::subRaster(const GenericRectangle<unsigned>& rect) const
	{
		return subRaster(rect.left, rect.top, rect.right, rect.bottom);
	}

	template <class T>
	Raster<T> Raster<T>::subRaster(unsigned x1, unsigned y1,
		unsigned x2, unsigned y2) const
	{
		X_WITHIN_WIDTH(x2);
		X_WITHIN_WIDTH(x1);
		Y_WITHIN_HEIGHT(y1);
		Y_WITHIN_HEIGHT(y2);

		if (x1 > x2 )
		{
//			BLOCXX_THROW(OutOfBoundsException, blocxx::Format("X dimensions are incorrectly ordered: %1 > %2", x1, x2).c_str());
		}
		if( y1 > y2 )
		{
//			BLOCXX_THROW(OutOfBoundsException, blocxx::Format("Y dimensions are incorrectly ordered: %1 > %2", y1, y2).c_str());
		}

		// Both of these add 1, because it is inclusive...
		unsigned x_range = x2 - x1 + 1;
		unsigned y_range = y2 - y1 + 1;

		Raster<T> ret_ras(x_range, y_range);

		for( unsigned y = 0; y < y_range; ++y )
		{
			for( unsigned x = 0; x < x_range; ++x )
			{
				unsigned source = (x + x1) + ((y + y1) * width);
				ret_ras.raster_data[x + y * x_range] = raster_data[source];
			}
		}
		return ret_ras;
	}

	template <class T>
	void Raster<T>::resize(unsigned width, unsigned height, bool preserve)
	{
		// If there is no change in size, do nothing but return.
		if( (width == this->width) && (height == this->height) )
		{
			return;
		}

		Raster<T> resized(width, height);

		// If it was requested to preserve data, copy as much as possible...
		if( preserve && this->raster_data && resized.raster_data )
		{
			unsigned max_x = blocxx::min(this->width, width);
			unsigned max_y = blocxx::min(this->height, height);
			for( unsigned y = 0; y < max_y; ++y )
			{
				for( unsigned x = 0; x < max_x; ++x )
				{
					resized.raster_data[(y * width) + x] = raster_data[(y * this->width) + x];
				}
			}
		}

		this->swap(resized);
	}

	template <class T>
	void Raster<T>::resize(unsigned width, unsigned height,
		int ux, int uy, const T& fill)
	{
		// If there is no change, return, doing nothing.
		if( (this->width == width) && (this->height == height) &&
			(ux == 0) && (uy == 0) )
		{
			return;
		}

		Raster<T> resized(width, height);

		// If it is possible to preserve data, copy as much as possible, and fill
		// the rest.
		if( !empty() && !resized.empty() )
		{
			int oux = ux + this->width;
			int ouy = uy + this->height;

			unsigned min_x = blocxx::max(ux, 0);
			unsigned max_x = blocxx::max(blocxx::min(oux, int(width)), 0);
			unsigned min_y = blocxx::max(uy, 0);
			unsigned max_y = blocxx::max(blocxx::min(ouy, int(height)), 0);

			int x_shift = -ux;
			int y_shift = -uy;
			// Copy any useful data.
			for( unsigned y = min_y; y < max_y; ++y )
			{
				unsigned old_y = y + y_shift;
				unsigned old_lin_y = old_y * this->width;
				unsigned lin_y = y * width;
				for( unsigned x = min_x; x < max_x; ++x )
				{
					unsigned old_x = x + x_shift;
					resized.raster_data[lin_y + x] = raster_data[old_lin_y + old_x];
				}
			}

			// Now that the data is copied, fill the rest of the Raster in the
			// largest stripes possible.  Here's what it looks like in the simple
			// case:
			//
			// 1111111111111
			// 1111111111111
			// 3333      444
			// 3333      444
			// 3333      444
			// 2222222222222
			// 2222222222222
			// 2222222222222
			//
			// Where numbers correspond to the numbered loops/tests below, and the
			// blanks are prefilled with copied data.
			//
			// These divisions were chosen to maximize the filling amount of a single
			// given loop (y-major).
			//
			// Note that the loops will not execute, and a maximum of 4 useless tests
			// will be performed, if the entire Raster was filled with old data.

			// (1) Fill above the copied rectangle, if it needs it...
			for( unsigned y = 0; y < min_y; ++y )
			{
				for( unsigned x = 0; x < width; ++x )
				{
					resized.raster_data[(y * width) + x] = fill;
				}
			}
			// (2) Fill below the copied rectangle, if it needs it...
			for( unsigned y = max_y; y < height; ++y )
			{
				for( unsigned x = 0; x < width; ++x )
				{
					resized.raster_data[(y * width) + x] = fill;
				}
			}
			// (3) Fill the gap on the left (if any)
			if( min_x > 0 )
			{
				for( unsigned y = min_y; y < max_y; ++y )
				{
					for( unsigned x = 0; x < min_x; ++x )
					{
						resized.raster_data[(y * width) + x] = fill;
					}
				}
			}
			// (4) Fill the gap on the right (if any)
			if( max_x < width )
			{
				for( unsigned y = min_y; y < max_y; ++y )
				{
					for( unsigned x = max_x; x < width; ++x )
					{
						resized.raster_data[(y * width) + x] = fill;
					}
				}
			}
		}
		else
		{
			// Preservation is not possible, fill everything.
			unsigned max_linear = width * height;
			for( unsigned i = 0; i < max_linear; ++i )
			{
				resized.raster_data[i] = fill;
			}
		}

		this->swap(resized);
	}


	template <class T>
	template <class U>
	inline U Raster<T>::reinterpret()
	{
		return reinterpret_cast<U>(raster_data);
	}

	template <typename T>
	CloneableRef Raster<T>::clone() const
	{
		return CloneableRef(new Raster<T>(*this));
	}

	template <typename T>
	blocxx::String Raster<T>::name() const
	{
		return "Raster";
	}

	template <typename T>
	void Raster<T>::collectInternalMembers(MemberStringDumpCollector& collector) const
	{
		collector.addMember("width", width);
		collector.addMember("height", height);
		collector.addMember("raster_data", "...");
	}

	template <typename T>
	void Raster<T>::swap(Raster<T>& rast)
	{
		blocxx::swap(width, rast.width);
		blocxx::swap(height, rast.height);
		blocxx::swap(raster_data, rast.raster_data);
	}

	// **********************************************************************
	// Member functions for class Raster::Scanline
	// **********************************************************************

	//------------------------------------------------------------------
	// The one and only constructor for Raster's internal class Scanline
	// Note: this is a non-public constructor...
	//------------------------------------------------------------------
	template <class T>
	Scanline<T>::Scanline(Raster<T>& ras, unsigned row):
		my_raster(ras),
		my_row(row)
	{
	}

	template <class T>
	inline T& Scanline<T>::operator[](unsigned x)
	{
		unsigned width = my_raster.getWidth();
		X_WITHIN_WIDTH(x);

		return my_raster(x, my_row);
	} // Scanline::operator[](x)

	template <class T>
	inline const T& Scanline<T>::operator[](unsigned x) const
	{
		unsigned width = my_raster.getWidth();
		X_WITHIN_WIDTH(x);

		return my_raster(x, my_row);
	} // Scanline::operator[](x) const

	template <class T>
	Scanline<T>& Scanline<T>::operator=(const Scanline& line)
	{
		// Generic check for self-assignment
		if( &line != this )
		{
			if( line.my_raster.getWidth() == my_raster.getWidth() )
			{
				for( unsigned i = 0; i < my_raster.getWidth(); ++i )
				{
					my_raster(i, my_row) = line.my_raster(i, line.my_row);
				}
			}
			else
			{
				BLOCXX_THROW(OutOfBoundsException, Format("Raster widths are different lhs=%1, rhs=%2",
						my_raster.getWidth(), line.my_raster.getWidth()).c_str());
			}
		}
		return *this;
	} // Scanline::operator=(Scanline)

#undef X_WITHIN_WIDTH
#undef Y_WITHIN_HEIGHT

} // namespace graphlib


#endif /* !defined(GRAPHLIB_RASTER_HPP) */
