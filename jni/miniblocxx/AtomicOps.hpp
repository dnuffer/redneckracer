/*******************************************************************************
* Copyright (C) 2005, Quest Software, Inc. All rights reserved.
* Copyright (C) 2006, Novell, Inc. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of
*       Quest Software, Inc.,
*       nor Novell, Inc.,
*       nor the names of its contributors or employees may be used to
*       endorse or promote products derived from this software without
*       specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/


/**
 * @author Dan Nuffer
 * @author Kevin Harris
 */

#ifndef BLOCXX_ATOMIC_OPS_HPP_INCLUDE_GUARD_
#define BLOCXX_ATOMIC_OPS_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

#if defined(BLOCXX_AIX)
extern "C"
{
#include <sys/atomic_op.h>
}
#elif defined(__HP_aCC) && defined(BLOCXX_ARCH_IA64)
#include <machine/sys/inline.h>
#endif

// The classes and functions defined in this file are not meant for general
// use, they are internal implementation details.  They may change at any time.

/**
 * @todo PA-RISC and itanium.  IA64 is supported for aCC, but not for gcc.
 * @see http://h21007.www2.hp.com/dspp/files/unprotected/Itanium/spinlocks.pdf
 * @see http://www.hpl.hp.com/research/linux/atomic_ops/
 */

#if defined(__GNUC__) && (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 1)
// New implementation based on the gcc 4.1+ sync operations.
namespace BLOCXX_NAMESPACE
{

// use fast inline assembly versions
struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};

inline int AtomicIncAndGet(Atomic_t &v)
{
	return __sync_fetch_and_add(&v.val, 1) + 1;
}
inline void AtomicInc(Atomic_t &v)
{
	AtomicIncAndGet(v);
}
inline int AtomicGet(Atomic_t const &v)
{
	__sync_synchronize();
	return v.val;
}
inline int AtomicDecAndGet(Atomic_t &v)
{
		return __sync_fetch_and_add(&v.val, -1) - 1;
}
inline void AtomicDec(Atomic_t &v)
{
	AtomicDecAndGet(v);
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	return AtomicDecAndGet(v) == 0;
}
} // end namespace BLOCXX_NAMESPACE

// x86 and x86-64 asm is identical
#elif (defined(BLOCXX_ARCH_X86) || defined(__i386__) || defined(BLOCXX_ARCH_X86_64) || defined(__x86_64__)) && defined(__GNUC__)

namespace BLOCXX_NAMESPACE
{

// use fast inline assembly versions
struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};
inline void AtomicInc(Atomic_t &v)
{
	__asm__ __volatile__(
		"lock ; " "incl %0"
		:"=m" (v.val)
		:"m" (v.val));
}
inline int AtomicIncAndGet(Atomic_t &v)
{
	int retval = 1;
	__asm__ __volatile__(
		"lock ; " "xaddl %0, %1"
		:"+r" (retval), "+m" (v.val)
		: : "memory");
	return retval + 1;
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	unsigned char c;
	__asm__ __volatile__(
		"lock ; " "decl %0; sete %1"
		:"=m" (v.val), "=qm" (c)
		:"m" (v.val) : "memory");
	return c != 0;
}
inline int AtomicGet(Atomic_t const &v)
{
	return v.val;
}
inline void AtomicDec(Atomic_t &v)
{
	__asm__ __volatile__(
		"lock ; " "decl %0"
		:"=m" (v.val)
		:"m" (v.val));
}
inline int AtomicDecAndGet(Atomic_t &v)
{
	int retval = -1;
	__asm__ __volatile__(
		"lock ; " "xaddl %0, %1"
		:"+r" (retval), "+m" (v.val)
		: : "memory");
	return retval - 1;
}

} // end namespace BLOCXX_NAMESPACE
#elif defined(__HP_aCC) && defined(BLOCXX_ARCH_IA64)
namespace BLOCXX_NAMESPACE
{
struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};
inline void AtomicInc(Atomic_t &v)
{
	_Asm_fetchadd(_FASZ_W, _SEM_ACQ, &v.val, 1, _LDHINT_NONE);
}
inline int AtomicIncAndGet(Atomic_t &v)
{
	return int(_Asm_fetchadd(_FASZ_W, _SEM_ACQ, &v.val, 1, _LDHINT_NONE)) + 1;
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	int c = int(_Asm_fetchadd(_FASZ_W, _SEM_ACQ, &v.val, int(-1), _LDHINT_NONE));
 		--c;
	return c == 0;
}
inline int AtomicGet(Atomic_t const &v)
{
	return int(v.val);
}
inline void AtomicDec(Atomic_t &v)
{
	_Asm_fetchadd(_FASZ_W, _SEM_ACQ, &v.val, -1, _LDHINT_NONE);
}
inline int AtomicDecAndGet(Atomic_t &v)
{
	return int(_Asm_fetchadd(_FASZ_W, _SEM_ACQ, &v.val, -1, _LDHINT_NONE)) - 1;
}
}

