/**
 * Function.hpp
 *
 * Copyright (C) 2010 Kevin Harris
 */

#if !defined(BLOCXX_Function_HPP_INCLUDED_)
#define BLOCXX_Function_HPP_INCLUDED_

// This whole file makes the assumption that a Function returning void
// can have an explicit return statement which evaluates to void:
// void bar() { }
// void foo() { return bar(); }
//
// This means certain Microsoft compilers are excluded.

#include "miniblocxx/BLOCXX_config.h"
#include "miniblocxx/IntrusiveCountableBase.hpp"
#include "miniblocxx/IntrusiveReference.hpp"

// TODO: Find some way to adapt const operator() so it can be applied to non-const objects.

namespace BLOCXX_NAMESPACE
{
	// Base classes to allow multiple behaviors
	template <typename Ret>
	class callable0 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable0() { }
		Ret operator()() { return call(); }
	protected:
		virtual Ret call() = 0;
	};
	template <typename Ret, typename A1>
	class callable1 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable1() { }
		Ret operator()(A1 a1) { return call(a1); }
	protected:
		virtual Ret call(A1 a1) = 0;
	};
	template <typename Ret, typename A1, typename A2>
	class callable2 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable2() { }
		Ret operator()(A1 a1, A2 a2) { return call(a1,a2); }
	protected:
		virtual Ret call(A1 a1, A2 a2) = 0;
	};
	template <typename Ret, typename A1, typename A2, typename A3>
	class callable3 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable3() { }
		Ret operator()(A1 a1, A2 a2, A3 a3) { return call(a1,a2,a3); }
	protected:
		virtual Ret call(A1 a1, A2 a2, A3 a3) = 0;
	};
	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	class callable4 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable4() { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4) { return call(a1,a2,a3,a4); }
	protected:
		virtual Ret call(A1 a1, A2 a2, A3 a3, A4 a4) = 0;
	};
	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	class callable5 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable5() { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) { return call(a1,a2,a3,a4,a5); }
	protected:
		virtual Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) = 0;
	};
	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	class callable6 : public blocxx::IntrusiveCountableBase
	{
	public:
		virtual ~callable6() { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) { return call(a1,a2,a3,a4,a5,a6); }
	protected:
		virtual Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) = 0;
	};

	// Wrappers to allow simple storage/calling
	template <typename Ret>
	class Function0
	{
	public:
		typedef callable0<Ret> base;
		Function0() { }
		explicit Function0(base* fp) : m_fp(fp) { }
		Ret operator()() { return m_fp ? (*m_fp)() : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function0& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)());
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1>
	class Function1
	{
	public:
		typedef callable1<Ret,A1> base;
		Function1() { }
		explicit Function1(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1) { return m_fp ? (*m_fp)(a1) : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function1& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1, typename A2>
	class Function2
	{
	public:
		typedef callable2<Ret,A1,A2> base;
		Function2() { }
		explicit Function2(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1, A2 a2) { return m_fp ? (*m_fp)(a1,a2) : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function2& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1,A2));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1, typename A2, typename A3>
	class Function3
	{
	public:
		typedef callable3<Ret,A1,A2,A3> base;
		Function3() { }
		explicit Function3(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1, A2 a2, A3 a3) { return m_fp ? (*m_fp)(a1,a2,a3) : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function3& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1,A2,A3));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	class Function4
	{
	public:
		typedef callable4<Ret,A1,A2,A3,A4> base;
		Function4() { }
		explicit Function4(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4) { return m_fp ? (*m_fp)(a1,a2,a3,a4) : Ret(); }
		template<typename T> void connect(const T& t);
		template<typename T> void connect(T& t);
		void connect(const Function4& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1,A2,A3,A4));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	class Function5
	{
	public:
		typedef callable5<Ret,A1,A2,A3,A4,A5> base;
		Function5() { }
		explicit Function5(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) { return m_fp ? (*m_fp)(a1,a2,a3,a4,a5) : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function5& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1,A2,A3,A4,A5));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	class Function6
	{
	public:
		typedef callable6<Ret,A1,A2,A3,A4,A5,A6> base;
		Function6() { }
		explicit Function6(base* fp) : m_fp(fp) { }
		Ret operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) { return m_fp ? (*m_fp)(a1,a2,a3,a4,a5,a6) : Ret(); }
		template<typename T> void connect(T& t);
		template<typename T> void connect(const T& t);
		void connect(const Function6& fn) { m_fp = fn.m_fp; }
		void connect(Ret (*fn)(A1,A2,A3,A4,A5,A6));
	private:
		blocxx::IntrusiveReference<base> m_fp;
	};

	//
	// Function pointer objects
	//
	template <typename Ret>
	class ptrfun0 : public callable0<Ret>
	{
	public:
		typedef Ret (*prototype)();
		ptrfun0(prototype f): m_fun(f) { }
	protected:
		Ret call() { return m_fun(); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1>
	class ptrfun1 : public callable1<Ret,A1>
	{
	public:
		typedef Ret (*prototype)(A1);
		ptrfun1(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1) { return m_fun(a1); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1, typename A2>
	class ptrfun2 : public callable2<Ret,A1,A2>
	{
	public:
		typedef Ret (*prototype)(A1,A2);
		ptrfun2(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2) { return m_fun(a1, a2); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1, typename A2, typename A3>
	class ptrfun3 : public callable3<Ret,A1,A2,A3>
	{
	public:
		typedef Ret (*prototype)(A1,A2,A3);
		ptrfun3(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3) { return m_fun(a1,a2,a3); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	class ptrfun4 : public callable4<Ret,A1,A2,A3,A4>
	{
	public:
		typedef Ret (*prototype)(A1,A2,A3,A4);
		ptrfun4(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4) { return m_fun(a1,a2,a3,a4); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	class ptrfun5 : public callable5<Ret,A1,A2,A3,A4,A5>
	{
	public:
		typedef Ret (*prototype)(A1,A2,A3,A4,A5);
		ptrfun5(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) { return m_fun(a1,a2,a3,a4,a5); }
	private:
		prototype m_fun;
	};

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	class ptrfun6 : public callable6<Ret,A1,A2,A3,A4,A5,A6>
	{
	public:
		typedef Ret (*prototype)(A1,A2,A3,A4,A5,A6);
		ptrfun6(prototype f): m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) { return m_fun(a1,a2,a3,a4,a5,a6); }
	private:
		prototype m_fun;
	};


	//
	// Member Function pointer objects.  There is probably some way to collapse
	// the non-const and const versions.
	//

	// Non-const
	template <typename Class, typename Ret>
	class memfun0 : public callable0<Ret>
	{
	public:
		typedef Ret (Class::*prototype)();
		memfun0(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call() { return (m_inst.*m_fun)(); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1>
	class memfun1 : public callable1<Ret,A1>
	{
	public:
		typedef Ret (Class::*prototype)(A1);
		memfun1(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1) { return (m_inst.*m_fun)(a1); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2>
	class memfun2 : public callable2<Ret,A1,A2>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2);
		memfun2(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2) { return (m_inst.*m_fun)(a1,a2); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3>
	class memfun3 : public callable3<Ret,A1,A2,A3>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3);
		memfun3(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3) { return (m_inst.*m_fun)(a1,a2,a3); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4>
	class memfun4 : public callable4<Ret,A1,A2,A3,A4>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4);
		memfun4(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4) { return (m_inst.*m_fun)(a1,a2,a3,a4); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	class memfun5 : public callable5<Ret,A1,A2,A3,A4,A5>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4,A5);
		memfun5(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) { return (m_inst.*m_fun)(a1,a2,a3,a4,a5); }
	private:
		Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	class memfun6 : public callable6<Ret,A1,A2,A3,A4,A5,A6>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4,A5,A6);
		memfun6(Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) { return (m_inst.*m_fun)(a1,a2,a3,a4,a5,a6); }
	private:
		Class& m_inst;
		prototype m_fun;
	};


	// Const
	template <typename Class, typename Ret>
	class memfun0_const : public callable0<Ret>
	{
	public:
		typedef Ret (Class::*prototype)() const;
		memfun0_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call() { return (m_inst.*m_fun)(); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1>
	class memfun1_const : public callable1<Ret,A1>
	{
	public:
		typedef Ret (Class::*prototype)(A1) const;
		memfun1_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1) { return (m_inst.*m_fun)(a1); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2>
	class memfun2_const : public callable2<Ret,A1,A2>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2) const;
		memfun2_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2) { return (m_inst.*m_fun)(a1,a2); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3>
	class memfun3_const : public callable3<Ret,A1,A2,A3>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3) const;
		memfun3_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3) { return (m_inst.*m_fun)(a1,a2,a3); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4>
	class memfun4_const : public callable4<Ret,A1,A2,A3,A4>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4) const;
		memfun4_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4) { return (m_inst.*m_fun)(a1,a2,a3,a4); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	class memfun5_const : public callable5<Ret,A1,A2,A3,A4,A5>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4,A5) const;
		memfun5_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) { return (m_inst.*m_fun)(a1,a2,a3,a4,a5); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	class memfun6_const : public callable6<Ret,A1,A2,A3,A4,A5,A6>
	{
	public:
		typedef Ret (Class::*prototype)(A1,A2,A3,A4,A5,A6) const;
		memfun6_const(const Class& c, prototype f): m_inst(c), m_fun(f) { }
	protected:
		Ret call(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) { return (m_inst.*m_fun)(a1,a2,a3,a4,a5,a6); }
	private:
		const Class& m_inst;
		prototype m_fun;
	};

	//
	// Functions to simplify creation of the above.
	//

	//
	// Free Function pointer adapters
	//
	template <typename Ret>
	Function0<Ret> ptrfun(Ret (*p)())
	{
		return Function0<Ret>(new ptrfun0<Ret>(p));
	}

	template <typename Ret, typename A1>
	Function1<Ret,A1> ptrfun(Ret (*p)(A1))
	{
		return Function1<Ret,A1>(new ptrfun1<Ret,A1>(p));
	}

	template <typename Ret, typename A1, typename A2>
	Function2<Ret,A1,A2> ptrfun(Ret (*p)(A1,A2))
	{
		return Function2<Ret,A1,A2>(new ptrfun2<Ret,A1,A2>(p));
	}

	template <typename Ret, typename A1, typename A2, typename A3>
	Function3<Ret,A1,A2,A3> ptrfun(Ret (*p)(A1,A2,A3))
	{
		return Function3<Ret,A1,A2,A3>(new ptrfun3<Ret,A1,A2,A3>(p));
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	Function4<Ret,A1,A2,A3,A4> ptrfun(Ret (*p)(A1,A2,A3,A4))
	{
		return Function4<Ret,A1,A2,A3,A4>(new ptrfun4<Ret,A1,A2,A3,A4>(p));
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	Function5<Ret,A1,A2,A3,A4,A5> ptrfun(Ret (*p)(A1,A2,A3,A4,A5))
	{
		return Function5<Ret,A1,A2,A3,A4,A5>(new ptrfun5<Ret,A1,A2,A3,A4,A5>(p));
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	Function6<Ret,A1,A2,A3,A4,A5,A6> ptrfun(Ret (*p)(A1,A2,A3,A4,A5,A6))
	{
		return Function6<Ret,A1,A2,A3,A4,A5,A6>(new ptrfun6<Ret,A1,A2,A3,A4,A5,A6>(p));
	}

	//
	// Member Function pointer adapters
	//
	template <typename Class, typename Ret>
	Function0<Ret> memfun(Class& c, Ret (Class::*p)())
	{
		return Function0<Ret>(new memfun0<Class,Ret>(c,p));
	}
	template <typename Class, typename Ret>
	Function0<Ret> memfun(const Class& c, Ret (Class::*p)() const)
	{
		return Function0<Ret>(new memfun0_const<Class,Ret>(c,p));
	}
	template <typename Class, typename Ret>
	Function0<Ret> memfun(Class* c, Ret (Class::*p)())
	{
		return Function0<Ret>(new memfun0<Class,Ret>(*c,p));
	}

	template <typename Class, typename Ret, typename A1>
	Function1<Ret,A1> memfun(Class& c, Ret (Class::*p)(A1))
	{
		return Function1<Ret,A1>(new memfun1<Class,Ret,A1>(c,p));
	}
	template <typename Class, typename Ret, typename A1>
	Function1<Ret,A1> memfun(const Class& c, Ret (Class::*p)(A1) const)
	{
		return Function1<Ret,A1>(new memfun1_const<Class,Ret,A1>(c,p));
	}
	template <typename Class, typename Ret, typename A1>
	Function1<Ret,A1> memfun(Class* c, Ret (Class::*p)(A1))
	{
		return Function1<Ret,A1>(new memfun1<Class,Ret,A1>(*c,p));
	}

	template <typename Class, typename Ret, typename A1, typename A2>
	Function2<Ret,A1,A2> memfun(Class& c, Ret (Class::*p)(A1,A2))
	{
		return Function2<Ret,A1,A2>(new memfun2<Class,Ret,A1,A2>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2>
	Function2<Ret,A1,A2> memfun(const Class& c, Ret (Class::*p)(A1,A2) const)
	{
		return Function2<Ret,A1,A2>(new memfun2_const<Class,Ret,A1,A2>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2>
	Function2<Ret,A1,A2> memfun(Class* c, Ret (Class::*p)(A1,A2))
	{
		return Function2<Ret,A1,A2>(new memfun2<Class,Ret,A1,A2>(*c,p));
	}

	template <typename Class, typename Ret, typename A1, typename A2, typename A3>
	Function3<Ret,A1,A2,A3> memfun(Class& c, Ret (Class::*p)(A1,A2,A3))
	{
		return Function3<Ret,A1,A2,A3>(new memfun3<Class,Ret,A1,A2,A3>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3>
	Function3<Ret,A1,A2,A3> memfun(const Class& c, Ret (Class::*p)(A1,A2,A3) const)
	{
		return Function3<Ret,A1,A2,A3>(new memfun3_const<Class,Ret,A1,A2,A3>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3>
	Function3<Ret,A1,A2,A3> memfun(Class* c, Ret (Class::*p)(A1,A2,A3))
	{
		return Function3<Ret,A1,A2,A3>(new memfun3<Class,Ret,A1,A2,A3>(*c,p));
	}

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4>
	Function4<Ret,A1,A2,A3,A4> memfun(Class& c, Ret (Class::*p)(A1,A2,A3,A4))
	{
		return Function4<Ret,A1,A2,A3,A4>(new memfun4<Class,Ret,A1,A2,A3,A4>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4>
	Function4<Ret,A1,A2,A3,A4> memfun(const Class& c, Ret (Class::*p)(A1,A2,A3,A4) const)
	{
		return Function4<Ret,A1,A2,A3,A4>(new memfun4_const<Class,Ret,A1,A2,A3,A4>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4>
	Function4<Ret,A1,A2,A3,A4> memfun(Class* c, Ret (Class::*p)(A1,A2,A3,A4))
	{
		return Function4<Ret,A1,A2,A3,A4>(new memfun4<Class,Ret,A1,A2,A3,A4>(*c,p));
	}

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	Function5<Ret,A1,A2,A3,A4,A5> memfun(Class& c, Ret (Class::*p)(A1,A2,A3,A4,A5))
	{
		return Function5<Ret,A1,A2,A3,A4,A5>(new memfun5<Class,Ret,A1,A2,A3,A4,A5>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	Function5<Ret,A1,A2,A3,A4,A5> memfun(const Class& c, Ret (Class::*p)(A1,A2,A3,A4,A5) const)
	{
		return Function5<Ret,A1,A2,A3,A4,A5>(new memfun5_const<Class,Ret,A1,A2,A3,A4,A5>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	Function5<Ret,A1,A2,A3,A4,A5> memfun(Class* c, Ret (Class::*p)(A1,A2,A3,A4,A5))
	{
		return Function5<Ret,A1,A2,A3,A4,A5>(new memfun5<Class,Ret,A1,A2,A3,A4,A5>(*c,p));
	}

	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	Function6<Ret,A1,A2,A3,A4,A5,A6> memfun(Class& c, Ret (Class::*p)(A1,A2,A3,A4,A5,A6))
	{
		return Function6<Ret,A1,A2,A3,A4,A5,A6>(new memfun6<Class,Ret,A1,A2,A3,A4,A5,A6>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	Function6<Ret,A1,A2,A3,A4,A5,A6> memfun(const Class& c, Ret (Class::*p)(A1,A2,A3,A4,A5,A6) const)
	{
		return Function6<Ret,A1,A2,A3,A4,A5,A6>(new memfun6_const<Class,Ret,A1,A2,A3,A4,A5,A6>(c,p));
	}
	template <typename Class, typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	Function6<Ret,A1,A2,A3,A4,A5,A6> memfun(Class* c, Ret (Class::*p)(A1,A2,A3,A4,A5,A6))
	{
		return Function6<Ret,A1,A2,A3,A4,A5,A6>(new memfun6<Class,Ret,A1,A2,A3,A4,A5,A6>(*c,p));
	}



	//
	// Connector Functions
	//
	template <typename Ret>
	template<typename T>
	void Function0<Ret>::connect(T& t)
	{
		typedef Ret (T::*op)();
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret>
	template<typename T>
	void Function0<Ret>::connect(const T& t)
	{
		typedef Ret (T::*op)() const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret>
	void Function0<Ret>::connect(Ret (*fn)())
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1>
	template<typename T>
	void Function1<Ret,A1>::connect(T& t)
	{
		typedef Ret (T::*op)(A1);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1>
	template<typename T>
	void Function1<Ret,A1>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1>
	void Function1<Ret,A1>::connect(Ret (*fn)(A1))
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1, typename A2>
	template<typename T>
	void Function2<Ret,A1,A2>::connect(T& t)
	{
		typedef Ret (T::*op)(A1,A2);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2>
	template<typename T>
	void Function2<Ret,A1,A2>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1,A2) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2>
	void Function2<Ret,A1,A2>::connect(Ret (*fn)(A1,A2))
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3>
	template<typename T>
	void Function3<Ret,A1,A2,A3>::connect(T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3>
	template<typename T>
	void Function3<Ret,A1,A2,A3>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3>
	void Function3<Ret,A1,A2,A3>::connect(Ret (*fn)(A1,A2,A3))
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	template<typename T>
	void Function4<Ret,A1,A2,A3,A4>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	template<typename T>
	void Function4<Ret,A1,A2,A3,A4>::connect(T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4>
	void Function4<Ret,A1,A2,A3,A4>::connect(Ret (*fn)(A1,A2,A3,A4))
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	template<typename T>
	void Function5<Ret,A1,A2,A3,A4,A5>::connect(T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4,A5);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	template<typename T>
	void Function5<Ret,A1,A2,A3,A4,A5>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4,A5) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5>
	void Function5<Ret,A1,A2,A3,A4,A5>::connect(Ret (*fn)(A1,A2,A3,A4,A5))
	{
		m_fp = ptrfun(fn).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	template<typename T>
	void Function6<Ret,A1,A2,A3,A4,A5,A6>::connect(T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4,A5,A6);
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	template<typename T>
	void Function6<Ret,A1,A2,A3,A4,A5,A6>::connect(const T& t)
	{
		typedef Ret (T::*op)(A1,A2,A3,A4,A5,A6) const;
		op o = &T::operator();
		m_fp = memfun(t, o).m_fp;
	}

	template <typename Ret, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	void Function6<Ret,A1,A2,A3,A4,A5,A6>::connect(Ret (*fn)(A1,A2,A3,A4,A5,A6))
	{
		m_fp = ptrfun(fn).m_fp;
	}

}

#endif
