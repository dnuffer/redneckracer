/* NOT automatically generated for windows */
#ifndef __SRC_COMMON_BLOCXX_WINDOWS_CONFIG_H
#define __SRC_COMMON_BLOCXX_WINDOWS_CONFIG_H 1

#if !defined(_MT)
#error The multithreaded C run-time library is required!
#endif

/* The system has OpenSSL headers and libs */
#ifndef BLOCXX_HAVE_OPENSSL 
#define BLOCXX_HAVE_OPENSSL  1 
#endif

#undef BLOCXX_HAVE_ISINF

/* The system doesn't have OpenSSL */
#undef BLOCXX_NO_SSL

#ifndef BLOCXX_DEFAULT_INSTALLDIR
#define BLOCXX_DEFAULT_INSTALLDIR "C:\\Program Files\\Vintela\\VSM"
#endif

#ifndef BLOCXX_DEFAULT_SYSCONF_DIR 
//#define BLOCXX_DEFAULT_SYSCONF_DIR  "/usr/local/etc" 
#define BLOCXX_DEFAULT_SYSCONF_DIR  BLOCXX_DEFAULT_INSTALLDIR "\\etc" 
#endif

#ifndef BLOCXX_DEFAULT_STATE_DIR 
//#define BLOCXX_DEFAULT_STATE_DIR  "/usr/local/var" 
#define BLOCXX_DEFAULT_STATE_DIR  BLOCXX_DEFAULT_INSTALLDIR "\\var" 
#endif

#ifndef BLOCXX_DEFAULT_LIB_DIR 
//#define BLOCXX_DEFAULT_LIB_DIR  "/usr/local/lib" 
#define BLOCXX_DEFAULT_LIB_DIR  BLOCXX_DEFAULT_INSTALLDIR "\\lib" 
#endif

#ifndef BLOCXX_DEFAULT_LIBEXEC_DIR 
//#define BLOCXX_DEFAULT_LIBEXEC_DIR  "/usr/local/libexec" 
#define BLOCXX_DEFAULT_LIBEXEC_DIR  BLOCXX_DEFAULT_INSTALLDIR "\\libexec" 
#endif

#ifndef BLOCXX_PIDFILE_DIR 
//#define BLOCXX_PIDFILE_DIR  "/var/run" 
#define BLOCXX_PIDFILE_DIR  BLOCXX_DEFAULT_INSTALLDIR "\\run" 
#endif

/* Either "" or the argument to --with-package-prefix */
#ifndef BLOCXX_PACKAGE_PREFIX 
#define BLOCXX_PACKAGE_PREFIX "" 
#endif

/* Defined if --enable-stack-trace was passed to configure */
/* #undef BLOCXX_ENABLE_STACK_TRACE_ON_EXCEPTIONS */

/* Defined if --enable-debug was passed to configure */
/* #undef BLOCXX_DEBUG */

/* Defined if --enable-profile was passed to configure */
/* #undef BLOCXX_PROFILE */


/* Defined if we want to use custom "new" operator to debug memory leaks */
/* #undef BLOCXX_DEBUG_MEMORY */

/* Define if we want to print "Entering"/"Leaving" <functionName> */
/* #undef BLOCXX_PRINT_FUNC_DEBUG */

/* Define if we want to check for NULL references and throw an exception */
#ifndef BLOCXX_CHECK_NULL_REFERENCES 
#define BLOCXX_CHECK_NULL_REFERENCES  1 
#endif

/* Define if we want to check for valid array indexing and throw an exception */
#ifndef BLOCXX_CHECK_ARRAY_INDEXING 
#define BLOCXX_CHECK_ARRAY_INDEXING  1 
#endif

/* Define if we want to build the perlIFC */
/* #undef BLOCXX_PERLIFC  1 */

/* Define which one is the current platform */
#ifndef BLOCXX_WIN32 
#define BLOCXX_WIN32  1 
#endif

