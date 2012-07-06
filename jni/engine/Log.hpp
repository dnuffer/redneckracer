// Copyright 2011 Nuffer Brothers Software LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef engine_Log_HPP_INCLUDED_
#define engine_Log_HPP_INCLUDED_

#include "engine/EngineConfig.hpp"

#define LOG_TAG "redneckracer"


#if defined(DEBUG) && defined(ANDROID)
	#include <android/log.h>
	#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
	#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
	#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
	#define LOGASSERT(cond, ...) do{ if(!(cond)) {__android_log_assert(#cond, LOG_TAG, __VA_ARGS__); } } while(false)
#elif (defined(DEBUG) && defined(__gnu_linux__)) || (TARGET_OS_IPHONE == 1)
#include <stdio.h>
	#define LOGD(...)  fprintf(stderr,__VA_ARGS__); fprintf(stderr, "\n")
	#define LOGI(...)  fprintf(stderr,__VA_ARGS__); fprintf(stderr, "\n")
	#define LOGE(...)  fprintf(stderr,__VA_ARGS__); fprintf(stderr, "\n")
	#define LOGASSERT(cond, ...) do{ if(!(cond)) { LOGE(__VA_ARGS__); } } while(false)
#else
	#define LOGD(...)
	#define LOGI(...)
	#define LOGE(...)
	#define LOGASSERT(cond, ...)
#endif

#endif
