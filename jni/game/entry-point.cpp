// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.

#include <jni.h>
#include "engine/Log.hpp"
#include <stdint.h>

#include "RRConfig.hpp"
#include "Globals.hpp"
#include "ErrorHandler.hpp"
#include "miniblocxx/Exception.hpp"

using namespace rr;

int width = 0;
int height = 0;

static void activateMarketFeedback()
{
	JNIEnv *env;
	LOGASSERT(s_jvm != NULL, "expected s_jvm != NULL");
	s_jvm->AttachCurrentThread(
		 // The Sun (Oracle) jdk is stupid.  Work around type issues.
#ifdef _JAVASOFT_JNI_H_
		 reinterpret_cast<void**>(&env)
#else
		 reinterpret_cast<JNIEnv**>(&env)
#endif
		 , NULL);

	LOGASSERT(env != NULL, "s_jvm->AttachCurrentThread() gave back a NULL env");
	jclass cls = env->FindClass("com/nufferbrotherssoftware/RedneckRacerLite/StartRenderer");
	if(cls == NULL)
	{
		LOGE("Can't find java class: com/nufferbrotherssoftware/RedneckRacerLite/StartRenderer");
	}
	else
	{
		jmethodID mid = env->GetMethodID(cls, "showMarketFeedback", "()V");
		LOGASSERT(mid != NULL, "env->GetMethodID returned a NULL method id!");
		LOGD("Calling showMarketFeedback()");
		env->CallVoidMethod(s_obj, mid);
	}

}


extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeInit( JNIEnv*  env,  jobject obj, jstring apkPath, jstring dataPath )
{
	// Save off the pointer to the java StartRenderer object so that the crash handler
	// can call StartRenderer.nativeCrashed()
	s_obj = obj;
	const char* apkPathNative;
	jboolean isCopy;
	apkPathNative = env->GetStringUTFChars(apkPath, &isCopy);
	LOGD("nativeInit(apkPathNative: %s)", apkPathNative);
	
	// Getting java vm for redirecting to market feedback
	JavaVM* vm;
	env->GetJavaVM(&vm);
	jobject globalRef = env->NewGlobalRef(obj);
	game().setMarketFeedbackFunction(activateMarketFeedback);

	try
	{
		game().init(apkPathNative);
		const char* cDataPath = env->GetStringUTFChars(dataPath, &isCopy);
		LOGD("nativeInit(cDataPath: %s)", cDataPath);
		game().setSettingsFilePath(cDataPath);
		LOGD("nativeInit(game().getSettingsFile(): %s)", game().getSettingsFile().c_str());
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().init() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().init() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().init() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
	LOGD("nativeResize(w: %d, h: %d)", w, h);
	try
	{
		game().resize(w, h);
		width = w;
		height = h;
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().nativeResize() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().nativeResize() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().nativeResize() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeOnTouchEvent(
		JNIEnv* env, jobject  thiz, jlong downTime, jlong eventTime, jint action, jfloat x, jfloat y, jfloat pressure, jfloat size,
		jint metaState, jfloat xPrecision, jfloat yPrecision, jint deviceId, jint edgeFlags)
{
	try
	{
		game().handleTouchEvent(downTime, eventTime, action, x, y, pressure, size, metaState, xPrecision, yPrecision,
				deviceId, edgeFlags);
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().handleTouchEvent() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().handleTouchEvent() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().handleTouchEvent() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeOnTrackballEvent(
		JNIEnv* env, jobject  thiz, jfloat deltaX)
{
	try
	{
		game().handleTrackballEvent(deltaX);
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().handleTrackballEvent() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().handleTrackballEvent() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().handleTrackballEvent() threw unknown exception");
		abort();
	}
}

/* Call to render the next GL frame */
extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeRender( JNIEnv*  env )
{
	try
	{
		game().renderNextFrame();
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().renderNextFrame() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().renderNextFrame() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().renderNextFrame() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeOnOrientationChanged(
		JNIEnv* env, jobject thiz, jfloat rollAngle, jfloat pitchAngle, jfloat headingAngle)
{
	try
	{
		if (height >= width)
			game().handleOrientationChanged(rollAngle, pitchAngle, headingAngle);
		else
			game().handleOrientationChanged(rollAngle, - headingAngle, pitchAngle);
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().handleOrientationChanged() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().handleOrientationChanged() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().handleOrientationChanged() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeOnKeyDown(
	JNIEnv* env, jobject thiz, jint keyCode, jint metastate)
{
	try
	{
		game().keyDown(keyCode, metastate);
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().handleKey() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().handleKey() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().handleKey() threw unknown exception");
		abort();
	}
}

extern "C" __attribute__((visibility("default"))) void
Java_com_nufferbrotherssoftware_RedneckRacerLite_StartRenderer_nativeOnKeyUp(
	JNIEnv* env, jobject thiz, jint keyCode, jint metastate)
{
	try
	{
		game().keyUp(keyCode, metastate);
	}
	catch (const blocxx::Exception& e)
	{
		LOGE("game().handleKey() threw Exception: file: %s line: %d type: %s: \"%s\"", e.getFile(), e.getLine(), e.type(), e.getMessage());
		abort();
	}
	catch (const std::exception& e)
	{
		LOGE("game().handleKey() threw std::exception: %s", e.what());
		abort();
	}
	catch (...)
	{
		LOGE("game().handleKey() threw unknown exception");
		abort();
	}
}

