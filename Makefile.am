AUTOMAKE_OPTIONS 	= subdir-objects
AM_CFLAGS 		= $(EDUDIST_CFLAGS)
bin_PROGRAMS 		= edudist
edudist_SOURCES 	= src/main.c src/datatypes/map.c \
			  src/parsers/configuration.c src/parsers/configuration.h \
			  src/network/http.c src/network/http.h \
			  src/parsers/uri.c src/parsers/uri.h \
			  src/datatypes/map.h src/datatypes/protocols.h \
			  src/datatypes/http_response.c src/datatypes/http_response.h \
			  src/high/command.c src/high/command.h
edudist_LDADD 		= $(EDUDIST_LIBS)