// Shouldn't be looking at this. BLOCXX_WIN32 is enough
#ifndef BLOCXX_USE_WIN32_THREADS
#define BLOCXX_USE_WIN32_THREADS  1
#endif

/* #undef u_int8_t */
/* #undef u_int16_t */
/* #undef u_int32_t */
#ifndef BLOCXX_HAVE_SOCKLEN_T 
#define BLOCXX_HAVE_SOCKLEN_T  1 
#endif

/* #undef BLOCXX_HAVE_PTHREAD_MUTEXATTR_SETTYPE */
/* #undef BLOCXX_HAVE_PTHREAD_SPIN_LOCK  1 */
/* #undef BLOCXX_HAVE_PAM  1 */

/* #undef BLOCXX_DISABLE_DIGEST */
/* #undef BLOCXX_DISABLE_ASSOCIATION_TRAVERSAL */
/* #undef BLOCXX_DISABLE_QUALIFIER_DECLARATION */
/* #undef BLOCXX_DISABLE_SCHEMA_MANIPULATION */
/* #undef BLOCXX_DISABLE_INSTANCE_MANIPULATION */


/* Define to 1 if you have the `backtrace' function. */
/* #undef BLOCXX_HAVE_BACKTRACE  1 */

/* Define to 1 if you have the <byteswap.h> header file. */
/* #undef BLOCXX_HAVE_BYTESWAP_H  1 */

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef BLOCXX_HAVE_DOPRNT */

/* Define to 1 if you have the <ext/hash_map> header file. */
/* #ifndef BLOCXX_HAVE_EXT_HASH_MAP 
#define BLOCXX_HAVE_EXT_HASH_MAP  1 
#endif */

/* Define to 1 if you have the <fcntl.h> header file. */
/* #ifndef BLOCXX_HAVE_FCNTL_H 
#define BLOCXX_HAVE_FCNTL_H  1 
#endif */

/* Define to 1 if you have the `gethostbyname_r' function. */
/* #ifndef BLOCXX_HAVE_GETHOSTBYNAME_R 
#define BLOCXX_HAVE_GETHOSTBYNAME_R  1 
#endif */

/* Define to 1 if you have the <getopt.h> header file. */
/* #ifndef BLOCXX_HAVE_GETOPT_H 
#define BLOCXX_HAVE_GETOPT_H  1 
#endif */

/* Define to 1 if you have the `getopt_long' function. */
/* #ifndef BLOCXX_HAVE_GETOPT_LONG 
#define BLOCXX_HAVE_GETOPT_LONG  1 
#endif */

/* Define to 1 if you have the `getpagesize' function. */
/* #ifndef BLOCXX_HAVE_GETPAGESIZE 
#define BLOCXX_HAVE_GETPAGESIZE  1 
#endif */

/* Define to 1 if you have the <hash_map> header file. */
/* #undef BLOCXX_HAVE_HASH_MAP */

/* Define to 1 if you have the <inttypes.h> header file. */
//#ifndef BLOCXX_HAVE_INTTYPES_H 
//#define BLOCXX_HAVE_INTTYPES_H  1 
//#endif

/* Define to 1 if you have the <istream> header file. */
#ifndef BLOCXX_HAVE_ISTREAM 
#define BLOCXX_HAVE_ISTREAM  1 
#endif

/* Define to 1 if you have the <limits.h> header file. */
#ifndef BLOCXX_HAVE_LIMITS_H 
#define BLOCXX_HAVE_LIMITS_H  1 
#endif

/* Define to 1 if you have the <memory.h> header file. */
#ifndef BLOCXX_HAVE_MEMORY_H 
#define BLOCXX_HAVE_MEMORY_H  1 
#endif

/* Define to 1 if you have a working `mmap' system call. */
/* #ifndef BLOCXX_HAVE_MMAP 
#define BLOCXX_HAVE_MMAP  1 
#endif */

