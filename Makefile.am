AUTOMAKE_OPTIONS 	= subdir-objects
AM_CFLAGS 		= $(EDUDIST_CFLAGS)
bin_PROGRAMS 		= edudist
edudist_SOURCES 	= src/main.c \
			  src/datatypes/map.c src/datatypes/map.h \
			  src/datatypes/list.c src/datatypes/list.h \
			  src/datatypes/protocols.h \
			  src/datatypes/repository.h \
			  src/datatypes/http_response.c \
			  src/datatypes/http_response.h \
			  src/network/http.c src/network/http.h \
			  src/parsers/uri.c src/parsers/uri.h \
			  src/parsers/configuration.c src/parsers/configuration.h \
			  src/parsers/package.c src/parsers/package.h \
			  src/packagers/zip.c src/packagers/zip.h \
			  src/database/sqlite3.c src/database/sqlite3.h \
			  src/database/installation.h src/database/installation.c \
			  src/database/repositories.h src/database/repositories.c \
			  src/database/packages.h src/database/packages.c \
			  src/database/files.h src/database/files.c \
			  src/high/command.c src/high/command.h

edudist_LDADD 		= $(EDUDIST_LIBS) -ldl -lpthread
