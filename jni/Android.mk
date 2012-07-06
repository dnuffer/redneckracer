LOCAL_PATH := $(call my-dir)

########################################################################################################
include $(CLEAR_VARS)

LOCAL_MODULE     := openal
LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/openal/include $(LOCAL_PATH)/openal/OpenAL32/Include
LOCAL_SRC_FILES  := openal/OpenAL32/alAuxEffectSlot.c \
                    openal/OpenAL32/alBuffer.c        \
                    openal/OpenAL32/alDatabuffer.c    \
                    openal/OpenAL32/alEffect.c        \
                    openal/OpenAL32/alError.c         \
                    openal/OpenAL32/alExtension.c     \
                    openal/OpenAL32/alFilter.c        \
                    openal/OpenAL32/alListener.c      \
                    openal/OpenAL32/alSource.c        \
                    openal/OpenAL32/alState.c         \
                    openal/OpenAL32/alThunk.c         \
                    openal/Alc/ALc.c                  \
                    openal/Alc/alcConfig.c            \
                    openal/Alc/alcEcho.c              \
                    openal/Alc/alcModulator.c         \
                    openal/Alc/alcReverb.c            \
                    openal/Alc/alcRing.c              \
                    openal/Alc/alcThread.c            \
                    openal/Alc/ALu.c                  \
                    openal/Alc/android.c              \
                    openal/Alc/bs2b.c                 \
                    openal/Alc/null.c                 \

LOCAL_CFLAGS     := -DAL_BUILD_LIBRARY -DAL_ALEXT_PROTOTYPES
LOCAL_LDLIBS     := -llog -Wl,-s

include $(BUILD_SHARED_LIBRARY)

########################################################################################################
include $(CLEAR_VARS)

LOCAL_MODULE := redneckracer

LOCAL_CFLAGS := -Wall -Wno-unused -fvisibility=hidden
#-Werror

LOCAL_C_INCLUDES := $(LOCAL_PATH)/boost $(LOCAL_PATH)/openal/include

LIBPNG_SRC_FILES :=\
	png.c \
	pngerror.c \
	pngget.c \
	pngmem.c \
	pngpread.c \
	pngread.c \
	pngrio.c \
	pngrtran.c \
	pngrutil.c \
	pngset.c \
	pngtrans.c \
	pngwio.c \
	pngwrite.c \
	pngwtran.c \
	pngwutil.c \

LIBZIP_SRC_FILES :=\
	zip_add.c \
	zip_add_dir.c \
	zip_close.c \
	zip_delete.c \
	zip_dirent.c \
	zip_entry_free.c \
	zip_entry_new.c \
	zip_err_str.c \
	zip_error.c \
	zip_error_clear.c \
	zip_error_get.c \
	zip_error_get_sys_type.c \
	zip_error_strerror.c \
	zip_error_to_str.c \
	zip_fclose.c \
	zip_file_error_clear.c \
	zip_file_error_get.c \
	zip_file_get_offset.c \
	zip_file_strerror.c \
	zip_filerange_crc.c \
	zip_fopen.c \
	zip_fopen_index.c \
	zip_fread.c \
	zip_free.c \
	zip_get_archive_comment.c \
	zip_get_archive_flag.c \
	zip_get_file_comment.c \
	zip_get_num_files.c \
	zip_get_name.c \
	zip_memdup.c \
	zip_name_locate.c \
	zip_new.c \
	zip_open.c \
	zip_rename.c \
	zip_replace.c \
	zip_set_archive_comment.c \
	zip_set_archive_flag.c \
	zip_set_file_comment.c \
	zip_source_buffer.c \
	zip_source_file.c \
	zip_source_filep.c \
	zip_source_free.c \
	zip_source_function.c \
	zip_source_zip.c \
	zip_set_name.c \
	zip_stat.c \
	zip_stat_index.c \
	zip_stat_init.c \
	zip_strerror.c \
	zip_unchange.c \
	zip_unchange_all.c \
	zip_unchange_archive.c \
	zip_unchange_data.c \

