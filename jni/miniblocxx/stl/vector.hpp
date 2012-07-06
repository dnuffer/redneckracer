/*
 * $Id: vector.hpp,v 1.7 2008/12/15 17:27:26 kpharris Exp $
 *
 * Part of "blocxx" -- A playground for graphics development.
 * Copyright (C) 2004 Kevin Harris
 */

#if       !defined(BLOCXX__vector_HPP)
#define            BLOCXX__vector_HPP

// Include for the placement new operator.
#include <new>

/**
 * The size (bytes) to increment after this size has been reached.  Before
 * this size, the size will be repeatedly doubled.  Note that for some types,
 * this size can never be reached evenly, so it is just an approximation.
 */
#if       !defined(BLOCXX_VECTOR_INCREMENT_SIZE)
#define BLOCXX_VECTOR_INCREMENT_SIZE 2048
#endif /* !defined(BLOCXX_VECTOR_INCREMENT_SIZE) */

#include "miniblocxx/stl/iterator.hpp"

namespace BLOCXX_NAMESPACE
{
	/**
	 *
	 * A quick vector (appenendable only, directly accessible).  This is intended
	 * to give a little faster speed over the STL vector.  The size of it cannot
	 * be decreased, except by assigning another smaller vector to it.
	 *
	 * Note: This will not be thread safe. locks/mutexes should be used around
	 * any critical sections (construction, appends, etc).
	 *
	 * Exception safety for constructors and other operations can be gained by
	 * defining BLOCXX_EXCEPTION_SAFETY.  This ensures that exceptions thrown
	 * while initializing elements of the array will not cause memory leaks from
	 * any elements which have already been initialized.  For built-in types
	 * (int, float, etc), this exception safety is not required, and speed will
	 * be gained by NOT defining this value.
	 *
	 * @author Kevin Harris <kpharris@users.sourceforge.net>
	 * @version $Revision: 1.7 $
	 *
	 */
	template<class T>
	class vector
	{
	public:
		typedef random_access_iterator<T> iterator;
		typedef const_random_access_iterator<T> const_iterator;
		typedef blocxx::reverse_iterator<iterator> reverse_iterator;
		typedef blocxx::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef T value_type;
		typedef size_t size_type;
		typedef ssize_t difference_type;
		typedef T& reference;
		typedef const T& const_reference;

	private:
		T* data_pointer;
		T* end_pointer;
		T* storage_end;

		/**
		 * Call the destructor for all currently existing array elements, but leave
		 * the current data pointer intact.
		 */
		void destroy_data();

		/**
		 * Remove the data pointer, setting things to a known state.
		 */
		void remove_storage();

		enum resize_type { E_ABSOLUTE_RESIZE, E_NORMAL_RESIZE };

		/**
		 * Increase the storage space in the vector.
		 */
		void increase_storage(size_t desired, resize_type size_method = E_ABSOLUTE_RESIZE);

		/**
		 * Initialize from iterators.  Requires that the current storage is empty.
		 */
		template <class iter_type>
		void initialize_from_iterators(iter_type first, iter_type last);

		inline size_t next_larger_size(size_t s) const;

	protected:

	public:
		/** Default constructor (empty) */
		vector();

		/** Secondary constructor (sized) */
		vector(size_t size);

		/** Tertiary constructor (sized/initialized) */
		vector(size_t size, const T& initial_value);

		/** Create a vector from an iterator set. */
		template <class iter_type>
		vector(iter_type first, iter_type last);

		/** Destructor */
		virtual ~vector();

		/** Copy constructor */
		vector(const vector& old);

		/** Assignment operator */
		vector& operator= (const vector& old);

		/** Subscript operator (not size checked) */
		inline T& operator[](size_t index)
		{
			return data_pointer[index];
		}
		/** Subscript operator (not size checked) */
		inline const T& operator[](size_t index) const
		{
			return data_pointer[index];
		}
		/** Tells if the vector is empty */
		inline bool empty() const
		{
			return data_pointer == end_pointer;
		}
		/** Returns the size of the vector */
		inline size_t size() const
		{
			return end_pointer - data_pointer;
		}

