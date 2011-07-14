if OPENGL
noinst_PROGRAMS=raytracer
else
noinst_PROGRAMS=
endif
if QUICKTIME 
raytracer_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a -lquicktime
raytracer_CXXFLAGS=-DQUICKTIME=1
else
raytracer_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a 
raytracer_CXXFLAGS=-DQUICKTIME=0
endif

raytracer_SOURCES = raytracer.C
INCLUDES= -I../COMMON/ -I../WPCG