#elif defined(BLOCXX_AIX)
namespace BLOCXX_NAMESPACE
{
// This comment was stolen from the libstdc++ implementation of atomicity.h
// (and modified).
// We cannot use the inline assembly for powerpc, since definitions for
// these operations since they depend on operations that are not available on
// the original POWER architecture.  AIX still runs on the POWER architecture,
// so it would be incorrect to assume the existence of these instructions.
//
// The definition of Atomic_t.val must match the type pointed to by atomic_p in
// <sys/atomic_op.h>.
struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};

inline void AtomicInc(Atomic_t &v)
{
	::fetch_and_add(const_cast<atomic_p>(&v.val), 1);
}
inline int AtomicIncAndGet(Atomic_t &v)
{
	// fetch_and_add returns the original value before the add operation.  Thus,
	// we must add one from the returned value before returning.
	return ::fetch_and_add(const_cast<atomic_p>(&v.val), 1) + 1;
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	// fetch_and_add returns the original value before the add operation.  Thus,
	// we must subtract one from the returned value before comparing.
	int c = ::fetch_and_add(const_cast<atomic_p>(&v.val), -1);
	--c;
	return c == 0;
}
inline int AtomicGet(Atomic_t const &v)
{
	int c = ::fetch_and_add(const_cast<atomic_p>(&v.val), 0);
	return c;
}
inline void AtomicDec(Atomic_t &v)
{
	::fetch_and_add(const_cast<atomic_p>(&v.val), -1);
}
inline int AtomicDecAndGet(Atomic_t &v)
{
	// fetch_and_add returns the original value before the add operation.  Thus,
	// we must subtract one from the returned value before returning.
	return ::fetch_and_add(const_cast<atomic_p>(&v.val), -1) - 1;
}

} // end namespace BLOCXX_NAMESPACE

#elif (defined(BLOCXX_ARCH_PPC) || defined(__ppc__)) && defined(__GNUC__)

namespace BLOCXX_NAMESPACE
{

// use fast inline assembly versions
struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};

inline void AtomicInc(Atomic_t &v)
{
	int t;
	__asm__ __volatile__(
		"1:	lwarx   %0,0,%2\n"
		"	addic   %0,%0,1\n"
		"	stwcx.  %0,0,%2\n"
		"	bne-    1b"
		: "=&r" (t), "=m" (v.val)
		: "r" (&v.val), "m" (v.val)
		: "cc");
}
inline int AtomicIncAndGet(Atomic_t &v)
{
	int c;
	__asm__ __volatile__(
		"1:	lwarx   %0,0,%1\n"
		"	addic   %0,%0,1\n"
		"	stwcx.  %0,0,%1\n"
		"	bne-    1b\n"
		"	isync"
		: "=&r" (c)
		: "r" (&v.val)
		: "cc", "memory");
	return c;
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	int c;
	__asm__ __volatile__(
		"1:	lwarx   %0,0,%1\n"
		"	addic   %0,%0,-1\n"
		"	stwcx.  %0,0,%1\n"
		"	bne-    1b\n"
		"	isync"
		: "=&r" (c)
		: "r" (&v.val)
		: "cc", "memory");
	return c == 0;
}
inline int AtomicGet(Atomic_t const &v)
{
	return v.val;
}
inline void AtomicDec(Atomic_t &v)
{
	int c;
	__asm__ __volatile__(
		"1:	lwarx   %0,0,%2\n"
		"	addic   %0,%0,-1\n"
		"	stwcx.  %0,0,%2\n"
		"	bne-    1b"
		: "=&r" (c), "=m" (v.val)
		: "r" (&v.val), "m" (v.val)
		: "cc");
}
inline int AtomicDecAndGet(Atomic_t &v)
{
	int c;
	__asm__ __volatile__(
		"1:	lwarx   %0,0,%1\n"
		"	addic   %0,%0,-1\n"
		"	stwcx.  %0,0,%1\n"
		"	bne-    1b\n"
		"	isync"
		: "=&r" (c)
		: "r" (&v.val)
		: "cc", "memory");
	return c;
}

} // end namespace BLOCXX_NAMESPACE