/* Define to 1 if you have the `nanosleep' function. */
/* #ifndef BLOCXX_HAVE_NANOSLEEP 
#define BLOCXX_HAVE_NANOSLEEP  1 
#endif */

/* Define to 1 if you have the <ostream> header file. */
#ifndef BLOCXX_HAVE_OSTREAM 
#define BLOCXX_HAVE_OSTREAM  1 
#endif

/* Define to 1 if you have the <ostream.h> header file. */
/* #ifndef BLOCXX_HAVE_OSTREAM_H 
#define BLOCXX_HAVE_OSTREAM_H  1 
#endif */

/* Define to 1 if you have the <pthread.h> header file. */
/* #ifndef BLOCXX_HAVE_PTHREAD_H 
#define BLOCXX_HAVE_PTHREAD_H  1 
#endif */

/* Define to 1 if you have the `sched_yield' function. */
/* #ifndef BLOCXX_HAVE_SCHED_YIELD 
#define BLOCXX_HAVE_SCHED_YIELD  1 
#endif */

/* Define to 1 if you have the <security/pam_appl.h> header file. */
/* #ifndef BLOCXX_HAVE_SECURITY_PAM_APPL_H 
#define BLOCXX_HAVE_SECURITY_PAM_APPL_H  1 
#endif */

/* Define to 1 if you have the <security/pam_misc.h> header file. */
/* #ifndef BLOCXX_HAVE_SECURITY_PAM_MISC_H 
#define BLOCXX_HAVE_SECURITY_PAM_MISC_H  1 
#endif */

/* Define to 1 if you have the <slp.h> header file. */
//#ifndef BLOCXX_HAVE_SLP_H 
//#define BLOCXX_HAVE_SLP_H  1 
//#endif

/* Define to 1 if you have the <stdint.h> header file. */
#ifndef BLOCXX_HAVE_STDINT_H 
#define BLOCXX_HAVE_STDINT_H  1 
#endif

/* Define to 1 if you have the <stdlib.h> header file. */
#ifndef BLOCXX_HAVE_STDLIB_H 
#define BLOCXX_HAVE_STDLIB_H  1 
#endif

/* Define to 1 if you have the <streambuf> header file. */
#ifndef BLOCXX_HAVE_STREAMBUF 
#define BLOCXX_HAVE_STREAMBUF  1 
#endif

/* Define to 1 if you have the <streambuf.h> header file. */
/* #ifndef BLOCXX_HAVE_STREAMBUF_H 
#define BLOCXX_HAVE_STREAMBUF_H  1 
#endif */

/* Define to 1 if you have the `strerror' function. */
#ifndef BLOCXX_HAVE_STRERROR 
#define BLOCXX_HAVE_STRERROR  1 
#endif

/* Define to 1 if you have the <strings.h> header file. */
/* #ifndef BLOCXX_HAVE_STRINGS_H 
#define BLOCXX_HAVE_STRINGS_H  1 
#endif */

/* Define to 1 if you have the <string.h> header file. */
#ifndef BLOCXX_HAVE_STRING_H 
#define BLOCXX_HAVE_STRING_H  1 
#endif

/* Define to 1 if you have the `strtoll' function. */
/* #ifndef BLOCXX_HAVE_STRTOLL 
#define BLOCXX_HAVE_STRTOLL  1 
#endif */

/* Define to 1 if you have the `strtoull' function. */
/* #ifndef BLOCXX_HAVE_STRTOULL 
#define BLOCXX_HAVE_STRTOULL  1 
#endif */

/* Define to 1 if `st_blksize' is member of `struct stat'. */
/* #ifndef BLOCXX_HAVE_STRUCT_STAT_ST_BLKSIZE 
#define BLOCXX_HAVE_STRUCT_STAT_ST_BLKSIZE  1 
#endif */

