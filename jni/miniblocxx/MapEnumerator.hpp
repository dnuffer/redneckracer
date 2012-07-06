#ifndef BLOCXX_MapEnumerator_HPP_INCLUDED_
#define BLOCXX_MapEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"
#include "miniblocxx/IntrusiveReference.hpp"
#include "miniblocxx/Function.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T, typename U>
class MapEnumerator : public Enumerator<U>
{
public:
	MapEnumerator(const IntrusiveReference<Enumerator<T> >& source, const Function1<U, T>& mapFunc)
		: _source(source)
		, _mapFunc(mapFunc)
	{}

	virtual bool moveNext()
	{
		return _source.moveNext();
	}

	virtual T current()
	{
		return _mapFunc(_source.current());
	}

	virtual void reset()
	{
		_source.reset();
	}

private:
	IntrusiveReference<Enumerator<T> > _source;
	Function1<U, T> _mapFunc;
};

}

#endif
