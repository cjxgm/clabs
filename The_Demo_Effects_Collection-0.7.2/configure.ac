dnl Process this file with autoconf to produce a configure script.
AC_INIT(Makefile.am)
#
AC_CANONICAL_SYSTEM
AC_ARG_PROGRAM
PACKAGE=The_Demo_Effects_Collection
THE_DEMO_EFFECTS_COLLECTION_VERSION=0.7.2

AC_PREFIX_DEFAULT("/usr/local/tdec")

AM_INIT_AUTOMAKE(The_Demo_Effects_Collection, 0.7.2)
AM_CONFIG_HEADER(config.h)

AC_CONFIG_SUBDIRS(libltdl)
AC_PROG_CC
AC_PROG_CPP

# Configure libltdl
AC_LIBLTDL_CONVENIENCE(libltdl)

# Configure libtool
AC_ENABLE_SHARED
AC_ENABLE_STATIC
AC_LIBTOOL_DLOPEN
AC_LIBTOOL_SETUP
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl Checks for programs.


AC_PROG_CXX
AC_PROG_MAKE_SET

AM_SANITY_CHECK

dnl Checks for libraries.
AC_CHECK_LIB(pthread, pthread_exit, , [AC_MSG_ERROR([libpthread not found])],)

AC_CHECK_LIB(SDL, SDL_Quit, , [AC_MSG_ERROR([libSDL not found])],
         -lpthread)

AC_CHECK_LIB(SDL_image, IMG_Load, , [AC_MSG_ERROR([libSDL_image not found])], 
	-lpthread)

AC_CHECK_LIB(m, abs, , [AC_MSG_ERROR([libm not found])],)


#AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, , [AC_MSG_ERROR([libSDL_mixer not found])], 
#	-lpthread)
AC_ARG_ENABLE(quicktime,
     [  --enable-quicktime    Use libquicktime to build a quicktime movie of raytrace [default=false]],
     [case "${enableval}" in
       yes) quicktime=true ;;
       no)  quicktime=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-libquicktime) ;;
     esac],[quicktime=false])
AM_CONDITIONAL(QUICKTIME, test x$quicktime = xtrue)

if test x$quicktime = xtrue; then
AC_CHECK_LIB(quicktime, quicktime_set_video, , [AC_MSG_ERROR([libquicktime not found])],
	-lpthread)
AC_CHECK_HEADERS(quicktime/quicktime.h)
else
echo "The use of libquicktime is disabled "
echo "No quicktime movie will be made when raytracer is run"
echo "run configure --enable-libquicktime=yes to include quicktime support "
fi

AC_ARG_ENABLE(mikmod,
     [  --enable-mikmod    Turn on mikmod [default=false]],
     [case "${enableval}" in
       yes) mikmod=true ;;
       no)  mikmod=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-mikmod) ;;
     esac],[mikmod=false])
AM_CONDITIONAL(MIKMOD, test x$mikmod = xtrue)

if test x$mikmod = xtrue; then
AC_CHECK_LIB(mikmod, load_mod, , [AC_MSG_ERROR([libmikmod not found])],
	-lpthread)
AC_CHECK_HEADERS(mikmod.h)
CFLAGS="$CFLAGS `libmikmod-config --cflags`"
LIBS="$LIBS `libmikmod-config --libs`"
else
echo "The use of the mikmod sound library is disabled "
echo "The Equalizer effect will not be build "
echo "run configure --enable-mikmod=yes to include mod support "
fi

AC_ARG_ENABLE(opengl,
     [  --enable-opengl    Turn on 3D effects using opengl [default=yes]],
     [case "${enableval}" in
       yes) opengl=true ;;
       no)  opengl=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-opengl) ;;
     esac],[opengl=true])
AM_CONDITIONAL(OPENGL, test x$opengl = xtrue)

if test x$opengl = xtrue; then
AC_PATH_XTRA
AC_CHECK_HEADERS(GL/gl.h GL/glu.h GL/glut.h)
AC_CHECK_LIB(GL, glPushMatrix, ,[AC_MSG_ERROR([libGL not found])],)
AC_CHECK_LIB(GLU, gluLookAt, ,[AC_MSG_ERROR([libGLU not found])],$X_LIBS)
AC_CHECK_LIB(glut, glutGetModifiers, ,[AC_MSG_ERROR([libglut not found])], $LIBS $X_LIBS $X_PRE_LIBS -lpthread -lX11 
-lXi -lXext -lXmu $X_EXTRA_LIBS -lm)
else
echo "The use of OpenGL is disabled"
echo "3D effects depending on OpenGL will not be build "
echo "run configure --enable-opengl=yes to include OpenGL support"
fi

LIBS="$LIBS $X_LIBS $X_PRE_LIBS -lpthread -lX11 -lXi -lXext -lXmu $X_EXTRA_LIBS -lm"

#files OPC_TreeCollider.cpp and OPC_RayCollider.cpp of OPCODE-1.2 generate run-time errors when compiled with optimization -O2 
#Therefore these are compiled with -O1
AC_MSG_CHECKING(checking gcc version)
AM_CONDITIONAL(GCCVERSION3, true)
AC_MSG_RESULT()

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(SDL/SDL_image.h SDL/SDL.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Get compile and linking flags
CFLAGS="$CFLAGS `sdl-config --cflags` -Wall -std=c89 -U__STRICT_ANSI__ -pedantic"
CPPFLAGS="$CPPFLAGS"
LIBS="$LIBS `sdl-config --libs`"

#ACLOCAL="aclocal -I m4 \$(ACLOCAL_FLAGS)"
#AC_SUBST(ACLOCAL)

includedir="$includedir"
AC_SUBST(includedir)
# Substitute INCLTDL and LIBLTDL in the Makefiles
AC_SUBST(INCLTDL)
AC_SUBST(LIBLTDL)

AC_OUTPUT(Makefile STARFIELD/Makefile PLASMA/Makefile COPPERBARS/Makefile EXPLOSION/Makefile FIRE/Makefile 
CANDYBAR/Makefile SINUSSCROLLER/Makefile LENS/Makefile JUMPINGSCROLL/Makefile SHADEBOB/Makefile COMMON/Makefile 
BOBS/Makefile UNLIMITEDBOBS/Makefile ROTOZOOM/Makefile EQUALIZER/Makefile CIRCLESCROLLER/Makefile SINEWAVE/Makefile 
FLIPIMAGE/Makefile WATER/Makefile DOOMMELT/Makefile SPIRALTWIST/Makefile OPCODE-1.2/Makefile WPCG/Makefile 
DEMON/Makefile BUMP2D/Makefile TERRAIN/Makefile UFOS/Makefile PLASMACUBE/Makefile TWINSCROLLER/Makefile 
SPIRALSINESCROLL/Makefile BARSCROLLER/Makefile TUNNEL/Makefile BLOB/Makefile TUXTRAIL/Makefile DEMONS/Makefile 
MULTILAYER/Makefile PLUGINS/Makefile PLUGINS/PLASMA/Makefile PLUGINS/FIRE/Makefile PLUGINS/SINESCROLLER/Makefile 
PLUGINS/JUMPINGSCROLLER/Makefile PLUGINS/STARFIELD/Makefile PLUGINS/COPPERBARS/Makefile PLUGINS/LENS/Makefile 
PLUGINS/FADE/Makefile PLUGINS/CIRCLESCROLLER/Makefile PLUGINS/RECTFILL/Makefile RAYTRACER/Makefile)
