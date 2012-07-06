// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.


#ifndef game_ErrorHandler_hpp_INCLUDED
#define game_ErrorHandler_hpp_INCLUDED

#include <jni.h>
#include <signal.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "engine/Log.hpp"

extern "C"
{
	extern jobject s_obj;
	extern JavaVM* s_jvm;
}

#endif
