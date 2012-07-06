#ifndef BLOCXX_FilterEnumerator_HPP_INCLUDED_
#define BLOCXX_FilterEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"
#include "miniblocxx/IntrusiveReference.hpp"
#include "miniblocxx/Function.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T>
class FilterEnumerator : public Enumerator<T>
{
public:
	FilterEnumerator(const IntrusiveReference<Enumerator<T> >& source, const Function1<bool, T>& predicate)
		: _source(source)
		, _predicate(predicate)
	{}

	virtual bool moveNext()
	{
		while (_source.moveNext())
		{
			_current = _source.current();
			if (_predicate(_current))
			{
				return true;
			}
		}
		return false;
	}

	virtual T current()
	{
		return _current;
	}

	virtual void reset()
	{
		_source.reset();
	}



private:
	IntrusiveReference<Enumerator<T> > _source;
	Function1<bool, T> _predicate;
	T _current;
};

}

#endif