/* Define to 1 if your `struct stat' has `st_blksize'. Deprecated, use
   `HAVE_STRUCT_STAT_ST_BLKSIZE' instead. */
/* #ifndef BLOCXX_HAVE_ST_BLKSIZE 
#define BLOCXX_HAVE_ST_BLKSIZE  1 
#endif */

/* Define to 1 if you have the <sys/cdefs.h> header file. */
/* #ifndef BLOCXX_HAVE_SYS_CDEFS_H 
#define BLOCXX_HAVE_SYS_CDEFS_H  1 
#endif */

/* Define to 1 if you have the <sys/file.h> header file. */
/* #ifndef BLOCXX_HAVE_SYS_FILE_H 
#define BLOCXX_HAVE_SYS_FILE_H  1 
#endif */

/* Define to 1 if you have the <sys/int_types.h> header file. */
/* #undef BLOCXX_HAVE_SYS_INT_TYPES_H */

/* Define to 1 if you have the <sys/queue.h> header file. */
/* #ifndef BLOCXX_HAVE_SYS_QUEUE_H 
#define BLOCXX_HAVE_SYS_QUEUE_H  1 
#endif */

/* Define to 1 if you have the <sys/sockio.h> header file. */
/* #undef BLOCXX_HAVE_SYS_SOCKIO_H */

/* Define to 1 if you have the <sys/stat.h> header file. */
#ifndef BLOCXX_HAVE_SYS_STAT_H 
#define BLOCXX_HAVE_SYS_STAT_H  1 
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#ifndef BLOCXX_HAVE_SYS_TYPES_H 
#define BLOCXX_HAVE_SYS_TYPES_H  1 
#endif

/* Define to 1 if you have the <unistd.h> header file. */
/* #ifndef BLOCXX_HAVE_UNISTD_H 
#define BLOCXX_HAVE_UNISTD_H  1 
#endif */

/* Define to 1 if you have the `vprintf' function. */
#ifndef BLOCXX_HAVE_VPRINTF 
#define BLOCXX_HAVE_VPRINTF  1 
#endif

/* Define to 1 if you have the <zlib.h> header file. */
#ifndef BLOCXX_HAVE_ZLIB_H 
#define BLOCXX_HAVE_ZLIB_H  1 
#endif

/* Name of package */
#ifndef BLOCXX_PACKAGE 
#define BLOCXX_PACKAGE  "blocxx" 
#endif

/* Define to the address where bug reports for this package should be sent. */
#ifndef BLOCXX_PACKAGE_BUGREPORT 
#define BLOCXX_PACKAGE_BUGREPORT  "blocxx-devel@lists.sourceforge.net" 
#endif

/* Define to the full name of this package. */
#ifndef BLOCXX_PACKAGE_NAME 
#define BLOCXX_PACKAGE_NAME  "BloCxx" 
#endif

/* Define to the full name and version of this package. */
#ifndef BLOCXX_PACKAGE_STRING 
#define BLOCXX_PACKAGE_STRING  "BloCxx 1.9.1" 
#endif

/* Define to the one symbol short name of this package. */
#ifndef BLOCXX_PACKAGE_TARNAME 
#define BLOCXX_PACKAGE_TARNAME  "blocxx" 
#endif

/* Define to the version of this package. */
#ifndef BLOCXX_PACKAGE_VERSION 
#define BLOCXX_PACKAGE_VERSION  "1.9.1" 
#endif

