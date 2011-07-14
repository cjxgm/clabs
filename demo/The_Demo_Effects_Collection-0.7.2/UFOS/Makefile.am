if OPENGL
noinst_PROGRAMS=ufos
else
noinst_PROGRAMS=
endif

ufos_SOURCES = ufos.C 
ufos_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
INCLUDES= -I../COMMON/ -I../WPCG