		inline void resize(size_t size)
		{
			increase_storage(size);
			for( size_t count = this->size(); count < size; ++count )
			{
				new(end_pointer++) T();
			}
		}

		inline iterator begin() { return iterator(data_pointer); }
		inline iterator end() { return iterator(end_pointer); }
		inline const_iterator begin() const { return const_iterator(data_pointer); }
		inline const_iterator end() const { return const_iterator(end_pointer); }

#if 0
		inline reverse_iterator rbegin() { return reverse_iterator(end()); }
		inline reverse_iterator rend() { return reverse_iterator(begin()); }
		inline const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		inline const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
#endif

		void append(const T& val);
		void append(const vector<T>& vec);

		/* push_back is for interface compat with std::vector. It just forwards to append.*/
		void push_back(const T& val);
		void push_back(const vector<T>& vec);

		iterator erase(iterator location);
		iterator erase(iterator first, iterator last);
		void clear();

		inline void reserve(size_t desired) { increase_storage(desired); }

	}; // class vector


	//-------------------------------------------
	// Default constructor for class vector
	//-------------------------------------------
	template<class T>
	inline vector<T>::vector():
		data_pointer(NULL),
		end_pointer(NULL),
		storage_end(NULL)
	{
		const size_t minimum_size = 16;
		increase_storage(minimum_size, E_ABSOLUTE_RESIZE);
	} // vector()


	template<class T>
	inline void vector<T>::destroy_data()
	{
		if( data_pointer )
		{
			for( T* ptr = data_pointer; ptr != end_pointer; ++ptr )
			{
				ptr->~T();
			}
		}
		end_pointer = data_pointer;
	}

	template<class T>
	inline void vector<T>::remove_storage()
	{
		destroy_data();
		delete[] reinterpret_cast<char*>(data_pointer);
		data_pointer = NULL;
		end_pointer = NULL;
		storage_end = NULL;
	}

	//-------------------------------------------
	// Secondary constructor for class vector
	//-------------------------------------------
	template<class T>
	inline vector<T>::vector(size_t size):
		data_pointer(NULL),
		end_pointer(NULL),
		storage_end(NULL)
	{
		increase_storage(size);

#if defined(BLOCXX_EXCEPTION_SAFETY)
		try
#endif
		{
			for( end_pointer = data_pointer; end_pointer != storage_end; ++end_pointer )
			{
				new(end_pointer) T();
			}
		}
#if defined(BLOCXX_EXCEPTION_SAFETY)
		catch(...)
		{
			// Clean up any pre-constructed items.
			remove_storage();

			// Rethrow the exception.
			throw;
		}
#endif
	} // vector()


	//-------------------------------------------
	// Default constructor for class vector
	//-------------------------------------------
	template<class T>
	inline vector<T>::vector(size_t size, const T& initial_value):
		data_pointer(NULL),
		end_pointer(NULL),
		storage_end(NULL)
	{
		increase_storage(size, E_ABSOLUTE_RESIZE);
#if defined(BLOCXX_EXCEPTION_SAFETY)
		try
#endif
		{
			for( end_pointer = data_pointer; end_pointer != storage_end; ++end_pointer )
			{
				new(end_pointer) T(initial_value);
			}
		}
#if defined(BLOCXX_EXCEPTION_SAFETY)
		catch(...)
		{
			// Clean up any pre-constructed items.
			remove_storage();

			// Rethrow the exception.
			throw;
		}
#endif
	} // vector()

	template<class T>
	template <class iter_type>
	inline vector<T>::vector(iter_type first, iter_type last):
		data_pointer(NULL),
		end_pointer(NULL),
		storage_end(NULL)
	{
		initialize_from_iterators(first, last);
	}

	template<class T>
	template <class iter_type>
	inline void vector<T>::initialize_from_iterators(iter_type first, iter_type last)
	{
		increase_storage(::blocxx::distance(first, last), E_ABSOLUTE_RESIZE);
#if defined(BLOCXX_EXCEPTION_SAFETY)
		try
#endif
		{
			for( end_pointer = data_pointer; first != last; ++first, ++end_pointer )
			{
				new(end_pointer) T(*first);
			}
		}
#if defined(BLOCXX_EXCEPTION_SAFETY)
		catch(...)
		{
			// Clean up any pre-constructed items.
			remove_storage();

			// Rethrow the exception.
			throw;
		}
#endif
	}

