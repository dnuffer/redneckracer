lib_LTLIBRARIES = libgraph.la
noinst_PROGRAMS = fb_test make_bitmap_font

libgraph_la_LDFLAGS = -shared -rpath @prefix@/lib

if HAVE_FREETYPE
freetype_defines = -DHAVE_FREETYPE
freetype_sources = FTFont.cpp
else
freetype_sources =
endif


libgraph_la_SOURCES = \
BaseClass.cpp \
Bounding2d.cpp \
BoundingGrid2d.cpp \
Intersectable2d.cpp \
Polygon2d.cpp \
Capabilities.cpp \
Cloneable.cpp \
StringDumpable.cpp \
KeyboardInput.cpp \
MouseInput.cpp \
TextUtils.cpp \
$(freetype_sources) \
PPMLoader.cpp \
XPMLoader.cpp \
XFrameBuffer.cpp

INCLUDES = -I@top_srcdir@ $(X_INCLUDE_DIRS) $(freetype_CFLAGS) $(freetype_defines)
LIBS += $(X_LIBS) -lX11 $(freetype_LIBS)

fb_test_SOURCES = fb_test.cpp
fb_test_LDFLAGS = -L. -lgraph -L../miniblocxx -lminiblocxx -lpthread

make_bitmap_font_SOURCES = make_bitmap_font.cpp
make_bitmap_font_LDFLAGS = -L. -lgraph -L../miniblocxx -lminiblocxx -lpthread