/* The size of a `char', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_CHAR 
#define BLOCXX_SIZEOF_CHAR  1 
#endif

/* The size of a `char *', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_CHAR_P
#ifndef _WIN64
#define BLOCXX_SIZEOF_CHAR_P  4 
#else
#define BLOCXX_SIZEOF_CHAR_P  8
#endif
#endif

/* The size of a `double', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_DOUBLE 
#define BLOCXX_SIZEOF_DOUBLE  8 
#endif

/* The size of a `float', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_FLOAT 
#define BLOCXX_SIZEOF_FLOAT  4 
#endif

/* The size of a `int', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_INT 
#define BLOCXX_SIZEOF_INT  4 
#endif

/* The size of a `long double', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_LONG_DOUBLE 
#define BLOCXX_SIZEOF_LONG_DOUBLE  8 
#endif

/* The size of a `long int', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_LONG_INT 
#define BLOCXX_SIZEOF_LONG_INT  4 
#endif

/* The size of a `long long int', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_LONG_LONG_INT 
#define BLOCXX_SIZEOF_LONG_LONG_INT  8 
#endif

/* The size of a `short int', as computed by sizeof. */
#ifndef BLOCXX_SIZEOF_SHORT_INT 
#define BLOCXX_SIZEOF_SHORT_INT  2 
#endif

/* Define to 1 if you have the ANSI C header files. */
#ifndef BLOCXX_STDC_HEADERS 
#define BLOCXX_STDC_HEADERS  1 
#endif

/* Define to 1 if `sys_siglist' is declared by <signal.h> or <unistd.h>. */
/* #ifndef BLOCXX_SYS_SIGLIST_DECLARED 
#define BLOCXX_SYS_SIGLIST_DECLARED  1 
#endif */

/* Intel x86 architecture */
#ifndef BLOCXX_ARCH_X86 
#define BLOCXX_ARCH_X86 1 
#endif

/* x86-64 architecture */
#ifndef BLOCXX_ARCH_X86_64 
#define BLOCXX_ARCH_X86_64 1 
#endif

/* Where true is. */
#ifndef BLOCXX_TRUE_PATHNAME 
#define BLOCXX_TRUE_PATHNAME "/bin/true" 
#endif

/* Where false is. */
#ifndef BLOCXX_FALSE_PATHNAME 
#define BLOCXX_FALSE_PATHNAME "/bin/false" 
#endif

/* FIXME: These values are pulled from a 64 bit linux config so I can compile,
figure out what the real values of these should be. */
#ifndef BLOCXX_DATETIME_MAXIMUM_TIME
#define BLOCXX_DATETIME_MAXIMUM_TIME 253402300799ll
#endif
#ifndef BLOCXX_DATETIME_MAXIMUM_YEAR
#define BLOCXX_DATETIME_MAXIMUM_YEAR 9999
#endif
#ifndef BLOCXX_DATETIME_MINIMUM_TIME
#define BLOCXX_DATETIME_MINIMUM_TIME -62167219200ll
#endif
#ifndef BLOCXX_DATETIME_MINIMUM_YEAR
#define BLOCXX_DATETIME_MINIMUM_YEAR 0
#endif

/* Fake libraries are enabled */
/* #undef BLOCXX_USE_FAKE_LIBS */

/* Version number of package */
#ifndef BLOCXX_VERSION 
#define BLOCXX_VERSION  "1.9.1" 
#endif

#ifndef BLOCXX_LIBRARY_VERSION
#define BLOCXX_LIBRARY_VERSION 5
#endif

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
/* #undef BLOCXX_WORDS_BIGENDIAN */

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#ifndef BLOCXX_YYTEXT_POINTER 
#define BLOCXX_YYTEXT_POINTER  1 
#endif

/* Define to `long' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to `uint16_t' if <sys/types.h> does not define. */
/* #undef u_int16_t */

/* Define to `uint32_t' if <sys/types.h> does not define. */
/* #undef u_int32_t */

/* Define to `uint8_t' if <sys/types.h> does not define. */
/* #undef u_int8_t */

// end of autoconf set vars

