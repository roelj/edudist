AUTOMAKE_OPTIONS 	= subdir-objects
AM_CFLAGS 		= $(MOEFEL_CFLAGS)
bin_PROGRAMS 		= moefel
moefel_SOURCES	 	= src/main.c \
			  src/datatypes/map.c src/datatypes/map.h \
			  src/datatypes/list.c src/datatypes/list.h \
			  src/datatypes/protocols.h \
			  src/datatypes/repository.h \
			  src/datatypes/http_response.c \
			  src/datatypes/http_response.h \
		  	  src/network/http.c src/network/http.h \
		  	  src/parsers/common.c src/parsers/common.h \
		  	  src/parsers/uri.c src/parsers/uri.h \
		  	  src/parsers/package.c src/parsers/package.h \
		  	  src/parsers/repository.c src/parsers/repository.h \
		  	  src/packagers/zip.c src/packagers/zip.h \
		  	  src/database/installation.h src/database/installation.c \
		  	  src/database/repositories.h src/database/repositories.c \
		  	  src/database/packages.h src/database/packages.c \
		  	  src/database/files.h src/database/files.c \
		  	  src/database/configuration.h src/database/configuration.c \
		  	  src/misc/datetime.c src/misc/datetime.h \
		  	  src/misc/strings.c src/misc/strings.h

moefel_LDADD 		= $(MOEFEL_LIBS) -ldl -lpthread

docs:
	@cd doc/; texi2pdf moefel.texi

docs-clean:
	@cd doc/; rm -rf *.aux *.cp *.fn *.ky *.log *.pg *.toc *.tp *.vr

docs-doxygen:
	@cd doc/; doxygen Doxyfile

gcov-clean:
	@rm -rf src/*.gcno src/*.gcda src/*/*.gcno src/*/*.gcda

.PHONY: docs docs-clean docs-doxygen gcov-clean
