noinst_PROGRAMS = multilayer
multilayer_SOURCES = multilayer.c 
multilayer_LDFLAGS = $(LIBLTDL) -export-dynamic ../COMMON/libTDEC.la "-dlopen" self 
multilayer_DEPENDENCIES = $(LIBLTDL)
INCLUDES = -I../COMMON
