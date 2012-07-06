#ifndef BLOCXX_Enumerator_HPP_INCLUDED_
#define BLOCXX_Enumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/CommonFwd.hpp"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "miniblocxx/IntrusiveReference.hpp"
#include "miniblocxx/RepeatEnumerator.hpp"
#include "miniblocxx/Array.hpp"
#include "miniblocxx/stl/algorithm"
#include "miniblocxx/ArrayEnumerator.hpp"
#include "miniblocxx/TakeEnumerator.hpp"
#include "miniblocxx/SequenceEnumerator.hpp"
#include "miniblocxx/Function.hpp"
#include "miniblocxx/MapEnumerator.hpp"
#include "miniblocxx/FilterEnumerator.hpp"


namespace BLOCXX_NAMESPACE
{

template <typename T>
class Enumerator : public IntrusiveCountableBase
{
public:
	virtual bool moveNext() = 0;
	virtual T current() = 0;
	virtual void reset() = 0;


	IntrusiveReference<Enumerator<T> > repeat(unsigned count)
	{
		return new RepeatEnumerator<T>(this, count);
	}

	IntrusiveReference<Enumerator<T> > randomShuffle()
	{
		Array<T> data = toArray();
		std::random_shuffle(data.begin(), data.end());
		return fromArray(data);
	}

	IntrusiveReference<Enumerator<T> > take(int count)
	{
		return new TakeEnumerator<T>(this, count);
	}

	template <typename U>
	IntrusiveReference<Enumerator<U> > map(Function1<U, T> f)
	{
		return new MapEnumerator<T, U>(this, f);
	}

	IntrusiveReference<Enumerator<T> > filter(Function1<bool, T> f)
	{
		return new FilterEnumerator<T>(this, f);
	}

	Array<T> toArray()
	{
		Array<T> result;
		while (moveNext())
			result.push_back(current());
		reset();
		return result;
	}

	virtual size_t count()
	{
		size_t result = 0;
		while (moveNext()) ++result;
		reset();
		return result;
	}

	static IntrusiveReference<Enumerator<T> > generateSequence(T start, T end)
	{
		return new SequenceEnumerator<T>(start, end);
	}

	static IntrusiveReference<Enumerator<T> > fromArray(const Array<T>& array)
	{
		return new ArrayEnumerator<T>(array);
	}
};



}


#endif
