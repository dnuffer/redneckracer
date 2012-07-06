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

////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2000 Andrei Alexandrescu
// Copyright (c) 2000 Petru Marginean
// Copyright (c) 2005 Joshua Lehrer
//
// Permission to use, copy, modify, distribute and sell this software for any
//     purpose is hereby granted without fee, provided that the above copyright
//     notice appear in all copies and that both that copyright notice and this
//     permission notice appear in supporting documentation.
// The author makes no representations about the
//     suitability of this software for any purpose. It is provided "as is"
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////
#ifndef BLOCXX_SCOPE_GUARD_HPP_INCLUDE_GUARD_
#define BLOCXX_SCOPE_GUARD_HPP_INCLUDE_GUARD_

#include "miniblocxx/BLOCXX_config.h"
//#include "miniblocxx/RefToValue.hpp"

namespace BLOCXX_NAMESPACE
{

    class ScopeGuardImplBase
    {
        ScopeGuardImplBase& operator =(const ScopeGuardImplBase&);

    protected:

        ~ScopeGuardImplBase()
        {}

        ScopeGuardImplBase(const ScopeGuardImplBase& other) throw()
            : m_dismissed(other.m_dismissed)
        {
            other.dismiss();
        }

        template <typename J>
        static void safeExecute(J& j) throw()
        {
            if (!j.m_dismissed)
#ifdef BLOCXX_HAVE_EXCEPTIONS
                try
                {
#endif
                    j.execute();
#ifdef BLOCXX_HAVE_EXCEPTIONS
                }
                catch(...)
                {}
#endif
        }

        mutable bool m_dismissed;

    public:
        ScopeGuardImplBase() throw() : m_dismissed(false)
        {}

        void dismiss() const throw()
        {
            m_dismissed = true;
        }
    };

    ////////////////////////////////////////////////////////////////
    ///
    /// \typedef typedef const ScopeGuardImplBase& ScopeGuard
    ///
    /// See Andrei's and Petru Marginean's CUJ article
    /// http://www.cuj.com/documents/s=8000/cujcexp1812alexandr/alexandr.htm
    ///
    /// Changes to the original code by Joshua Lehrer:
    /// http://www.lehrerfamily.com/scopeguard.html
    ////////////////////////////////////////////////////////////////

    typedef const ScopeGuardImplBase& ScopeGuard;

    template <typename F>
    class ScopeGuardImpl0 : public ScopeGuardImplBase
    {
    public:
        static ScopeGuardImpl0<F> makeGuard(F fun)
        {
            return ScopeGuardImpl0<F>(fun);
        }

        ~ScopeGuardImpl0() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            m_fun();
        }

    protected:
        ScopeGuardImpl0(F fun) : m_fun(fun)
        {}