GRAPHLIB_SRC_FILES = \
	BaseClass.cpp \
	Capabilities.cpp \
	Cloneable.cpp \
	KeyboardInput.cpp \
	StringDumpable.cpp \

MINIBLOCXX_SRC_FILES = \
	Array.cpp \
	Assertion.cpp \
	AtomicOps.cpp \
	BaseStreamBuffer.cpp \
	BinarySerialization.cpp \
	Bool.cpp \
	Char16.cpp \
	COWIntrusiveCountableBase.cpp \
	DateTime.cpp \
	DelayedFormat.cpp \
	Demangle.cpp \
	Exception.cpp \
	Format.cpp \
	IntrusiveCountableBase.cpp \
	IOException.cpp \
	Mutex.cpp \
	MutexImpl.cpp \
	OutOfBoundsException.cpp \
	ReferenceHelpers.cpp \
	String.cpp \
	StringBuffer.cpp \
	StringConversionImpl.cpp \
	StringStream.cpp \
	ThreadOnce.cpp \
	TimeConstants.cpp \
	TimeDuration.cpp \
	TimePeriod.cpp \
	TmUtils.cpp \
	UTF8Utils.cpp \
	UTF8UtilscompareToIgnoreCase.cpp \

ENGINE_SRC_FILES = \
	AccelerateAction.cpp \
	AndroidKeyboardInput.cpp \
	Animation.cpp \
	Boundable.cpp \
	Collider.cpp \
	Director.cpp \
	Drawable.cpp \
	GL.cpp \
	GLMock.cpp \
	Label.cpp \
	Menu.cpp \
	MenuItem.cpp \
	Mesh.cpp \
	MoveAction.cpp \
	Resource.cpp \
	Resources.cpp \
	RotateAction.cpp \
	Scene.cpp \
	Sound.cpp \
	SoundDevice.cpp \
	Sprite.cpp \
	Texture.cpp \
	TexturedFont.cpp \
	TexturedQuad.cpp \
	TextureLibrary.cpp \
	TextureLoader.cpp \
	TouchButton.cpp \
	ProgressBar.cpp \
	DrawableRectangle.cpp \
	DrawableLine.cpp \
	FollowAction.cpp \

GAME_SRC_FILES = \
	Animal.cpp \
	BestTimes.cpp \
	CivilCar.cpp \
	Destroyer.cpp \
	DrivingAI.cpp \
	ErrorHandler.cpp \
	FPSAction.cpp \
	GameLibrary.cpp \
	GarageScene.cpp \
	Globals.cpp \
	LoadingScreenScene.cpp \
	MainMenuScene.cpp \
	Obstacle.cpp \
	OpponentAI.cpp \
	OpponentTruck.cpp \
	OptionsScene.cpp \
	PlayerTruck.cpp \
	PoliceAI.cpp \
	RacePosAction.cpp \
	RaceScene.cpp \
	RaceTracks.cpp \
	RedneckRacerGame.cpp \
	RoadBound.cpp \
	ShotGun.cpp \
	TrackSelectScene.cpp \
	TrophyRoomScene.cpp \
	Truck.cpp \
	TruckController.cpp \
	TruckTouchHandler.cpp \
	entry-point.cpp \


LOCAL_SRC_FILES := \
	$(LIBPNG_SRC_FILES:%=libpng/%) \
	$(LIBZIP_SRC_FILES:%=libzip/%) \
	$(GRAPHLIB_SRC_FILES:%=graphlib/%) \
	$(MINIBLOCXX_SRC_FILES:%=miniblocxx/%) \
	$(ENGINE_SRC_FILES:%=engine/%) \
	$(GAME_SRC_FILES:%=game/%) \

LOCAL_SHARED_LIBRARIES := libopenal
LOCAL_LDLIBS := -lGLESv1_CM -llog -lz

include $(BUILD_SHARED_LIBRARY)

