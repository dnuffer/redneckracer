APP_CPPFLAGS += -fexceptions -frtti -DANDROID
APP_CFLAGS += -DANDROID

ifeq ($(NDK_DEBUG),1)
	APP_CFLAGS += -DDEBUG
else
	APP_CFLAGS += -O3
	APP_OPTIM := release
endif

APP_STL := gnustl_static

#From the NDK CPU Arch ABIs document:
#II. Generating code for a specific ABI:
#=======================================
#
#By default, the NDK will generate machine code for the 'armeabi' ABI.
#You can however add the following line to your Application.mk to generate
#ARMv7-a compatible machine code instead:
#
#   APP_ABI := armeabi-v7a
#
#It is also possible to build machine code for *two* distinct ABIs by using:
#
#   APP_ABI := armeabi armeabi-v7a
#
#This will instruct the NDK to build two versions of your machine code: one for
#each ABI listed on this line. Both libraries will be copied to your application
#project path and will be ultimately packaged into your .apk.
#
#Such a package is called a "fat binary" in Android speak since it contains
#machine code for more than one CPU architecture. At installation time, the
#package manager will only unpack the most appropriate machine code for the
#target device. See below for details.

APP_ABI := armeabi armeabi-v7a