#elif defined(BLOCXX_WIN32)

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <wtypes.h>

namespace BLOCXX_NAMESPACE
{

// use fast inline assembly versions
struct BLOCXX_COMMON_API Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile long val;
};
inline void AtomicInc(Atomic_t &v)
{
	InterlockedIncrement(&v.val);
}
inline int AtomicIncAndGet(Atomic_t &v)
{
	return InterlockedIncrement(&v.val);
}
inline bool AtomicDecAndTest(Atomic_t &v)
{
	return InterlockedDecrement(&v.val) == 0;
}
inline int AtomicGet(Atomic_t const &v)
{
	return v.val;
}
inline void AtomicDec(Atomic_t &v)
{
	InterlockedDecrement(&v.val);
}
inline int AtomicDecAndGet(Atomic_t &v)
{
	return InterlockedDecrement(&v.val);
}

} // end namespace BLOCXX_NAMESPACE

#elif defined(BLOCXX_HAVE_PTHREAD_SPIN_LOCK)
#include <pthread.h>

#define BLOCXX_USE_PTHREAD_SPIN_LOCK_ATOMIC_OPS // used in BLOCXX_AtomicOps.cpp

namespace BLOCXX_NAMESPACE
{
/**
 * The Atomic_t structure is used by all the AtomicXXX functions.
 */
struct Atomic_t
{
	/**
	 * Construct an Atomic_t structure with the value initialized to
	 * zero.
	 */
	Atomic_t();

	/**
	 * Construct an Atomic_t structure with the value initialized to
	 * a given value.
	 * @param i The value to initialize this Atomic_t to.
	 */
	Atomic_t(int i);

	/** The current value for theis Atomic_t */
	int val;

	pthread_spinlock_t spinlock;
};

/**
 * Increment the value in a given Atomic_t structure
 * @param v The Atomic_t to increment the value in.
 */
void AtomicInc(Atomic_t &v);

/**
 * Increment the value in a given Atomic_t structure
 * @param v The Atomic_t to increment the value in.
 * @return the post-increment value contained in the Atomic_t
 */
int AtomicIncAndGet(Atomic_t &v);

/**
 * Decrement the value in a given Atomic_t structure and test it for zero.
 * @param v The Atomic_t to decrement and test.
 * @return true if the value in v has reached zero.
 */
bool AtomicDecAndTest(Atomic_t &v);
/**
 * Get the value from a given Atomic_t structure
 * @param v The Atomic_t to get the value from.
 * @return the value contained in the Atomic_t
 */
int AtomicGet(Atomic_t const &v);

/**
 * Decrement the value in a given Atomic_t structure.
 * @param v The Atomic_t to decrement.
 */
void AtomicDec(Atomic_t &v);

/**
 * Decrement the value in a given Atomic_t structure.
 * @param v The Atomic_t to decrement.
 * @return the post-decrement value contained in the Atomic_t
 */
int AtomicDecAndGet(Atomic_t &v);

} // end namespace BLOCXX_NAMESPACE

#else
// use slow mutex protected versions
#define BLOCXX_USE_BLOCXX_DEFAULT_ATOMIC_OPS // used in BLOCXX_AtomicOps.cpp

namespace BLOCXX_NAMESPACE
{

struct Atomic_t
{
	Atomic_t() : val(0) {}
	Atomic_t(int i) : val(i) {}
	volatile int val;
};
void AtomicInc(Atomic_t &v);
int AtomicIncAndGet(Atomic_t &v);
bool AtomicDecAndTest(Atomic_t &v);
int AtomicGet(Atomic_t const &v);
void AtomicDec(Atomic_t &v);
int AtomicDecAndGet(Atomic_t &v);

} // end namespace BLOCXX_NAMESPACE

#endif
#endif
