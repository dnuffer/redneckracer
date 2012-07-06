
#define PROVIDE_AUTO_TEST_MAIN
#include "AutoTest.hpp"

#include "miniblocxx/Enumerator.hpp"

using namespace blocxx;

AUTO_UNIT_TEST(EnumeratorTests_generateSequence)
{
	IntrusiveReference<Enumerator<int> > e = Enumerator<int>::generateSequence(1, 3);
	unitAssert(e);
	unitAssert(e->moveNext());
	unitAssertEquals(1, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(2, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(3, e->current());
	unitAssertEquals(false, e->moveNext());

	e->reset();
	unitAssert(e->moveNext());
	unitAssertEquals(1, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(2, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(3, e->current());
	unitAssertEquals(false, e->moveNext());
}

AUTO_UNIT_TEST(EnumeratorTests_repeat)
{
	IntrusiveReference<Enumerator<int> > e = Enumerator<int>::generateSequence(1, 2)->repeat(2);
	unitAssert(e);
	unitAssert(e->moveNext());
	unitAssertEquals(1, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(2, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(1, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(2, e->current());
	unitAssertEquals(false, e->moveNext());
}

AUTO_UNIT_TEST(EnumeratorTests_toArray)
{
	Array<int> a = Enumerator<int>::generateSequence(1,3)->toArray();
	unitAssertEquals(3U, a.size());
	unitAssertEquals(1, a[0]);
	unitAssertEquals(2, a[1]);
	unitAssertEquals(3, a[2]);
}

AUTO_UNIT_TEST(EnumeratorTests_fromArray)
{
	Array<int> a; a.push_back(3); a.push_back(9);
	IntrusiveReference<Enumerator<int> > e = Enumerator<int>::fromArray(a);
	unitAssert(e);
	unitAssert(e->moveNext());
	unitAssertEquals(3, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(9, e->current());
	unitAssertEquals(false, e->moveNext());

	e->reset();
	unitAssert(e->moveNext());
	unitAssertEquals(3, e->current());
	unitAssert(e->moveNext());
	unitAssertEquals(9, e->current());
	unitAssertEquals(false, e->moveNext());
}

AUTO_UNIT_TEST(EnumeratorTests_randomShuffle)
{
	IntrusiveReference<Enumerator<int> > e = Enumerator<int>::generateSequence(1, 3)->randomShuffle();
	unitAssert(e);
	unitAssert(e->moveNext());
	unitAssert(e->moveNext());
	unitAssert(e->moveNext());
	unitAssertEquals(false, e->moveNext());

	e->reset();
	unitAssert(e->moveNext());
	unitAssert(e->moveNext());
	unitAssert(e->moveNext());
	unitAssertEquals(false, e->moveNext());
}