/**
 * The BLOCXX_DEPRECATED macro can be used to trigger compile-time warnings
 * with gcc >= 3.2 when deprecated functions are used.
 *
 * For non-inline functions, the macro is used at the very end of the
 * function declaration, right before the semicolon, unless it's pure
 * virtual:
 *
 * int deprecatedFunc() const BLOCXX_DEPRECATED;
 * virtual int deprecatedPureVirtualFunc() const BLOCXX_DEPRECATED = 0;
 *
 * Functions which are implemented inline are handled differently:
 * the BLOCXX_DEPRECATED macro is used at the front, right before the return
 * type, but after "static" or "virtual":
 *
 * BLOCXX_DEPRECATED void deprecatedFuncA() { .. }
 * virtual BLOCXX_DEPRECATED int deprecatedFuncB() { .. }
 * static BLOCXX_DEPRECATED bool deprecatedFuncC() { .. }
 *
 * You can also mark whole structs or classes as deprecated, by inserting the
 * BLOCXX_DEPRECATED macro after the struct/class keyword, but before the
 * name of the struct/class:
 *
 * class BLOCXX_DEPRECATED DeprecatedClass { };
 * struct BLOCXX_DEPRECATED DeprecatedStruct { };
 *
 */
#if __GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 2)
# define BLOCXX_DEPRECATED __attribute__ ((deprecated))
#else
# define BLOCXX_DEPRECATED
#endif

#define BLOCXX_FUNCTION_NOT_IMPLEMENTED

#ifdef __cplusplus
/* Need this first macro because ## doesn't expand vars, and we need an intermediate step */
#ifndef BLOCXX_NAMESPACE_CAT 
#define BLOCXX_NAMESPACE_CAT(ow, ver) ow ## ver 
#endif
#ifndef BLOCXX_NAMESPACE_AUX 
#define BLOCXX_NAMESPACE_AUX(ver) BLOCXX_NAMESPACE_CAT(blocxx, ver) 
#endif
#ifndef BLOCXX_NAMESPACE 
#define BLOCXX_NAMESPACE BLOCXX_NAMESPACE_AUX(BLOCXX_LIBRARY_VERSION) 
#endif

/* need this to set up an alias. */
namespace BLOCXX_NAMESPACE
{
}

namespace blocxx = BLOCXX_NAMESPACE;


#endif // #ifdef __cplusplus

#ifdef BLOCXX_HAVE_SLP_H 
#undef BLOCXX_HAVE_SLP_H
#endif

//#ifndef BLOCXX_DISABLE_DIGEST
//#define BLOCXX_DISABLE_DIGEST
//#endif

// Statements like:
// #pragma message(Reminder "Fix this problem!")
// Which will cause messages like:
// C:\Source\Project\main.cpp(47): Reminder: Fix this problem!
// to show up during compiles. Note that you can NOT use the
// words "error" or "warning" in your reminders, since it will
// make the IDE think it should abort execution. You can double
// click on these messages and jump to the line in question.
#define Stringize( L ) #L
#define MakeString( M, L ) M(L)
#define $Line MakeString( Stringize, __LINE__ )
#define Reminder __FILE__ "(" $Line ") : Reminder: "

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0400
#endif

#define BLOCXX_USE_DLL 1
#define NO_POSIX_SIGNALS 1
//#define BLOCXX_DEBUG_MEMORY 1

typedef long off_t;

#define snprintf _snprintf

#ifdef max
#undef max
#endif

#if !defined(BLOCXX_STATIC_BUILD) && !defined(_DLL)
#pragma message ( "WARNING: BLOCXX_config.h: Not building a DLL and BLOCXX_STATIC_BUILD is not defined.  Auto-enabling BLOCXX_STATIC_BUILD." )
#define BLOCXX_STATIC_BUILD
#endif

#ifdef BLOCXX_STATIC_BUILD
#define BLOCXX_EXPORT
#define BLOCXX_IMPORT
#else
#define BLOCXX_EXPORT __declspec(dllexport)
#define BLOCXX_IMPORT __declspec(dllimport)
#endif

