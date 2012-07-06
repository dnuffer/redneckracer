#ifndef BLOCXX_LARGER_HPP_INCLUDE_GUARD_
#define BLOCXX_LARGER_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/Types.hpp"

namespace BLOCXX_NAMESPACE
{
	template <typename T>
	struct Larger
	{
		typedef T type;
	};

	template<> struct Larger<char> { typedef Int16 type; };
	template<> struct Larger<unsigned char> { typedef UInt16 type; };
	template<> struct Larger<signed char> { typedef Int16 type; };
	template<> struct Larger<Int16> { typedef Int32 type; };
	template<> struct Larger<UInt16> { typedef UInt32 type; };

#if !defined(BLOCXX_INT16_IS_SHORT) || (BLOCXX_INT16_IS_SHORT != 1)
	template<> struct Larger<short> { typedef Int32 type; };
	template<> struct Larger<unsigned short> { typedef UInt32 type; };
#endif

#if !defined(BLOCXX_INT16_IS_INT) && !defined(BLOCXX_INT32_IS_INT)
	template<> struct Larger<int> { typedef Int64 type; };
	template<> struct Larger<unsigned int> { typedef UInt64 type; };
#endif

	template<> struct Larger<Int32> { typedef Int64 type; };
	template<> struct Larger<UInt32> { typedef UInt64 type; };

	template<> struct Larger<Real32> { typedef Real64 type; };

#if !defined(BLOCXX_REAL32_IS_FLOAT) || (BLOCXX_REAL32_IS_FLOAT != 1)
	template<> struct Larger<float> { typedef Real64 type; };
#endif
#if defined(BLOCXX_REAL64_IS_LONG_DOUBLE) && (BLOCXX_REAL64_IS_LONG_DOUBLE == 1)
	template<> struct Larger<double> { typedef Real64 type; };
#endif

} // end namespace BLOCXX_NAMESPACE

#endif
