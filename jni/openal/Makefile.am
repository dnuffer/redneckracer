lib_LTLIBRARIES = liboal.la

liboal_la_CFLAGS =  \
-DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES -DHAVE_ALSA \
-Iinclude -IOpenAL32/Include

liboal_la_LDFLAGS = -shared -rpath @prefix@/lib

liboal_la_SOURCES = \
	OpenAL32/alAuxEffectSlot.c \
	OpenAL32/alBuffer.c        \
	OpenAL32/alDatabuffer.c    \
	OpenAL32/alEffect.c        \
	OpenAL32/alError.c         \
	OpenAL32/alExtension.c     \
	OpenAL32/alFilter.c        \
	OpenAL32/alListener.c      \
	OpenAL32/alSource.c        \
	OpenAL32/alState.c         \
	OpenAL32/alThunk.c         \
	Alc/ALc.c                  \
	Alc/alcConfig.c            \
	Alc/alcEcho.c              \
	Alc/alcModulator.c         \
	Alc/alcReverb.c            \
	Alc/alcRing.c              \
	Alc/alcThread.c            \
	Alc/ALu.c                  \
	Alc/alsa.c              \
	Alc/bs2b.c                 \
	Alc/null.c

