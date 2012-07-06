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
 * @author Dan Nuffer
 */

#ifndef BLOCXX_SORTED_VECTOR_MAP_HPP_INCLUDE_GUARD_
#define BLOCXX_SORTED_VECTOR_MAP_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/COWReference.hpp"
#include "miniblocxx/vector.hpp"
#include "miniblocxx/CommonFwd.hpp"
#include <utility> // for std::pair
#include <functional> // for std::less
#include <algorithm>

namespace BLOCXX_NAMESPACE
{

template <class Key, class T, class Compare>
class SortedVectorMapDataCompare
{
	typedef std::pair<Key, T> Data;
public:
	bool operator()(const Data& lhs, const Data& rhs) const
	{
		return keyLess(lhs.first, rhs.first);
	}

	bool operator()(const Data& lhs, const typename Data::first_type& k) const
	{
		return keyLess(lhs.first, k);
	}

	bool operator()(const typename Data::first_type& k, const Data& rhs) const
	{
		return keyLess(k, rhs.first);
	}
	bool operator()(const typename Data::first_type& k, const typename Data::first_type& rhs) const
	{
		return keyLess(k, rhs);
	}
private:
	bool keyLess(const typename Data::first_type& k1,
		const typename Data::first_type& k2) const
	{
		return Compare()(k1, k2);
	}
};


// forward declarations are necessary for template friends.
template<class Key, class T, class Compare>
inline bool operator==(const SortedVectorMap<Key, T, Compare>& x,
	const SortedVectorMap<Key, T, Compare>& y);

template<class Key, class T, class Compare>
inline bool operator<(const SortedVectorMap<Key, T, Compare>& x,
	const SortedVectorMap<Key, T, Compare>& y);

template <class Key, class T, class Compare>
inline void swap(SortedVectorMap<Key, T, Compare>& x,
	SortedVectorMap<Key, T, Compare>& y);

template<class Key, class T, class Compare>
class SortedVectorMap
{
	typedef std::pair<Key, T> Data;
	typedef std::vector<Data> container_t;
	COWReference<container_t> m_impl;
public:
	typedef          Key key_type;
	typedef          T data_type;
	typedef          std::pair<const key_type, data_type> value_type;
	typedef          Compare key_compare;
	typedef          Compare value_compare;
	typedef typename container_t::pointer pointer;
	typedef typename container_t::reference reference;
	typedef typename container_t::const_reference const_reference;
	typedef typename container_t::iterator iterator;
	typedef typename container_t::const_iterator const_iterator;
	typedef typename container_t::reverse_iterator reverse_iterator;
	typedef typename container_t::const_reverse_iterator const_reverse_iterator;
	typedef typename container_t::size_type size_type;
	typedef typename container_t::difference_type difference_type;
	SortedVectorMap() : m_impl(new container_t) {  }
	explicit SortedVectorMap(container_t* toWrap) : m_impl(toWrap)
	{
		std::sort(m_impl->begin(), m_impl->end(), Compare());
		m_impl->erase(std::unique(m_impl->begin(), m_impl->end(), &equivalent), m_impl->end());
	}
	template <class InputIterator>
	SortedVectorMap(InputIterator first, InputIterator last) :
		m_impl(new container_t(first, last))
	{
		std::sort(m_impl->begin(), m_impl->end(), Compare());
		m_impl->erase(std::unique(m_impl->begin(), m_impl->end(), &equivalent), m_impl->end());
	}
	const_iterator begin() const
	{
		return m_impl->begin();
	}
	const_iterator end() const
	{
		return m_impl->end();
	}
	// These are slightly dangerous, if you use them, DON'T CHANGE THE KEY!
	iterator begin()
	{
		return m_impl->begin();
	}
	iterator end()
	{
		return m_impl->end();
	}
	const_reverse_iterator rbegin() const
	{
		return m_impl->rbegin();
	}
	const_reverse_iterator rend() const
	{
		return m_impl->rend();
	}
	bool empty() const
	{
		return m_impl->empty();
	}
	size_type size() const
	{
		return m_impl->size();
	}
	size_type max_size() const
	{
		return m_impl->max_size();
	}
	data_type& operator[](const key_type& k)
	{
		iterator i = std::lower_bound(m_impl->begin(), m_impl->end(), k, Compare());
		if (i != m_impl->end() && equivalent(i->first, k))
		{
			return i->second;
		}
		return (*(m_impl->insert(i, value_type(k, data_type())))).second;
	}
	void swap(SortedVectorMap<Key, T, Compare>& x)
	{
		m_impl.swap(x.m_impl);
	}
	std::pair<iterator, bool> insert(const value_type& x)
	{
		iterator i = std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());
		if (i != m_impl->end() && equivalent(i->first, x.first))
		{
			return std::pair<iterator, bool>(i, false);
		}
		else
		{
			return std::pair<iterator, bool>(m_impl->insert(i, x), true);
		}
	}
	iterator insert(iterator, const value_type& x)
	{
		iterator i = std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());

