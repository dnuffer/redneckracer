#ifndef BLOCXX_ArrayEnumerator_HPP_INCLUDED_
#define BLOCXX_ArrayEnumerator_HPP_INCLUDED_

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Enumerator.hpp"
#include "miniblocxx/Array.hpp"

namespace BLOCXX_NAMESPACE
{

template <typename T>
class ArrayEnumerator : public Enumerator<T>
{
public:
	ArrayEnumerator(const Array<T>& array)
		: _array(array)
		, _position(_array.end())
	{}

	bool moveNext()
	{
		if (_position == _array.end())
		{
			_position = _array.begin();
		}
		else
		{
			++_position;
		}
		return _position < _array.end();
	}
	T current() { return *_position; }
	void reset() { _position = _array.end(); }
	size_t count() { return _array.size(); }

private:
	Array<T> _array;
	typename Array<T>::const_iterator _position;
};


}

#endif
