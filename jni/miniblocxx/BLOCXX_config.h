#ifndef __SRC_COMMON_BLOCXX_CONFIG_H
#define __SRC_COMMON_BLOCXX_CONFIG_H 1

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#if defined(_WIN32)
#include "miniblocxx/BLOCXX_windows_config.h"
#elif defined(ANDROID) || defined(linux)
#include "miniblocxx/BLOCXX_android_config.h"
#elif defined(__APPLE__) && defined(_ARCH_PPC)
#include "miniblocxx/BLOCXX_osx_ppc_config.h"
#elif defined(__APPLE__) && defined(__x86_64)
#include "miniblocxx/BLOCXX_osx_x64_config.h"
#elif defined(__APPLE__) && defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE == 1
#include "miniblocxx/BLOCXX_osx_iOS_config.h"
#else
#error You need to fix this to configure properly on your platform
#endif

/* __SRC_COMMON_BLOCXX_CONFIG_H */
#endif