        F m_fun;
    };

    template <typename F>
    inline ScopeGuardImpl0<F> makeGuard(F fun)
    {
        return ScopeGuardImpl0<F>::makeGuard(fun);
    }

    template <typename F, typename P1>
    class ScopeGuardImpl1 : public ScopeGuardImplBase
    {
    public:
        static ScopeGuardImpl1<F, P1> makeGuard(F fun, P1 p1)
        {
            return ScopeGuardImpl1<F, P1>(fun, p1);
        }

        ~ScopeGuardImpl1() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            m_fun(m_p1);
        }

    protected:
        ScopeGuardImpl1(F fun, P1 p1) : m_fun(fun), m_p1(p1)
        {}

        F m_fun;
        const P1 m_p1;
    };

    template <typename F, typename P1>
    inline ScopeGuardImpl1<F, P1> makeGuard(F fun, P1 p1)
    {
        return ScopeGuardImpl1<F, P1>::makeGuard(fun, p1);
    }

    template <typename F, typename P1, typename P2>
    class ScopeGuardImpl2: public ScopeGuardImplBase
    {
    public:
        static ScopeGuardImpl2<F, P1, P2> makeGuard(F fun, P1 p1, P2 p2)
        {
            return ScopeGuardImpl2<F, P1, P2>(fun, p1, p2);
        }

        ~ScopeGuardImpl2() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            m_fun(m_p1, m_p2);
        }

    protected:
        ScopeGuardImpl2(F fun, P1 p1, P2 p2) : m_fun(fun), m_p1(p1), m_p2(p2)
        {}

        F m_fun;
        const P1 m_p1;
        const P2 m_p2;
    };

    template <typename F, typename P1, typename P2>
    inline ScopeGuardImpl2<F, P1, P2> makeGuard(F fun, P1 p1, P2 p2)
    {
        return ScopeGuardImpl2<F, P1, P2>::makeGuard(fun, p1, p2);
    }

    template <typename F, typename P1, typename P2, typename P3>
    class ScopeGuardImpl3 : public ScopeGuardImplBase
    {
    public:
        static ScopeGuardImpl3<F, P1, P2, P3> makeGuard(F fun, P1 p1, P2 p2, P3 p3)
        {
            return ScopeGuardImpl3<F, P1, P2, P3>(fun, p1, p2, p3);
        }

        ~ScopeGuardImpl3() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            m_fun(m_p1, m_p2, m_p3);
        }

    protected:
        ScopeGuardImpl3(F fun, P1 p1, P2 p2, P3 p3) : m_fun(fun), m_p1(p1), m_p2(p2), m_p3(p3)
        {}

        F m_fun;
        const P1 m_p1;
        const P2 m_p2;
        const P3 m_p3;
    };

    template <typename F, typename P1, typename P2, typename P3>
    inline ScopeGuardImpl3<F, P1, P2, P3> makeGuard(F fun, P1 p1, P2 p2, P3 p3)
    {
        return ScopeGuardImpl3<F, P1, P2, P3>::makeGuard(fun, p1, p2, p3);
    }

    //************************************************************

    template <class Obj, typename MemFun>
    class ObjScopeGuardImpl0 : public ScopeGuardImplBase
    {
    public:
        static ObjScopeGuardImpl0<Obj, MemFun> makeObjGuard(Obj& obj, MemFun memFun)
        {
            return ObjScopeGuardImpl0<Obj, MemFun>(obj, memFun);
        }

        ~ObjScopeGuardImpl0() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            (m_obj.*m_memFun)();
        }

    protected:
        ObjScopeGuardImpl0(Obj& obj, MemFun memFun) : m_obj(obj), m_memFun(memFun)
        {}

        Obj& m_obj;
        MemFun m_memFun;
    };

    template <class Obj, typename MemFun>
    inline ObjScopeGuardImpl0<Obj, MemFun> makeObjGuard(Obj& obj, MemFun memFun)
    {
        return ObjScopeGuardImpl0<Obj, MemFun>::makeObjGuard(obj, memFun);
    }

    template <typename Ret, class Obj1, class Obj2>
    inline ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()> makeGuard(Ret(Obj2::*memFun)(), Obj1 &obj)
    {
      return ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()>::makeObjGuard(obj,memFun);
    }

    template <typename Ret, class Obj1, class Obj2>
    inline ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()> makeGuard(Ret(Obj2::*memFun)(), Obj1 *obj)
    {
      return ObjScopeGuardImpl0<Obj1,Ret(Obj2::*)()>::makeObjGuard(*obj,memFun);
    }

    template <class Obj, typename MemFun, typename P1>
    class ObjScopeGuardImpl1 : public ScopeGuardImplBase
    {
    public:
        static ObjScopeGuardImpl1<Obj, MemFun, P1> makeObjGuard(Obj& obj, MemFun memFun, P1 p1)
        {
            return ObjScopeGuardImpl1<Obj, MemFun, P1>(obj, memFun, p1);
        }

        ~ObjScopeGuardImpl1() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            (m_obj.*m_memFun)(m_p1);
        }

    protected:
        ObjScopeGuardImpl1(Obj& obj, MemFun memFun, P1 p1) : m_obj(obj), m_memFun(memFun), m_p1(p1)
        {}

        Obj& m_obj;
        MemFun m_memFun;
        const P1 m_p1;
    };

    template <class Obj, typename MemFun, typename P1>
    inline ObjScopeGuardImpl1<Obj, MemFun, P1> makeObjGuard(Obj& obj, MemFun memFun, P1 p1)
    {
        return ObjScopeGuardImpl1<Obj, MemFun, P1>::makeObjGuard(obj, memFun, p1);
    }

    template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
    inline ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b> makeGuard(Ret(Obj2::*memFun)(P1a), Obj1 &obj, P1b p1)
    {
      return ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b>::makeObjGuard(obj,memFun,p1);
    }

    template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b>
    inline ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b> makeGuard(Ret(Obj2::*memFun)(P1a), Obj1 *obj, P1b p1)
    {
      return ObjScopeGuardImpl1<Obj1,Ret(Obj2::*)(P1a),P1b>::makeObjGuard(*obj,memFun,p1);
    }

    template <class Obj, typename MemFun, typename P1, typename P2>
    class ObjScopeGuardImpl2 : public ScopeGuardImplBase
    {
    public:
        static ObjScopeGuardImpl2<Obj, MemFun, P1, P2> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, P2 p2)
        {
            return ObjScopeGuardImpl2<Obj, MemFun, P1, P2>(obj, memFun, p1, p2);
        }

        ~ObjScopeGuardImpl2() throw()
        {
            safeExecute(*this);
        }

        void execute()
        {
            (m_obj.*m_memFun)(m_p1, m_p2);
        }

    protected:
        ObjScopeGuardImpl2(Obj& obj, MemFun memFun, P1 p1, P2 p2) : m_obj(obj), m_memFun(memFun), m_p1(p1), m_p2(p2)
        {}

        Obj& m_obj;
        MemFun m_memFun;
        const P1 m_p1;
        const P2 m_p2;
    };

    template <class Obj, typename MemFun, typename P1, typename P2>
    inline ObjScopeGuardImpl2<Obj, MemFun, P1, P2> makeObjGuard(Obj& obj, MemFun memFun, P1 p1, P2 p2)
    {
        return ObjScopeGuardImpl2<Obj, MemFun, P1, P2>::makeObjGuard(obj, memFun, p1, p2);
    }

    template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, typename P2b>
    inline ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b> makeGuard(Ret(Obj2::*memFun)(P1a,P2a), Obj1 &obj, P1b p1, P2b p2)
    {
      return ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b>::makeObjGuard(obj,memFun,p1,p2);
    }

    template <typename Ret, class Obj1, class Obj2, typename P1a, typename P1b, typename P2a, typename P2b>
    inline ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b> makeGuard(Ret(Obj2::*memFun)(P1a,P2a), Obj1 *obj, P1b p1, P2b p2)
    {
      return ObjScopeGuardImpl2<Obj1,Ret(Obj2::*)(P1a,P2a),P1b,P2b>::makeObjGuard(*obj,memFun,p1,p2);
    }

} // namespace BLOCXX_NAMESPACE

#define BLOCXX_CONCATENATE_DIRECT(s1, s2)  s1##s2
#define BLOCXX_CONCATENATE(s1, s2)         BLOCXX_CONCATENATE_DIRECT(s1, s2)
#define BLOCXX_ANONYMOUS_VARIABLE(str)     BLOCXX_CONCATENATE(str, __LINE__)

#define BLOCXX_ON_BLOCK_EXIT      blocxx::ScopeGuard BLOCXX_ANONYMOUS_VARIABLE(scopeGuard) = blocxx::makeGuard
#define BLOCXX_ON_BLOCK_EXIT_OBJ  blocxx::ScopeGuard BLOCXX_ANONYMOUS_VARIABLE(scopeGuard) = blocxx::makeObjGuard

#endif

