###############################################################################
#                                                                             #
# Source file for 'autoconf' program generating 'configure' script            #
#                                                                             #
# library version: 0.4.0                                                      #
#                                                                             #
# (C) Pavel Stupka 2005                                                       #
#                                                                             #
###############################################################################

#------------------------------------------------------------------------------
# Init macros
#------------------------------------------------------------------------------

# version 2.59 or higher of 'autoconf' is required
AC_PREREQ(2.59)
AC_INIT(src/astral3d.h)
AM_CONFIG_HEADER([config.h])

#------------------------------------------------------------------------------
# Package name and version
#------------------------------------------------------------------------------

GENERIC_LIBRARY_NAME=astral3d

# library version
GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=4
GENERIC_MICRO_VERSION=0

# API version
GENERIC_API_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_API_VERSION)

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

#------------------------------------------------------------------------------
# Check for programs
#------------------------------------------------------------------------------

AC_PROG_CXX
AC_PROG_CC
AM_PROG_LIBTOOL
AM_SANITY_CHECK
AC_LANG_CPLUSPLUS

#------------------------------------------------------------------------------
# SDL library version check (http://www.libsdl.org/)
#------------------------------------------------------------------------------

SDL_VERSION=1.2.8
AM_PATH_SDL($SDL_VERSION,
        :,
        AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LD_FLAGS $SDL_LIBS"

#------------------------------------------------------------------------------
# Header files check
#------------------------------------------------------------------------------

AC_CHECK_HEADER([SDL/SDL_image.h],
        :,
        [AC_MSG_ERROR([*** Could not find SDL/SDL_image.h \
You must download SDL_image from http://www.libsdl.org/projects/SDL_image/ to\
 proceed])])

AC_CHECK_HEADER([GL/gl.h],
        :,
        [AC_MSG_ERROR([*** Could not find GL/gl.h])])

AC_CHECK_HEADER([GL/glu.h],
        :,
        [AC_MSG_ERROR([*** Could not find GL/glu.h])])

LIBS="$LIBS -lGL -lGLU -lSDL_image"

#------------------------------------------------------------------------------
# Libraries check
#------------------------------------------------------------------------------

AC_CHECK_LIB([GL], [glEnable],
        :,
        [AC_MSG_ERROR([*** OpenGL devel library not found!])])

AC_CHECK_LIB([GLU], [gluPerspective],
        :,
        [AC_MSG_ERROR([*** GLU devel library not found!])])

AC_CHECK_LIB([m], [sin],
        :,
        [AC_MSG_ERROR([*** Mathemtical (-lm) library not found!])])

AC_CHECK_LIB([SDL], [SDL_Init],
        :,
        [AC_MSG_ERROR([*** SDL devel library not found!])])

AC_CHECK_LIB([SDL_image], [IMG_Load],
        :,
        [AC_MSG_ERROR([*** SDL_image devel library not found!])])

AC_PROG_RANLIB

#------------------------------------------------------------------------------
# Compiler characteristics check
#------------------------------------------------------------------------------

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

#------------------------------------------------------------------------------
# Check for functions
#------------------------------------------------------------------------------

AC_HEADER_STDC
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([sqrt])

#------------------------------------------------------------------------------
# Output files
#------------------------------------------------------------------------------

AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT
