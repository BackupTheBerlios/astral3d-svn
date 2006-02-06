h_sources = astral3d astral3d.h atexture.h awindow.h acamera.h alevel.h atext.h \
            avector.h acollision.h apolygons.h aconsole.h a3ds.h a3dsmodel.h aerror.h \
            alight.h asurface.h

cpp_sources = acamera.cpp alevel.cpp atext.cpp avector.cpp acollision.cpp apolygons.cpp \
              atexture.cpp awindow.cpp aconsole.cpp a3ds.cpp a3dsmodel.cpp aerror.cpp \
              alight.cpp asurface.cpp

library_includedir=$(includedir)
library_include_HEADERS = $(h_sources)

INCLUDES = -I$(top_srcdir)

lib_LIBRARIES= libastral3d.a
libastral3d_a_SOURCES= $(h_sources) $(cpp_sources)
