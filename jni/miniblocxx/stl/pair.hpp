#if !defined(BLOCXX__pair_HPP)
#define BLOCXX__pair_HPP

namespace BLOCXX_NAMESPACE
{
	template <typename T1, typename T2>
	struct pair
	{
		T1 first;
		T2 second;

		pair(const T1& t1, const T2& t2) : first(t1), second(t2) { }
		pair() : first(), second() { }
	};

	template <typename T1, typename T2>
	pair<T1,T2> make_pair(const T1& t1, const T2& t2)
	{
		return pair<T1,T2>(t1,t2);
	}
} // namespace BLOCXX_NAMESPACE

#endif /* !defined(BLOCXX__pair_HPP) */
