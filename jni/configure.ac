VERSION=0.00.00
AC_INIT(redneckracer, 0.00.00, mail@example.com)
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(redneckracer, 0.00.00)
AM_CONFIG_HEADER(config.hpp)

AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_PROG_CXX
AC_LANG(C++)

AC_PATH_XTRA
if test -n "$no_x"; then
	AC_MSG_ERROR([Can't find X, you need X!])
	USE_X=false
else
	USE_X=true
fi
AC_SUBST(X_INCLUDE_DIRS)
AC_SUBST(X_LIB_DIRS)

AC_ARG_ENABLE([debug-mode], AS_HELP_STRING([--enable-debug-mode], [enable debug mode]))
if test "x${enable_debug_mode:+set}" = xset; then
	CFLAGS="${CFLAGS} -DDEBUG"
	CXXFLAGS="${CPPLAGS} -DDEBUG"
fi


PKG_CHECK_MODULES([freetype], [freetype2], [HAVE_FREETYPE=1], [HAVE_FREETYPE=0])
AM_CONDITIONAL(HAVE_FREETYPE, test x$HAVE_FREETYPE = x1)

PKG_CHECK_MODULES([png], [libpng], [HAVE_PNG=1], [HAVE_PNG=0])
AM_CONDITIONAL(HAVE_PNG, test x$HAVE_PNG = x1)

AX_CHECK_GL

CXXFLAGS="${CXXFLAGS} -Wall"
CFLAGS="${CFLAGS} -Wall"

AC_OUTPUT(
	[
		Makefile
		graphlib/Makefile
		miniblocxx/Makefile
		engine/Makefile
		game/Makefile
		CppUnit/Makefile
		test/Makefile
		test/gmock/Makefile
		libzip/Makefile
		libpng/Makefile
		openal/Makefile
	],
	[
		echo "------------------"
		echo "Configure is done."
		echo "------------------"
	]
)
