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
 * @author Jon Carey
 * @author Dan Nuffer
 */

#ifndef BLOCXX_AUTO_PTR_HPP_INCLUDE_GUARD_
#define BLOCXX_AUTO_PTR_HPP_INCLUDE_GUARD_
#include "miniblocxx/BLOCXX_config.h"

namespace BLOCXX_NAMESPACE
{

/**
 * The AutoPtr class provides a simple class for smart pointers to single
 * objects (for multiple, see AutoPtrVec); it stores a pointer to an object
 * obtained by way of the new operator and deletes that object when the
 * AutoPtr object is destroyed. The AutoPtr class provides semantics of strict
 * ownership. After construction an AutoPtr object owns the object whose
 * pointer it holds. When an instantiation of AutoPtr is copied, ownership
 * of the object is transferred to the destination AutoPtr. The behavior is
 * undefined if more than one instantiation of AutoPtr owns the same object
 * at the same time.
 * Note: This class should not be used for a smart pointer to a dynamically
 * allocated array. AutoPtrVec should be used for that. If AutoPtr holds
 * a pointer to an array, the destructor for all objects in the array will
 * not be called when AutoPtr delete the pointer
 *
 * @todo Rename this class
 */
template <class X> class AutoPtr
{
private:
	X* _ptr;

	// no copying
	AutoPtr(const AutoPtr& a);
	AutoPtr& operator= (const AutoPtr& a);

public:
	typedef X element_type;

	/**
	 * Construct a new AutoPtr object that assumes ownership of
	 * a given pointer.
	 * @param p pointer to the object
	 */
	explicit AutoPtr(X* p = 0);

	/**
	 * Destroy this AutoPtr object and the object it points to.
	 */
	~AutoPtr();

	/**
	 * Assignment operator
	 * Assign a new pointer to this AutoPtr object. If this AutoPtr
	 * object points to another object before this assignment, then
	 * that object will be destroyed first.
	 * @param p The new pointer this AutoPtr object will own.
	 * @return A reference to this AutoPtr object.
	 */
	AutoPtr& operator= (X* p);

	/**
	 * @return A reference to the object to which the underlying held
	 * pointer points.
	 */
	X& operator*() const;

	/**
	 * @return The underlying held pointer.
	 */
	X* operator->() const;

	/**
	 * @return The underlying held pointer.
	 */
	X* get() const;

	/**
	 * Release ownership of the underlying pointer.
	 * @return The value of the underlying held pointer before the
	 * call to release
	 */
	X* release();

	/**
	 * Delete the object the underlying pointer points to and take
	 * ownership of a new pointer. This is similar to the
	 * assignment operator.
	 * @param p The pointer this AutoPtr will take ownership of.
	 */
	void reset(X* p=0);
};

template <class X>
inline AutoPtr<X>::AutoPtr(X* p) : _ptr(p) {}

template <class X>
inline AutoPtr<X>& AutoPtr<X>::operator= (X* p)
{
	if (p != _ptr)
	{
		reset();
		_ptr = p;
	}
	return *this;
}

template <class X>
inline AutoPtr<X>::~AutoPtr()
{
	typedef char type_must_be_complete[sizeof(X)];
	delete _ptr;
}

template <class X>
inline X& AutoPtr<X>::operator*() const {  return *_ptr;}

template <class X>
inline X* AutoPtr<X>::operator->() const {  return _ptr;}

template <class X>
inline X* AutoPtr<X>::get() const {  return _ptr;}

template <class X>
inline X* AutoPtr<X>::release()
{
	X* rval = _ptr;
	_ptr = 0;
	return rval;
}

template <class X>
inline void AutoPtr<X>::reset(X* p)
{
	delete _ptr;
	_ptr = p;
}

/**
 * The AutoPtrVec class provides a simple class for smart pointers to a
 * dynamically allocated array of objects. It stores a pointer to the array
 * obtained by way of the new operator and deletes the array when the
 * AutoPtrVec object is destroyed. The AutoPtrVec class provides semantics of
 * strict ownership. After construction an AutoPtrVec object owns the array
 * whose pointer it holds. When an instantiation of AutoPtrVec is copied,
 * ownership of the array is transferred to the destination AutoPtrVec. The
 * behavior is undefined if more than one instantiation of AutoPtrVec owns the
 * same array at the same time.
 */
template <class X> class AutoPtrVec
{
private:
	X* _ptr;

	// no copying
	AutoPtrVec(const AutoPtrVec& a);
	AutoPtrVec& operator= (const AutoPtrVec& a);

public:
	typedef X element_type;

	/**
	 * Construct a new AutoPtrVec object that assumes ownership of
	 * a given array pointer.
	 * @param p pointer to the array.
	 */
	explicit AutoPtrVec(X* p = 0);

	/**
	 * Destroy this AutoPtrVec object and the array it points to.
	 */
	~AutoPtrVec();

	/**
	 * Assignment operator
	 * Assign a new array pointer to this AutoPtrVec object. If this AutoPtrVec
	 * object points to another array before this assignment, then
	 * that array will be destroyed first.
	 * @param p The new array pointer this AutoPtrVec object will own.
	 * @return A reference to this AutoPtrVec object.
	 */
	AutoPtrVec& operator= (X* p);

	/**
	 * @return A reference to the object to which the underlying held
	 * pointer points.
	 */
	X& operator*() const;

	/**
	 * @return The underlying held pointer.
	 */
	X* operator->() const;

	/**
	 * @return A read/write reference to the nth element of the array owned
	 * by this AutoPtrVec object.
	 */
	X& operator[](unsigned n);

	/**
	 * @return A read only reference to the nth element of the array owned
	 * by this AutoPtrVec object.
	 */
	const X& operator[](unsigned i) const;

	/**
	 * @return The underlying pointer to the array.
	 */
	X* get() const;

	/**
	 * Release ownership of the underlying array.
	 * @return The value of the underlying held array pointer before the
	 * call to release.
	 */
	X* release();

	/**
	 * Delete the array the underlying pointer points to and take
	 * ownership of a new array pointer. This is similar to the
	 * assignment operator.
	 * @param p The array pointer this AutoPtr will take ownership of.
	 */
	void reset(X* p=0);
};


template <class X>
inline AutoPtrVec<X>::AutoPtrVec(X* p) : _ptr(p) {}

template <class X>
AutoPtrVec<X>& AutoPtrVec<X>::operator= (X* p)
{
	if (p != _ptr)
	{
		reset();
		_ptr = p;
	}
	return *this;
}

template <class X>
AutoPtrVec<X>::~AutoPtrVec()
{
	typedef char type_must_be_complete[sizeof(X)];
	delete [] _ptr;
}

template <class X>
X& AutoPtrVec<X>::operator*() const {  return *_ptr;}

template <class X>
X* AutoPtrVec<X>::operator->() const {  return _ptr;}

template <class X>
X& AutoPtrVec<X>::operator[](unsigned i) { return _ptr[i]; }

template <class X>
const X& AutoPtrVec<X>::operator[](unsigned i) const { return _ptr[i]; }

template <class X>
X* AutoPtrVec<X>::get() const {  return _ptr;}

template <class X>
X* AutoPtrVec<X>::release()
{
	X* rval = _ptr;
	_ptr = 0;
	return rval;
}

template <class X>
void AutoPtrVec<X>::reset(X* p)
{
	delete [] _ptr;
	_ptr = p;
}

} // end namespace BLOCXX_NAMESPACE

#endif