	//----------------------------------
	// Destructor for class vector
	//----------------------------------
	template<class T>
	inline vector<T>::~vector()
	{
		remove_storage();
	} // ~vector()

	//----------------------------------------
	// Copy constructor for class vector
	//----------------------------------------
	template<class T>
	inline vector<T>::vector(const vector<T>& old):
		data_pointer(NULL),
		end_pointer(NULL),
		storage_end(NULL)
	{
		if( old.data_pointer )
		{
			size_t desired_size = next_larger_size(old.size());
			increase_storage(desired_size);

#if defined(BLOCXX_EXCEPTION_SAFETY)
			try
#endif
			{
				T const* old_ptr = old.data_pointer;
				T const* old_end = old.end_pointer;
				for( end_pointer = data_pointer; old_ptr != old_end; ++end_pointer, ++old_ptr)
				{
					new(end_pointer) T(*old_ptr);
				}
			}
#if defined(BLOCXX_EXCEPTION_SAFETY)
			catch(...)
			{
				// Cleanup any already constructed items...
				remove_storage();

				// Rethrow the exception.
				throw;
			}
#endif
		}
	} // vector(vector)

	template<class T>
	inline size_t vector<T>::next_larger_size(size_t s) const
	{
		// Ccalculating the power of two that is larger or equal to the size needed
		// to store the data isn't just for quick resizing.  It turns out that the
		// raw memory allocation for an even power of two is a few percent faster
		// on some platforms (Linux/MacOS) than the time for a lesser chunk of
		// memory that is a sum of even powers of two.
		size_t next_power = 1;
		while(next_power < s)
		{
			next_power <<= 1;
		}
		return next_power;
	}

	//-------------------------------------------
	// Assignment operator for class vector
	//-------------------------------------------
	template<class T>
	inline vector<T>& vector<T>::operator= (const vector<T>& old)
	{
		// Generic check for self-assignment
		if( &old != this)
		{
			// Remove the current data (leave the storage alone).
			destroy_data();

			size_t desired_size = next_larger_size(old.size());

			// Increase the storage (if required) to hold all of the data to be
			// copied.
			increase_storage(desired_size);

			// NOTE: This assumes that the data sizes are valid when the pointers
			// are non-null, and that when the pointers are NULL, the data size is
			// 0.  This should always be the case.
			//
			// Also, no exception block is required around this, since all memory
			// allocated can be freed from the destructor, and this loop will leave
			// the data in good state (up until the final item that threw), so
			// data_size worth of elements can have the destructor called, should any
			// exception be thrown from a copy constructor..
			T const* old_ptr = old.data_pointer;
			T const* old_end = old.end_pointer;
			for( end_pointer = data_pointer; old_ptr != old_end; ++end_pointer, ++old_ptr)
			{
				new(end_pointer) T(*old_ptr);
			}
		}
		return (*this);
	} // vector::operator=(vector)

	template <class T>
	inline void vector<T>::increase_storage(size_t desired, vector<T>::resize_type size_method)
	{
		// NOTE: Size cannot decrease, so the only other case is equal, in which we
		// don't need any more memory.
		size_t current_size = storage_end - data_pointer;
		if( desired > current_size )
		{
			size_t new_size = desired;
			if( size_method == E_NORMAL_RESIZE )
			{
				new_size = current_size << 1;

				if( new_size < desired )
				{
					new_size = desired;
				}
			}
			T* new_ptr = reinterpret_cast<T*>(new char[sizeof(T) * new_size]);
			T* new_end = new_ptr;

			// NOTE: this exception block is very important if containing objects
			// can allocate memory, or other resources.  Otherwise, there will be a
			// memory leak!
#if defined(BLOCXX_EXCEPTION_SAFETY)
			try
#endif
			{
				for( T* old_iter = data_pointer; old_iter != end_pointer; ++old_iter, ++new_end)
				{
					new(new_end) T(*old_iter);
				}
			}
#if defined(BLOCXX_EXCEPTION_SAFETY)
			catch(...)
			{
				// Note that this only cleans up local storage.  The class storage will
				// be freed when the destructor is called.
				for(T* new_iter = new_ptr; new_iter != new_end; ++new_iter)
				{
					new_iter->~T();
				}

				delete[] reinterpret_cast<char*>(new_ptr);

				throw;
			}
#endif
			remove_storage();
			data_pointer = new_ptr;
			end_pointer = new_end;
			storage_end = data_pointer + new_size;
		}
	}

