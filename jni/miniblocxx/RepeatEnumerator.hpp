#ifndef BLOCXX_RepeatEnumerator_HPP_INCLUDED_
#define BLOCXX_RepeatEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"
#include "miniblocxx/IntrusiveReference.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T>
class RepeatEnumerator : public Enumerator<T>
{
public:
	RepeatEnumerator(const IntrusiveReference<Enumerator<T> >& toWrap, unsigned count)
		: wrappedEnumerator(toWrap)
		, repeatCount(count)
		, doneCount(0)
	{}

	bool moveNext()
	{
		if (doneCount == repeatCount)
			return false;

		if (!wrappedEnumerator->moveNext())
		{
			++doneCount;
			wrappedEnumerator->reset();
			return this->moveNext();
		}
		return true;
	}
	T current() { return wrappedEnumerator->current(); }
	void reset() { doneCount = 0; wrappedEnumerator->reset(); }
	size_t count() { return wrappedEnumerator->count() * repeatCount; }

private:
	IntrusiveReference<Enumerator<T> > wrappedEnumerator;
	unsigned repeatCount;
	unsigned doneCount;
};



}

#endif
