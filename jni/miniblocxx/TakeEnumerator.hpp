#ifndef BLOCXX_TakeEnumerator_HPP_INCLUDED_
#define BLOCXX_TakeEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"
#include "miniblocxx/IntrusiveReference.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T>
class TakeEnumerator : public Enumerator<T>
{
public:
	TakeEnumerator(IntrusiveReference<Enumerator<T> > toWrap, unsigned count)
		: wrappedEnumerator(toWrap)
		, takeCount(count)
		, doneCount(0)
	{}

	bool moveNext()
	{
		if (doneCount == takeCount)
			return false;

		if (!wrappedEnumerator->moveNext())
		{
			doneCount = takeCount; // set so that wrappedEnumerator->moveNext() won't be called on further calls to moveNext()
			return false;
		}
		++doneCount;
		return true;
	}
	T current() { return wrappedEnumerator->current(); }
	void reset() { doneCount = 0; wrappedEnumerator->reset(); }
	size_t count() { return max(wrappedEnumerator->count(), takeCount); }

private:
	IntrusiveReference<Enumerator<T> > wrappedEnumerator;
	unsigned takeCount;
	unsigned doneCount;
};


}

#endif
