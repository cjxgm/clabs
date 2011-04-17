if OPENGL
noinst_PROGRAMS=demon
else
noinst_PROGRAMS=
endif

demon_SOURCES = demon.C 
demon_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
INCLUDES= -I../COMMON/ -I../WPCG
