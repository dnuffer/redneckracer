#if       !defined(BLOCXX__algorithm_HPP)
#define            BLOCXX__algorithm_HPP

#include <stdlib.h>

namespace BLOCXX_NAMESPACE
{
	template<typename iterator, typename T>
	iterator lower_bound(iterator first, iterator last, const T& val)
	{
		// The slow implementation...
		for(; first < last; ++first )
		{
			if( !(*first < val) )
			{
				break;
			}
		}
		return first;
	}

	template<typename iterator, typename T>
	iterator find(iterator first, iterator last, const T& val)
	{
		// The slow implementation...
		for(; first < last; ++first )
		{
			if( *first == val )
			{
				break;
			}
		}
		return first;
	}

	template<typename iterator, typename T, typename comparison>
	iterator lower_bound(iterator first, iterator last, const T& val, comparison compare)
	{
		// The slow implementation...
		for(; first < last; ++first )
		{
			if( !compare(*first, val) )
			{
				break;
			}
		}
		return first;
	}

	template<typename T>
	void swap(T& t1, T& t2)
	{
		T temp(t1);
		t1 = t2;
		t2 = temp;
	}

	template <typename T>
	T min(const T& t1, const T& t2)
	{
		if( t2 < t1 )
		{
			return t2;
		}
		return t1;
	}

	template <typename T>
	T max(const T& t1, const T& t2)
	{
		if( t2 > t1 )
		{
			return t2;
		}
		return t1;
	}

	template <typename T>
	inline T random_number(T n)
	{
	  return lrand48() % n;
	}

	template <typename T>
	void random_shuffle(T first, T last)
	{
		if (first == last) return;
		for (T i = first + 1; i != last; ++i)
			swap(*i, *(first + random_number((i - first) + 1)));
	}
} // end BLOCXX_NAMESPACE

#endif /* !defined(BLOCXX__algorithm_HPP) */
