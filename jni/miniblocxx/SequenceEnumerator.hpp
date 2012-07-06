#ifndef BLOCXX_SequenceEnumerator_HPP_INCLUDED_
#define BLOCXX_SequenceEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T>
class SequenceEnumerator : public Enumerator<T>
{
public:
	SequenceEnumerator(T start, T end)
		: _start(start)
		, _end(end)
		, _current(start - 1)
	{}

	bool moveNext() { _current++; return _current <= _end; }
	T current() { return _current; }
	void reset() { _current = _start - 1; }
	size_t count() { return _end - _start; }

private:
	T _start;
	T _end;
	T _current;
};

}

#endif

