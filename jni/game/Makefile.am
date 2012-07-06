lib_LTLIBRARIES = libgame.la
#noinst_PROGRAMS = redneckracer
#noinst_DATA = redneckracer-assets.zip

INCLUDES = -I@top_srcdir@ -I@top_srcdir@/boost

#.PHONY: redneckracer-assets.zip

redneckracer-assets.zip:
	rm -f $@
	cd $(abs_top_srcdir)/.. && pwd && zip -r -o $(abs_builddir)/$@ assets

libgame_la_LDFLAGS = -shared -rpath @prefix@/lib

libgame_la_SOURCES = \
	Animal.cpp \
	BestTimes.cpp \
	CivilCar.cpp \
	Destroyer.cpp \
	DrivingAI.cpp \
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
	TruckController.cpp \
	Truck.cpp \
	TruckTouchHandler.cpp

redneckracer_SOURCES = \
	eglut/eglut.c \
	eglut/eglut_x11.c \
	redneckracer-main.cpp

redneckracer_LDFLAGS = \
	-lX11 -lEGL -lGL -lGLESv1_CM \
	-L. -lgame \
	-L../engine -lengine \
	-L../graphlib -lgraph \
	-L../miniblocxx -lminiblocxx \
	-L../libzip -lzip \
	-L../libpng -lpng \
	-L../openal -loal
