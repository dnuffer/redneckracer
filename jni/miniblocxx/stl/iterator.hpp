#if       !defined(BLOCXX__iterator_HPP)
#define            BLOCXX__iterator_HPP

#include <stdint.h> // to get the typedef for ssize_t

namespace BLOCXX_NAMESPACE
{
	template <class T> class const_random_access_iterator;
	template <class T> class random_access_iterator;
	/**
	 * A simple random_access iterator.
	 */
	template <class T>
	class random_access_iterator
	{
	public:
		typedef T value_type;

		inline random_access_iterator(): ptr(NULL)
		{
		}
		inline explicit random_access_iterator(T* p): ptr(p)
		{
		}
		inline random_access_iterator(const random_access_iterator<T>& old): ptr(old.ptr)
		{
		}
		inline ~random_access_iterator()
		{
		}
		inline random_access_iterator& operator=(const random_access_iterator<T>& old)
		{
			ptr = old.ptr;
			return *this;
		}

		inline T* operator->() const { return ptr; }
		inline T& operator*() const { return *ptr; }
		inline random_access_iterator operator++()
		{
			++ptr;
			return *this;
		}
		inline random_access_iterator operator++(int)
		{
			random_access_iterator temp(*this);
			++ptr;
			return temp;
		}
		inline bool operator!=(const random_access_iterator& it) const
		{
			return ptr != it.ptr;
		}
		inline bool operator==(const random_access_iterator& it) const
		{
			return ptr == it.ptr;
		}
		inline bool operator<(const random_access_iterator& it) const
		{
			return ptr < it.ptr;
		}
		inline random_access_iterator<T> operator--()
		{
			--ptr;
			return *this;
		}
		inline random_access_iterator<T> operator--(int)
		{
			random_access_iterator<T> temp(*this);
			--ptr;
			return temp;
		}
		inline size_t operator-(const random_access_iterator<T>& iter) const
		{
			return ptr - iter.ptr;
		}
		inline random_access_iterator<T> operator+(ssize_t i) const
		{
			return random_access_iterator<T>(ptr + i);
		}
		inline random_access_iterator<T> operator-(ssize_t i) const
		{
			return random_access_iterator<T>(ptr - i);
		}

		friend class const_random_access_iterator<T>;
	private:
		T* ptr;
	};

	/**
	 * A simple const random_access iterator
	 */
	template <class T>
	class const_random_access_iterator
	{
	public:
		typedef const T value_type;

		inline const_random_access_iterator(): ptr(NULL)
		{
		}
		inline explicit const_random_access_iterator(const T* p): ptr(p)
		{
		}
		inline const_random_access_iterator(const random_access_iterator<T>& old) : ptr(old.ptr)
		{
		}
		inline const_random_access_iterator(const const_random_access_iterator<T>& old): ptr(old.ptr)
		{
		}
		inline ~const_random_access_iterator()
		{
		}
		inline const_random_access_iterator& operator=(const const_random_access_iterator<T>& old)
		{
			ptr = old.ptr;
			return *this;
		}

		inline const T* operator->() const { return ptr; }
		inline const T& operator*() const { return *ptr; }
		inline const_random_access_iterator operator++()
		{
			++ptr;
			return *this;
		}
		inline const_random_access_iterator operator++(int)
		{
			const_random_access_iterator temp(*this);
			++ptr;
			return temp;
		}
		inline bool operator!=(const const_random_access_iterator& it)
		{
			return ptr != it.ptr;
		}
		inline bool operator==(const const_random_access_iterator& it)
		{
			return ptr == it.ptr;
		}
		inline bool operator<(const const_random_access_iterator& it) const
		{
			return ptr < it.ptr;
		}
		inline const_random_access_iterator<T> operator--()
		{
			--ptr;
			return *this;
		}
		inline const_random_access_iterator<T> operator--(int)
		{
			const_random_access_iterator<T> temp(*this);
			--ptr;
			return temp;
		}
		inline size_t operator-(const const_random_access_iterator<T>& iter) const
		{
			return ptr - iter.ptr;
		}
		inline const_random_access_iterator<T> operator+(ssize_t i) const
		{
			return const_random_access_iterator<T>(ptr + i);
		}
		inline const_random_access_iterator<T> operator-(ssize_t i) const
		{
			return const_random_access_iterator<T>(ptr - i);
		}
	private:
		const T* ptr;
	};


	template <typename iterator_type>
	class reverse_iterator
	{
	public:
		inline reverse_iterator(): m_iter()
		{
		}
		inline explicit reverse_iterator(const iterator_type& i): m_iter(i)
		{
		}
		inline reverse_iterator(const reverse_iterator<iterator_type>& old): m_iter(old.m_iter)
		{
		}
		inline ~reverse_iterator()
		{
		}
		inline reverse_iterator& operator=(const reverse_iterator<iterator_type>& old)
		{
			m_iter = old.m_iter;
			return *this;
		}

		inline typename iterator_type::value_type* operator->() const
		{
			return &(operator*());
		}
		inline typename iterator_type::value_type& operator*() const
		{
			iterator_type i = m_iter;
			return *--i;
		}
		inline reverse_iterator operator++()
		{
			--m_iter;
			return *this;
		}
		inline reverse_iterator operator++(int)
		{
			reverse_iterator temp(*this);
			---m_iter;
			return temp;
		}
		inline bool operator!=(const reverse_iterator& it)
		{
			return m_iter != it.m_iter;
		}
		inline bool operator==(const reverse_iterator& it)
		{
			return m_iter == it.m_iter;
		}
		inline bool operator<(const reverse_iterator& it) const
		{
			return m_iter > it.m_iter;
		}
		inline reverse_iterator<iterator_type> operator--()
		{
			++m_iter;
			return *this;
		}
		inline reverse_iterator<iterator_type> operator--(int)
		{
			reverse_iterator<iterator_type> temp(*this);
			++m_iter;
			return temp;
		}
		inline size_t operator-(const reverse_iterator<iterator_type>& iter) const
		{
			return iter.m_iter - m_iter;
		}
		inline reverse_iterator<iterator_type> operator+(ssize_t i) const
		{
			return reverse_iterator<iterator_type>(m_iter - i);
		}
		inline reverse_iterator<iterator_type> operator-(ssize_t i) const
		{
			return reverse_iterator<iterator_type>(m_iter + i);
		}
	private:
		iterator_type m_iter;
	};

	template <typename iterator>
	size_t distance(iterator start, iterator finish)
	{
		size_t count = 0;
		while(start != finish)
		{
			++start;
			++count;
		}
		return count;
	}

	template <typename T>
	size_t distance(random_access_iterator<T> start, random_access_iterator<T> finish)
	{
		return finish - start;
	}
	template <typename T>
	size_t distance(const_random_access_iterator<T> start, const_random_access_iterator<T> finish)
	{
		return finish - start;
	}
	template <typename T>
	size_t distance(reverse_iterator<T> start, reverse_iterator<T> finish)
	{
		return finish - start;
	}
}
#endif