#ifdef BLOCXX_COMMON_BUILD
#define BLOCXX_COMMON_API BLOCXX_EXPORT
#else
#define BLOCXX_COMMON_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_XML_BUILD
#define BLOCXX_XML_API BLOCXX_EXPORT
#else
#define BLOCXX_XML_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_MOF_BUILD
#define BLOCXX_MOF_API BLOCXX_EXPORT
#else
#define BLOCXX_MOF_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_HTTP_BUILD
#define BLOCXX_HTTP_API BLOCXX_EXPORT
#else
#define BLOCXX_HTTP_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_HTTPSVC_BUILD
#define BLOCXX_HTTPSVC_API BLOCXX_EXPORT
#else
#define BLOCXX_HTTPSVC_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_CLIENT_BUILD
#define BLOCXX_CLIENT_API BLOCXX_EXPORT
#else
#define BLOCXX_CLIENT_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_LISTENER_BUILD
#define BLOCXX_LISTENER_API BLOCXX_EXPORT
#else
#define BLOCXX_LISTENER_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_DB_BUILD
#define BLOCXX_DB_API BLOCXX_EXPORT
#else
#define BLOCXX_DB_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_REPHDB_BUILD
#define BLOCXX_HDB_API BLOCXX_EXPORT
#else
#define BLOCXX_HDB_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_WQL_BUILD
#define BLOCXX_WQL_API BLOCXX_EXPORT
#else
#define BLOCXX_WQL_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_WQLCOMMON_BUILD
#define BLOCXX_WQLCOMMON_API BLOCXX_EXPORT
#else
#define BLOCXX_WQLCOMMON_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_PROVIDER_BUILD
#define BLOCXX_PROVIDER_API BLOCXX_EXPORT
#else
#define BLOCXX_PROVIDER_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_CPPPROVIFC_BUILD
#define BLOCXX_CPPPROVIFC_API BLOCXX_EXPORT
#else
#define BLOCXX_CPPPROVIFC_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_CIMOMSERVER_BUILD
#define BLOCXX_CIMOMSERVER_API BLOCXX_EXPORT
#else
#define BLOCXX_CIMOMSERVER_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_CIMOMCOMMON_BUILD
#define BLOCXX_CIMOMCOMMON_API BLOCXX_EXPORT
#else
#define BLOCXX_CIMOMCOMMON_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_INDICATIONSERVER_BUILD
#define BLOCXX_INDICATIONSERVER_API BLOCXX_EXPORT
#else
#define BLOCXX_INDICATIONSERVER_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_INDICATIONREPLAYER_BUILD
#define BLOCXX_INDICATIONREPLAYER_API BLOCXX_EXPORT
#else
#define BLOCXX_INDICATIONREPLAYER_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_PROVIDERAGENT_BUILD
#define BLOCXX_PROVIDERAGENT_API BLOCXX_EXPORT
#else
#define BLOCXX_PROVIDERAGENT_API BLOCXX_IMPORT
#endif

#ifdef BLOCXX_WIN32
//#define BLOCXX_EXPORT_TEMPLATE(API, TMPL, X) \
//template class API TMPL< X >
#define BLOCXX_EXPORT_TEMPLATE(API, TMPL, X)
#else
#define BLOCXX_EXPORT_TEMPLATE(API, TMPL, X)
#endif

#pragma warning (disable: 4297)
#pragma warning (disable: 4355)
#pragma warning (disable: 4006)
#pragma warning (disable: 4396) // inline specifier cannot be used for templated friends
#pragma warning (disable: 4510) // default constructor cannot be generated (lazy global)
#pragma warning (disable: 4512) // assignment operator cannot be generated (lazy global)
#pragma warning (disable: 4610) // can never be instantiated, user defined constructor required(lazy global)
#pragma warning (disable: 4127) // conditional expression is constant (do { } while(0) -- everywhere macros are used)


/* __SRC_COMMON_BLOCXX_WINDOWS_CONFIG_H */
#endif
