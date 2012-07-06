/*******************************************************************************
* Copyright (C) 2005, Quest Software, Inc. All rights reserved.
* Copyright (C) 2006, Novell, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of
*       Quest Software, Inc.,
*       nor Novell, Inc.,
*       nor the names of its contributors or employees may be used to
*       endorse or promote products derived from this software without
*       specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/



/**
 * @author Jon Carey
 * @author Dan Nuffer
 */

#ifndef BLOCXX_ARRAY_HPP_INCLUDE_GUARD_
#define BLOCXX_ARRAY_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/ArrayFwd.hpp"
#include "miniblocxx/COWReference.hpp"
#include "miniblocxx/Types.hpp"
#include "miniblocxx/Exception.hpp"
#include "miniblocxx/vector.hpp"

namespace BLOCXX_NAMESPACE
{

// Declare the OutOfBoundsException
BLOCXX_DECLARE_APIEXCEPTION(OutOfBounds, BLOCXX_COMMON_API);

/**
 * Array<> wraps std::vector<> in COWReference<> adding ref counting and copy
 * on write capability.  It also adds valid range checks to operator[] if
 * BLOCXX_CHECK_ARRAY_INDEXING is defined.
 *
 * Invariants: See std::vector<>
 * Thread safety: read
 * Copy semantics: Copy On Write
 * Exception safety: same as std::vector<T>
 */
template<class T> class Array
{
	typedef std::vector<T> V;

#ifdef BLOCXX_WIN32
#pragma warning (push)
#pragma warning (disable: 4251)
#endif

	COWReference<V> m_impl;

#ifdef BLOCXX_WIN32
#pragma warning (pop)
#endif

public:
	typedef typename V::value_type value_type;
	typedef typename V::iterator iterator;
	typedef typename V::const_iterator const_iterator;
	typedef typename V::reference reference;
	typedef typename V::const_reference const_reference;
	typedef typename V::size_type size_type;
	typedef typename V::difference_type difference_type;
	typedef typename V::reverse_iterator reverse_iterator;
	typedef typename V::const_reverse_iterator const_reverse_iterator;

	/**
	 * Default Constructor
	 */
	Array();
	/**
	 * Destructor
	 */
	~Array();
	/**
	 * Constructor
	 * @param toWrap The std::vector to wrap with this Array object.
	 */
	explicit Array(V* toWrap);
	/**
	 * Construct an Array that consist of a specified number of elements
	 * that are copies of a given object.
	 * @param n Number of elements the Array will contain.
	 * @param value The value every element of the array will be
	 *		initialized to.
	 */
	Array(size_type n, const T& value);
	/**
	 * Construct an Array that consist of a specified number of elements
	 * that are copies of a given object.
	 * @param n Number of elements the Array will contain.
	 * @param value The value every element of the array will be
	 *		initialized to.
	 */
	Array(int n, const T& value);
	/**
	 * Construct an Array that consist of a specified number of elements
	 * that are copies of a given object.
	 * @param n Number of elements the Array will contain.
	 * @param value The value every element of the array will be
	 *		initialized to.
	 */
	Array(long n, const T& value);
	/**
	 * Construct an Array that consist of a specified number of elements
	 * that have be constructed using the default constructor of
	 * class T.
	 * @param n Number of elements the Array will contain.
	 */
	explicit Array(size_type n);
	/**
	 * Construct an Array from a range specified with InputIterators.
	 * @param first The iterator for the beginning of the range.
	 * @param last The iterator for the end of the range.
	 */
	template<class InputIterator>
	Array(InputIterator first, InputIterator last);
	/**
	 * @return A read/write iterator that  points to the first element in the
	 *		Array. Iteration is done in the normal order (1st to last) with
	 *		the returned iterator.
	 */
	iterator begin();
	/**
	 * @return A read only iterator that  points to the first element in the
	 *		Array. Iteration is done in the normal order (1st to last) with
	 *		the returned iterator.
	 */
	const_iterator begin() const;
	/**
	 * @return A read/write iterator that points one past the last element in
	 *		the Array. Iteration is done in the normal order (1st to last)
	 *		with the returned iterator.
	 */
	iterator end();
	/**
	 * @return A read only iterator that points one past the last element in
	 *		the Array. Iteration is done in the normal order (1st to last)
	 *		with the returned iterator.
	 */
	const_iterator end() const;
	/**
	 * @return A read/write reverse iterator that points to the last element
	 * 		in the Array. Iteration is done in reverse order with the
	 *		returned iterator.
	 */
	reverse_iterator rbegin();
	/**
	 * @return A read only reverse iterator that points to the last element
	 * 		in the Array. Iteration is done in reverse order with the
	 *		returned iterator.
	 */
	const_reverse_iterator rbegin() const;
	/**
	 * @return A read/write reverse iterator that points one before the first
	 *		element of the Array. Iteration is done in reverse order with the
	 *		returned iterator.
	 */
	reverse_iterator rend();
	/**
	 * @return A read only reverse iterator that points one before the first
	 *		element of the Array. Iteration is done in reverse order with the
	 *		returned iterator.
	 */
	const_reverse_iterator rend() const;
	/**
	 * @return The number of elements in the Array
	 */
	size_type size() const;
	/**
	 * @return The maximum number of elements the Array can hold.
	 */
	size_type max_size() const;
	/**
	 * @return The space that is currently allocated to hold the elements.
	 *		This value will be >= size()
	 */
	size_type capacity() const;
	/**
	 * @return true if the Array is empty (contains zero elements)
	 */
	bool empty() const;
	/**
	 * Retrieve A read/write reference to an object in the Array at a given
	 *		index.
	 * @param n The index of the element to retrieve the reference for.
	 * @return A read/write reference to the object at the given index.
	 */
	reference operator[](size_type n);
	/**
	 * Retrieve A read only reference to an object in the Array at a given
	 *		index
	 * @param n The index of the element to retrieve the reference for.
	 * @return A read only reference to the object at the given index.
	 */
	const_reference operator[](size_type n) const;

	reference at(size_type pos);
	const_reference at(size_type pos) const;

	/**
	 * Append an object to the end of the Array.
	 * @param x The object to append to the end of the Array.
	 * @return A reference to this Array object.
	 */
	Array<T>& operator+= (const T& x);
	/**
	 * Ensure the capacity is at least the size of a given
	 * value. If the given value is > max_size() the method
	 * with throw an std::length_error exception.
	 * @param n - The minimum size this Array can be.
	 */
	void reserve(size_type n);
	/**
	 * @return A read/write reference to the first element of the Array.
	 */
	reference front();
	/**
	 * @return A read only reference to the first element of the Array.
	 */
	const_reference front() const;
	/**
	 * @return A read/write reference to the last element of the Array.
	 */
	reference back();
	/**
	 * @return A read only reference to the last element of the Array.
	 */
	const_reference back() const;
	/**
	 * Append an element to the end of the Array.
	 * @param x The element to append to the end of the Array.
	 */
	void push_back(const T& x);
	/**
	 * Append an element to the end of the Array. This is identical
	 * to push_back.
	 * @param x The element to append to the end of the Array.
	 */
	void append(const T& x);
	/**
	 * Swap the elements of this Array with the elements of another.
	 * @param x The Array object this Array will swap elements with.
	 */
	void swap(Array<T>& x);
	/**
	 * Insert an element in the Array before an element specified
	 * by an iterator.
	 * @param position An iterator that points to the insertion point.
	 *		The element will be inserted before this point.
	 * @param x The element to insert into the Array.
	 * @return An iterator that points to the newly inserted element
	 *		in the Array.
	 */
	iterator insert(iterator position, const T& x);
	/**
	 * Insert an element in the Array before an element specified
	 * by an index.
	 * @param position An index that specifies where the element will be
	 *		inserted.
	 * @param x The element to insert into the Array.
	 */
	void insert(size_type position, const T& x);
	/**
	 * Remove an element from the Array at a given index.
	 */
	void remove(size_type index);
	/**
	 * Remove an element specified by a range.
	 * @param begin An iterator that specifies the beginning of the range
	 *		to remove.
	 * @param end An iterator that specifies the end of the range to remove.
	 */
	void remove(size_type begin, size_type end);
	/**
	 * Insert a range of elements before a given position in the Array.
	 * @param position The position to insert the elements at. The insertion
	 *		will be done before this position.
	 * @param first The beginning of the range of elements to insert.
	 * @param last The end of the range of elements to insert.
	 */
	template<class InputIterator>
	void insert(iterator position, InputIterator first, InputIterator last);
	/**
	 * Append the elements of another Array to the end of this Array.
	 * @param x The Array the elements will be copied from.
	 */
	void appendArray(const Array<T>& x);
	/**
	 * Remove the last element of the Array.
	 */
	void pop_back();
	/**
	 * Remove an element of the Array specified with an iterator.
	 * @param position An iterator that points to the element to be removed.
	 * @return An iterator that points to the element in the Array following
	 *		the removed element.
	 */
	iterator erase(iterator position);
	/**
	 * Remove elements of the Array specified by a beginning and ending
	 * iterator.
	 * @param first An iterator that specifies the first element to remove.
	 * @param last An iterator that specifies the last element to remove.
	 * @return An iterator that points to the element in the Array following
	 *		the removed elements.
	 */
	iterator erase(iterator first, iterator last);
	/**
	 * Ensure the Array is a given size.
	 * @param new_size The new size of the Array.
	 * @param x An object to append to the end of the Array if it needs
	 *		to be larger than what it already is.
	 */
	void resize(size_type new_size, const T& x);
	/**
	 * Ensure the Array is a given size. If the Array needs to be larger that
	 * what it already is, this method will append items that have been
	 * initialized using the default constructor of class T.
	 * @param new_size The new size of the Array.
	 */
	void resize(size_type new_size);
	/**
	 * Remove all items from the Array. The size() of the Array should be
	 * zero after calling this method.
	 */
	void clear();
	/**
	 * Find element x in the array range specified by the first and last
	 * iterators.
	 * @param  x     The element to seach for.
	 * @param  first The first position iterator; begin of the range.
	 * @param  last  The last position iterator; end of the range.
	 * @return An read only iterator pointing to the found element or
	 * the end iterator pointing one past the last element in the array.
	 */
	const_iterator find(const T &x, const_iterator first,
	                                const_iterator last) const;
	/**
	 * Find element x in the array.
	 * @param  x    The element to seach for.
	 * @return An read only iterator pointing to the found element or
	 * the end iterator pointing one past the last element in the array.
	 */
	const_iterator find(const T &x) const;
	/**
	 * Find element x in the array range specified by the first and last
	 * iterators.
	 * @param  x     The element to seach for.
	 * @param  first The first position iterator; begin of the range.
	 * @param  last  The last position iterator; end of the range.
	 * @return An read/write iterator pointing to the found element or
	 * the end iterator pointing one past the last element in the array.
	 */
	iterator       find(const T &x, iterator first, iterator last);
	/**
	 * Find element x in the array.
	 * @param  x    The element to seach for.
	 * @return An read/write iterator pointing to the found element or
	 * the end iterator pointing one past the last element in the array.
	 */
	iterator       find(const T &x);
	/**
	 * Determine if element x is contained in the array range specified
	 * by the first and last iterators.
	 * @param  x     The element to seach for.
	 * @param  first The first position iterator; begin of the range.
	 * @param  last  The last position iterator; end of the range.
	 * @return true if the element x is contained in the specified
	 *         range of the array.
	 */
	bool contains(const T& x, const_iterator first,
	                          const_iterator last) const;
	/**
	 * Determine if element x is contained in the array.
	 * @param  x    The element to seach for.
	 * @return true if the element x is contained in the array.
	 */
	bool contains(const T& x) const;

	/**
	 * Determine equality of two Arrays. Two Arrays are considered equal if
	 * they are of the same size and elements in the same position are equal
	 * as defined by the "==" operator of the type in the Array.
	 * @param x The first Array object in the comparison.
	 * @param y The second Array object in the comparison.
	 * @return true if Array x is equal to Array y.
	 */
	friend bool operator== <>(const Array<T>& x, const Array<T>& y);

	/**
	 * Determine if one Array is less than another. This is done by
	 * doing a lexicographical compare on the Arrays. a lexicographical
	 * compare will compares the Arrays element by element until:
	 *		- It finds two corresponding elements unequal, and the result of
	 *		  their comparison is taken as the result of the comparison
	 *		  between sequences.
	 *		- No inequalities are found, but one sequence has more elements
	 *		  than the other, and the shorter sequence is considered less
	 *		  than the longer sequence.
	 *		- No inequalities are found and the sequences have the same
	 *		  number of elements, and so the sequences are equal and the
	 *		  result of the comparison is false.
	 * @param x The first Array object in the comparison.
	 * @param y The second Array object in the comparison.
	 * @return true if Array x is less than Array y.
	 */
	friend bool operator< <>(const Array<T>& x, const Array<T>& y);
private:
#ifdef BLOCXX_CHECK_ARRAY_INDEXING
	void checkValidIndex(size_type index) const;
#endif

	/** \example arrayDemo.cpp */
};

/**
 * Determine two Arrays are not equal. Two Arrays are considered equal if
 * they are of the same size and elements in the same position are equal
 * as defined by the "==" operator of the type in the Array.
 * @param x The first Array object in the comparison.
 * @param y The second Array object in the comparison.
 * @return true if Array x is not equal to Array y.
 */
template <class T>
inline bool operator != (const Array<T>& x, const Array<T>& y)
{
	return !(x == y);
}

/**
 * Determine if one Array is less than or equal to another. This is done by
 * doing a lexicographical compare on the Arrays. a lexicographical compare
 * will compares the Arrays element by element until:
 *		- It finds two corresponding elements unequal, and the result of
 *		  their comparison is taken as the result of the comparison
 *		  between sequences.
 *		- No inequalities are found, but one sequence has more elements
 *		  than the other, and the shorter sequence is considered less
 *		  than the longer sequence.
 *		- No inequalities are found and the sequences have the same
 *		  number of elements, and so the sequences are equal and the
 *		  result of the comparison is false.
 * @param x The first Array object in the comparison.
 * @param y The second Array object in the comparison.
 * @return true if Array x is less than or equal to Array y.
 */
template <class T>
inline bool operator <= (const Array<T>& x, const Array<T>& y)
{
	return !(y < x);
}

/**
 * Determine if one Array is greater than or equal to another. This is done by
 * doing a lexicographical compare on the Arrays. a lexicographical compare
 * will compares the Arrays element by element until:
 *		- It finds two corresponding elements unequal, and the result of
 *		  their comparison is taken as the result of the comparison
 *		  between sequences.
 *		- No inequalities are found, but one sequence has more elements
 *		  than the other, and the shorter sequence is considered less
 *		  than the longer sequence.
 *		- No inequalities are found and the sequences have the same
 *		  number of elements, and so the sequences are equal and the
 *		  result of the comparison is false.
 * @param x The first Array object in the comparison.
 * @param y The second Array object in the comparison.
 * @return true if Array x is greater than or equal to Array y.
 */
template <class T>
inline bool operator >= (const Array<T>& x, const Array<T>& y)
{
	return !(x < y);
}

/**
 * Determine if one Array is greater than another. This is done by
 * doing a lexicographical compare on the Arrays. a lexicographical compare
 * will compares the Arrays element by element until:
 *		- It finds two corresponding elements unequal, and the result of
 *		  their comparison is taken as the result of the comparison
 *		  between sequences.
 *		- No inequalities are found, but one sequence has more elements
 *		  than the other, and the shorter sequence is considered less
 *		  than the longer sequence.
 *		- No inequalities are found and the sequences have the same
 *		  number of elements, and so the sequences are equal and the
 *		  result of the comparison is false.
 * @param x The first Array object in the comparison.
 * @param y The second Array object in the comparison.
 * @return true if Array x is greater than Array y.
 */
template <class T>
inline bool operator > (const Array<T>& x, const Array<T>& y)
{
	return y < x;
}

typedef Array<UInt8>      UInt8Array;
typedef Array<Int8>       Int8Array;
typedef Array<UInt16>     UInt16Array;
typedef Array<Int16>      Int16Array;
typedef Array<UInt32>     UInt32Array;
typedef Array<Int32>      Int32Array;
typedef Array<UInt64>     UInt64Array;
typedef Array<Int64>      Int64Array;
typedef Array<Real64>     Real64Array;
typedef Array<Real32>     Real32Array;

} // end namespace BLOCXX_NAMESPACE

#include "miniblocxx/ArrayImpl.hpp"

#endif