		return m_impl->insert(i, x);
	}
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last)
	{
		m_impl->insert(m_impl->end(), first, last);
		std::sort(m_impl->begin(), m_impl->end(), Compare());
		m_impl->erase(std::unique(m_impl->begin(), m_impl->end(), &equivalent), m_impl->end());
	}
	iterator erase(iterator position)
	{
		return m_impl->erase(position);
	}
	size_type erase(const key_type& x)
	{
		iterator i = std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());
		if (i != m_impl->end() && equivalent(i->first, x))
		{
			m_impl->erase(i);
			return 1;
		}
		else
		{
			return 0;
		}
	}
	iterator erase(iterator first, iterator last)
	{
		return m_impl->erase(first, last);
	}
	void clear()
	{
		m_impl->clear();
	}
	const_iterator find(const key_type& x) const
	{
		const_iterator pos = std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());
		if (pos != m_impl->end() && equivalent(pos->first, x))
		{
			return pos;
		}
		else
		{
			return m_impl->end();
		}
	}
	iterator find(const key_type& x)
	{
		iterator pos = std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());
		if (pos != m_impl->end() && equivalent(pos->first, x))
		{
			return pos;
		}
		else
		{
			return m_impl->end();
		}
	}
	size_type count(const key_type& x) const
	{
		if (std::binary_search(m_impl->begin(), m_impl->end(), x, Compare()))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	const_iterator lower_bound(const key_type& x) const
	{
		return std::lower_bound(m_impl->begin(), m_impl->end(), x, Compare());
	}
	const_iterator upper_bound(const key_type& x) const
	{
		return std::upper_bound(m_impl->begin(), m_impl->end(), x, Compare());
	}
	std::pair<const_iterator, const_iterator>
		equal_range(const key_type& x) const
	{
		return std::equal_range(m_impl->begin(), m_impl->end(), x, Compare());
	}
	friend bool operator== <>(const SortedVectorMap<Key, T, Compare>& x,
		const SortedVectorMap<Key, T, Compare>& y);
	friend bool operator< <>(const SortedVectorMap<Key, T, Compare>& x,
		const SortedVectorMap<Key, T, Compare>& y);
private:
	static bool equivalent(const key_type& x, const key_type& y)
	{
		// Strict weak ordering: Two objects x and y are equivalent if both f(x, y) and f(y, x) are false.
		return (!Compare()(x, y) && !Compare()(y, x));
	}
};
template<class Key, class T, class Compare>
inline bool operator==(const SortedVectorMap<Key, T, Compare>& x,
	const SortedVectorMap<Key, T, Compare>& y)
{
	return *x.m_impl == *y.m_impl;
}
template<class Key, class T, class Compare>
inline bool operator<(const SortedVectorMap<Key, T, Compare>& x,
	const SortedVectorMap<Key, T, Compare>& y)
{
	return *x.m_impl < *y.m_impl;
}
template <class Key, class T, class Compare>
inline void swap(SortedVectorMap<Key, T, Compare>& x,
	SortedVectorMap<Key, T, Compare>& y)
{
	x.swap(y);
}

} // end namespace BLOCXX_NAMESPACE

#endif
