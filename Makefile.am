AUTOMAKE_OPTIONS 	= subdir-objects
AM_CFLAGS 		= $(EDUDIST_CFLAGS)
bin_PROGRAMS 		= edudist
edudist_SOURCES 	= src/main.c src/datatypes/map.c \
			  src/parsers/configuration.c src/parsers/configuration.h \
			  src/datatypes/map.h src/datatypes/configuration.h
edudist_LDADD 		= $(EDUDIST_LIBS)
