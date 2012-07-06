#ifndef SRC_BLOCXX_BLOCXX_ANDROID_CONFIG_H
#define SRC_BLOCXX_BLOCXX_ANDROID_CONFIG_H 1

// Not a full config.h...

/* need this to set up an alias. */
namespace BLOCXX_NAMESPACE
{
namespace Time {}
}

namespace blocxx = BLOCXX_NAMESPACE;

#ifndef BLOCXX_SIZEOF_CHAR
#define BLOCXX_SIZEOF_CHAR 1
#endif

#ifndef BLOCXX_SIZEOF_SHORT_INT
#define BLOCXX_SIZEOF_SHORT_INT __SIZEOF_SHORT__
#endif

#ifndef BLOCXX_SIZEOF_INT
#define BLOCXX_SIZEOF_INT __SIZEOF_INT__
#endif

#ifndef BLOCXX_SIZEOF_LONG_INT
#define BLOCXX_SIZEOF_LONG_INT __SIZEOF_LONG__
#endif

#ifndef BLOCXX_SIZEOF_LONG_LONG_INT
#define BLOCXX_SIZEOF_LONG_LONG_INT __SIZEOF_LONG_LONG__
#endif

#ifndef BLOCXX_SIZEOF_FLOAT
#define BLOCXX_SIZEOF_FLOAT __SIZEOF_FLOAT__
#endif

#ifndef BLOCXX_SIZEOF_DOUBLE
#define BLOCXX_SIZEOF_DOUBLE __SIZEOF_DOUBLE__
#endif

#ifndef BLOCXX_SIZEOF_LONG_DOUBLE
#define BLOCXX_SIZEOF_LONG_DOUBLE __SIZEOF_LONG_DOUBLE__
#endif

#ifndef BLOCXX_THREAD_ONCE_INIT
#define BLOCXX_THREAD_ONCE_INIT PTHREAD_ONCE_INIT
#endif

#ifndef BLOCXX_EXPORT_TEMPLATE
#define BLOCXX_EXPORT_TEMPLATE(API, TMPL, X)
#endif

/* Correct invalid PTHREAD_MUTEX_INITIALIZER declarations (AIX 5.1). */
#ifndef BLOCXX_MUTEX_INITIALIZER
#define BLOCXX_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif

#define BLOCXX_COMMON_API
#define BLOCXX_FUNCTION_NOT_IMPLEMENTED
#define BLOCXX_USE_PTHREAD
#define BLOCXX_HAVE_GETTIMEOFDAY

#ifndef __EXCEPTIONS
#define BLOCXX_NO_EXCEPTIONS
#endif

// So far, this and the exceptions are the only things visible from
// preprocessor defines between crystax and the official NDK.
#ifndef __GXX_RTTI
#define BLOCXX_NO_STL
#endif

#if defined(ANDROID)
#define BLOCXX_NO_IOSTREAMS
#endif

#define BLOCXX_HAVE_STREAMBUF
#define BLOCXX_HAVE_SYS_TIME_H

/* SRC_BLOCXX_ANDROID_CONFIG_H */
#endif
