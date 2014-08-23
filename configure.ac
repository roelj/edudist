AC_INIT([moefel], [0.1], [roel@moefel.org])
AC_CONFIG_AUX_DIR([scripts])
AC_CANONICAL_HOST
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/main.c])
AC_PROG_CC
AM_PROG_CC_C_O

AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h])
AC_CONFIG_FILES([Makefile])

case $host in
*-linux*)
AC_SUBST(OS_LINUX)
AM_CONDITIONAL([OS_LINUX], [true])
;;
*-mingw*)
AC_SUBST(OS_LINUX)
AM_CONDITIONAL([OS_LINUX], [false])
;;
*-cygwin*)
AC_SUBST(OS_LINUX)
AM_CONDITIONAL([OS_LINUX], [true])
;;
*)
AC_MSG_ERROR([Moefel is not (yet) supported on $host.])
esac

AC_SUBST(ENABLE_GCOV_OPTION)
AC_ARG_ENABLE([gcov],
AC_HELP_STRING([--enable-gcov],
[enable GCOV suport @<:@default=no@:>@]),
AM_CONDITIONAL([ENABLE_GCOV_OPTION], [true]), AM_CONDITIONAL([ENABLE_GCOV_OPTION], [false]))

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([curl], [libcurl])
PKG_CHECK_MODULES([libzip], [libzip])
PKG_CHECK_MODULES([sqlite], [sqlite3])
PKG_CHECK_MODULES([openssl], [openssl])
PKG_CHECK_MODULES([gtk], [gtk+-3.0])
PKG_CHECK_MODULES([glib], [glib-2.0])
PKG_CHECK_MODULES([guile], [guile-1.8])

AC_OUTPUT
