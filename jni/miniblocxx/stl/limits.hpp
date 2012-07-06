#if !defined(BLOCXX__limits_HPP)
#define BLOCXX__limits_HPP

// This one is required by C99:
#include <limits.h>
// This one works around missing headers
#include "miniblocxx/stl/float.hpp"

namespace BLOCXX_NAMESPACE
{
	template <typename T>
	struct numeric_limits
	{
		static const bool is_specialized = false;
		static T min() throw() { return T(0); }
		static T max() throw() { return T(0); }
		static const bool is_signed = false;
		static T epsilon() throw() { return T(0); }
		static const bool is_exact = false;
	};

	template <>
	struct numeric_limits<bool>
	{
		static const bool is_specialized = true;
		static bool min() throw() { return false; }
		static bool max() throw() { return true; }
		static const bool is_signed = false;
		static bool epsilon() throw() { return false; }
		static const bool is_exact = true;
	};

#define NUMERIC_LIMITS_DEFINITION(T, vmin, vmax, eps, exact) \
	template <> struct numeric_limits<T> \
	{ \
		static const bool is_specialized = true; \
		static T min() throw() { return (vmin); } \
		static T max() throw() { return (vmax); } \
		static const bool is_signed = !(vmin == 0); \
		static T epsilon() throw() { return (T)(eps); }	\
		static const bool is_exact = exact; \
	}

	//static const bool is_exact = (((eps) < (T)(1)) && ((eps) > (T)(0))); 
	
	// The gcc implementation defines epsilon as zero for all integer types.  I disagree, but am following suit:
	NUMERIC_LIMITS_DEFINITION(char, CHAR_MIN, CHAR_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(signed char, SCHAR_MIN, SCHAR_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(unsigned char, 0, UCHAR_MAX, 0, true);

	NUMERIC_LIMITS_DEFINITION(short, SHRT_MIN, SHRT_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(unsigned short, 0, USHRT_MAX, 0, true);

	NUMERIC_LIMITS_DEFINITION(int, INT_MIN, INT_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(unsigned int, 0, UINT_MAX, 0, true);

	NUMERIC_LIMITS_DEFINITION(long, LONG_MIN, LONG_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(unsigned long, 0, ULONG_MAX, 0, true);

	NUMERIC_LIMITS_DEFINITION(long long, LLONG_MIN, LLONG_MAX, 0, true);
	NUMERIC_LIMITS_DEFINITION(unsigned long long, 0, ULLONG_MAX, 0, true);

	NUMERIC_LIMITS_DEFINITION(float, FLT_MIN, FLT_MAX, FLT_EPSILON, false);
	NUMERIC_LIMITS_DEFINITION(double, DBL_MIN, DBL_MAX, DBL_EPSILON, false);
	NUMERIC_LIMITS_DEFINITION(long double, LDBL_MIN, LDBL_MAX, LDBL_EPSILON, false);
#undef NUMERIC_LIMITS_DEFINITION

	template <typename T>
	T calculate_epsilon() throw()
	{
		static bool s_calculated = false;
		static volatile T s_eps = 0;
		if( s_calculated )
		{
			return s_eps;
		}
		T two(2);
		// Prevent these from being optimized.
		volatile const T one(1);
		volatile T sum = two;
		while( sum > one )
		{
			s_eps /= two;
			sum = one + s_eps;
		}
		s_eps *= two;
		s_calculated = true;
		return s_eps;
	}
} // namespace BLOCXX_NAMESPACE

#endif /* !defined(BLOCXX__limits_HPP) */
