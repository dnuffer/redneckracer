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

// This header file contains the implementation of the Array template class.
// It is not intended to be used directly by users of the blocxx library.


/**
 * @author Jon Carey
 * @author Dan Nuffer
 *
 * This header file contains the implementation of the Array template class.
 * It is not intended to be used directly by users of the blocxx library.
 */

#ifndef BLOCXX_ARRAY_IMPL_HPP_INCLUDE_GUARD_
#define BLOCXX_ARRAY_IMPL_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

namespace BLOCXX_NAMESPACE
{

/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array()
: m_impl(new V)
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::~Array()
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array(V* toWrap)
: m_impl(toWrap)
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array(size_type n, const T& value)
: m_impl(new V(n, value))
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array(int n, const T& value)
: m_impl(new V(n, value))
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array(long n, const T& value)
: m_impl(new V(n, value))
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>::Array(size_type n)
: m_impl(new V(n))
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
template<class InputIterator>
inline Array<T>::Array(InputIterator first, InputIterator last)
: m_impl(new V(first, last))
{
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::begin()
{
	return m_impl->begin();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_iterator
Array<T>::begin() const
{
	return m_impl->begin();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::end()
{
	return m_impl->end();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_iterator
Array<T>::end() const
{
	return m_impl->end();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reverse_iterator
Array<T>::rbegin()
{
	return m_impl->rbegin();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reverse_iterator
Array<T>::rbegin() const
{
	return m_impl->rbegin();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reverse_iterator
Array<T>::rend()
{
	return m_impl->rend();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reverse_iterator
Array<T>::rend() const
{
	return m_impl->rend();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::size_type
Array<T>::size() const
{
	return m_impl->size();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::size_type
Array<T>::max_size() const
{
	return m_impl->max_size();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::size_type
Array<T>::capacity() const
{
	return m_impl->capacity();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline bool
Array<T>::empty() const
{
	return m_impl->empty();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reference
Array<T>::operator[](size_type n)
{
#ifdef BLOCXX_CHECK_ARRAY_INDEXING
	checkValidIndex(n);
#endif
	return m_impl->operator[](n);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reference
Array<T>::operator[](size_type n) const
{
#ifdef BLOCXX_CHECK_ARRAY_INDEXING
	checkValidIndex(n);
#endif
	return m_impl->operator[](n);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reference
Array<T>::at(size_type pos)
{
	return m_impl->at(pos);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reference
Array<T>::at(size_type pos) const
{
	return m_impl->at(pos);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline Array<T>&
Array<T>::operator+= (const T& x)
{
	m_impl->push_back(x);
	return *this;
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::reserve(size_type n)
{
	m_impl->reserve(n);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reference
Array<T>::front()
{
	return m_impl->front();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reference
Array<T>::front() const
{
	return m_impl->front();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::reference
Array<T>::back()
{
	return m_impl->back();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_reference
Array<T>::back() const
{
	return m_impl->back();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::push_back(const T& x)
{
	m_impl->push_back(x);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::append(const T& x)
{
	push_back(x);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::swap(Array<T>& x)
{
	m_impl.swap(x.m_impl);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::insert(iterator position, const T& x)
{
	return m_impl->insert(position, x);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::insert(size_type position, const T& x)
{
	m_impl->insert(m_impl->begin() + position, x);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::remove(size_type index)
{
#ifdef BLOCXX_CHECK_ARRAY_INDEXING
	checkValidIndex(index);
#endif
	m_impl->erase(m_impl->begin() + index);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::remove(size_type begin, size_type end)
{
#ifdef BLOCXX_CHECK_ARRAY_INDEXING
	checkValidIndex(begin);
	checkValidIndex(end - 1);
#endif
	m_impl->erase(m_impl->begin() + begin, m_impl->begin() + end);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
template<class InputIterator>
inline void
Array<T>::insert(iterator position, InputIterator first, InputIterator last)
{
	m_impl->insert(position, first, last);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::appendArray(const Array<T>& x)
{
	insert(end(), x.begin(), x.end());
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::pop_back()
{
	m_impl->pop_back();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::erase(iterator position)
{
	return m_impl->erase(position);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::erase(iterator first, iterator last)
{
	return m_impl->erase(first, last);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::resize(size_type new_size, const T& x)
{
	m_impl->resize(new_size, x);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::resize(size_type new_size)
{
	m_impl->resize(new_size);
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::clear()
{
	m_impl->clear();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_iterator
Array<T>::find(const T &x, const_iterator first, const_iterator last) const
{
	for( ; first != end(); ++first)
	{
		if( x == *first)
			return first;
		if(first == last)
			break;
	}
	return end();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::const_iterator
Array<T>::find(const T &x) const
{
	return find(x, begin(), end());
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::find(const T &x, iterator first, iterator last)
{
	for( ; first != end(); ++first)
	{
		if( x == *first)
			return first;
		if(first == last)
			break;
	}
	return end();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline typename Array<T>::iterator
Array<T>::find(const T &x)
{
	return find(x, begin(), end());
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline bool
Array<T>::contains(const T& x, const_iterator first, const_iterator last) const
{
	return find(x, first, last) != end();
}
/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline bool
Array<T>::contains(const T& x) const
{
	return find(x, begin(), end()) != end();
}

#ifdef BLOCXX_CHECK_ARRAY_INDEXING
/////////////////////////////////////////////////////////////////////////////
BLOCXX_COMMON_API void throwArrayOutOfBoundsException(size_t size, size_t idx);

/////////////////////////////////////////////////////////////////////////////
template <typename T>
inline void
Array<T>::checkValidIndex(size_type index) const
{
	if (index >= size())
	{
		throwArrayOutOfBoundsException(size(), index);
	}
}
#endif
template<class T>
inline bool operator==(const Array<T>& x, const Array<T>& y)
{
	return *x.m_impl == *y.m_impl;
}
template<class T>
inline bool operator<(const Array<T>& x, const Array<T>& y)
{
	return *x.m_impl < *y.m_impl;
}
template<class T>
inline void swap(Array<T>& x, Array<T>& y)
{
	x.swap(y);
}

} // end namespace BLOCXX_NAMESPACE

#endif