	template <class T>
	inline void vector<T>::append(const T& val)
	{
		// Call the increase_storage function if there's not enough space.
		if( end_pointer >= storage_end )
		{
			size_t new_size = size() + 1;

			increase_storage(new_size, E_NORMAL_RESIZE);
		}
		new(end_pointer++) T(val);
	}

	template <class T>
	inline void vector<T>::append(const vector<T>& vec)
	{
		// Call the increase_storage function if there's not enough space.
		T* new_end = end_pointer + vec.size();
		if( new_end >= storage_end )
		{
			size_t new_size = size() + vec.size();
			increase_storage(new_size, E_NORMAL_RESIZE);
		}

		const T* copy_end = vec.end_pointer;

		// If the copy constructors here throw anything, we're safe, as the
		// end_pointer shows one beyond the last successfully copied element.  The
		// destructor should properly handle things.
		for( const T* copy_iter = vec.data_pointer; copy_iter != copy_end; ++copy_iter, ++end_pointer)
		{
			new(end_pointer) T(*copy_iter);
		}
	}

	template<class T>
	inline void vector<T>::push_back(const T& val)
	{
		append(val);
	}

	template<class T>
	inline void vector<T>::push_back(const vector<T>& vec)
	{
		append(vec);
	}

	template <class T>
	typename vector<T>::iterator vector<T>::erase(typename vector<T>::iterator pos)
	{
		if( pos != end() )
		{
			return this->erase(pos, pos + 1);
		}
		return end();
	}

	template <class T>
	typename vector<T>::iterator vector<T>::erase(typename vector<T>::iterator first, typename vector<T>::iterator last)
	{
		T* front = &*first;
		T* back = &*last;

		if( back > front )
		{
			// Copy data down...
			while( back != end_pointer )
			{
				// Although almost everything else about the vector is using a
				// copy constructor, I can't come up with an exception safe method of
				// doing so (there would be the chance of an exception being thrown in
				// the copy constructor while one element in the middle of the vector
				// has already been destroyed and not yet reinitialized).  We'll need
				// to resort to the assignment operators for safety.
				*front = *back;
				++front;
				++back;
			}
			// Clean up the extra data.
			while( end_pointer != front )
			{
				--end_pointer;
				end_pointer->~T();
			}
		}
		// Return the element that appears *after* the ones that were deleted.  This now happens to be 'first'
		return first;
	}

	template <class T>
	void vector<T>::clear()
	{
		if( !empty() )
		{
			while( end_pointer != data_pointer )
			{
				--end_pointer;
				end_pointer->~T();
			}
		}
	}

	template <class T>
	bool operator==(const vector<T>& vec1, const vector<T>& vec2)
	{
		if( vec1.size() != vec2.size() )
		{
			return false;
		}
		// For now, it seems as though the subscript access is faster than iterator access.
		for( size_t i = 0; i < vec1.size(); ++i )
		{
			if( vec1[i] != vec2[i] )
			{
				return false;
			}
		}
		return true;
	}

	template <class T>
	bool operator!=(const vector<T>& vec1, const vector<T>& vec2)
	{
		return !(vec1 == vec2);
	}

	/**
	 * The purpose of this header is to only have one definition of this function.  Various classes use
	 * COWReference<std::vector<T> > and thus need this function.
	 */
	template <class T>
	inline ::blocxx::vector<T>* COWReferenceClone(::blocxx::vector<T>* obj)
	{
		return new ::blocxx::vector<T>(*obj);
	}

} // namespace BLOCXX_NAMESPACE

#endif /* !defined(BLOCXX__vector_HPP) */
