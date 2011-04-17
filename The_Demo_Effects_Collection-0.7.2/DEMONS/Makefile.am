if OPENGL
noinst_PROGRAMS=demons
else
noinst_PROGRAMS=
endif

demons_SOURCES = demons.C 
demons_LDADD = ../COMMON/libTDEC.la ../WPCG/libWPCG.a ../OPCODE-1.2/libOPCODE.a
INCLUDES= -I../COMMON/ -I../WPCG
