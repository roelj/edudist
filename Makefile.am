AUTOMAKE_OPTIONS 	= subdir-objects
AM_CFLAGS 		= $(curl_CFLAGS) $(libzip_CFLAGS) $(sqlite_CFLAGS) $(openssl_CFLAGS) \
			  $(gtk_CFLAGS) $(glib_CFLAGS)
bin_PROGRAMS 		= moefel
moefel_SOURCES	 	= src/main.c \
		  	  src/database/installation.h src/database/installation.c \
		  	  src/database/repositories.h src/database/repositories.c \
		  	  src/database/packages.h src/database/packages.c \
		  	  src/database/files.h src/database/files.c \
		  	  src/database/configuration.h src/database/configuration.c \
			  src/datatypes/map.c src/datatypes/map.h \
			  src/datatypes/list.c src/datatypes/list.h \
			  src/datatypes/repository.c src/datatypes/repository.h \
			  src/datatypes/package.c src/datatypes/package.h \
			  src/datatypes/http_response.c \
			  src/datatypes/http_response.h \
			  src/datatypes/file.h \
		  	  src/filesystem/library.c src/filesystem/library.h \
		  	  src/gui/mainwindow.c src/gui/mainwindow.h \
		  	  src/gui/packages_tab.c src/gui/packages_tab.h \
		  	  src/gui/repositories_tab.c src/gui/repositories_tab.h \
			  src/high/command.c src/high/command.h \
		  	  src/misc/datetime.c src/misc/datetime.h \
		  	  src/misc/strings.c src/misc/strings.h \
		  	  src/network/http.c src/network/http.h \
		  	  src/parsers/common.c src/parsers/common.h \
		  	  src/parsers/package.c src/parsers/package.h \
		  	  src/parsers/repository.c src/parsers/repository.h \
		  	  src/packagers/zip.c src/packagers/zip.h

moefel_LDADD	        = $(curl_LIBS) $(libzip_LIBS) $(sqlite_LIBS) $(openssl_LIBS) $(gtk_LIBS) \
			  $(glib_LIBS)
if OS_LINUX
moefel_LDADD	       += -ldl -lpthread
else
moefel_LDADD	       += -lshell32
moefel_LDFLAGS	        = -mwindows
endif

if ENABLE_GCOV_OPTION
AM_CFLAGS	       += -fprofile-arcs -ftest-coverage
endif

GUILE_VERSION		= guile-1.8
GUILE_CONFIG		= `pkg-config $(GUILE_VERSION) --cflags --libs`
GUILE_SOURCE_FILES      = src/scm/cli.c src/scm/cli.h src/scm/init.c src/scm/init.h

guile-lib:
	$(CC) $(GUILE_CONFIG) -g -shared -o libguile-moefel.so -fPIC $(moefel_SOURCES) \
	$(GUILE_SOURCE_FILES) $(AM_CFLAGS) $(moefel_LDADD) $(guile_LIBS) $(guile_CFLAGS)

docs:
	@cd doc/; texi2pdf moefel.texi

docs-clean:
	@cd doc/; rm -rf *.aux *.cp *.fn *.ky *.log *.pg *.toc *.tp *.vr

docs-doxygen:
	@cd doc/; doxygen Doxyfile

gcov-clean:
	@rm -rf src/*.gcno src/*.gcda src/*/*.gcno src/*/*.gcda

.PHONY: docs docs-clean docs-doxygen gcov-clean
