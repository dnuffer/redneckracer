// Copyright 2011 Nuffer Brothers Software LLC. All Rights Reserved.


#include "ErrorHandler.hpp"

extern "C" {

JavaVM* s_jvm;
jobject s_obj;
static jmethodID s_nativeCrashed;

static struct sigaction old_sa[NSIG];

void android_sigaction(int signal, siginfo_t* info, void* reserved) {
    JNIEnv *env;
    s_jvm->AttachCurrentThread(
		 // The Sun (Oracle) jdk is stupid.  Work around type issues.
#ifdef _JAVASOFT_JNI_H_
		 reinterpret_cast<void**>(&env)
#else
		 reinterpret_cast<JNIEnv**>(&env)
#endif
		 , NULL);
    env->CallVoidMethod(s_obj, s_nativeCrashed);
    old_sa[signal].sa_handler(signal);
}

__attribute__ ((visibility ("default"))) JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* jvm, void* reserved)
{
	LOGD("JNI_OnLoad: jvm = %p", jvm);
    jclass cls;
    s_jvm = jvm;
    JNIEnv* env;
    if(jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_2))
		 return JNI_ERR;
    cls = env->FindClass("com/nufferbrotherssoftware/RedneckRacerLite/StartRenderer");
    
    s_nativeCrashed = env->GetMethodID(cls, "nativeCrashed", "()V");
    
    //// Try to catch crashes...
    struct sigaction handler;
    memset(&handler, 0, sizeof(struct sigaction));
    handler.sa_sigaction = android_sigaction;
    handler.sa_flags = SA_RESETHAND;
    #define CATCHSIG(X) sigaction(X, &handler, &old_sa[X])
    CATCHSIG(SIGILL);
    CATCHSIG(SIGABRT);
    CATCHSIG(SIGBUS);
    CATCHSIG(SIGFPE);
    CATCHSIG(SIGSEGV);
    CATCHSIG(SIGSTKFLT);
    CATCHSIG(SIGPIPE);
    
    return JNI_VERSION_1_2;
}
}

