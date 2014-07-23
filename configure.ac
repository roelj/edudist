AC_INIT([moefel], [0.1], [roel@moefel.org])
AC_CONFIG_AUX_DIR([scripts])
AC_CONFIG_MACRO_DIR([scripts])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])
AC_CONFIG_SRCDIR([src/main.c])
LT_INIT
AC_PROG_CC
AM_PROG_CC_C_O

AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h])
AC_CONFIG_FILES([Makefile])

AC_MSG_CHECKING([operating system])
AC_MSG_RESULT($host)
case $host in
*-linux*)
AC_MSG_RESULT([ (Linux back-end)])
AC_DEFINE(OS_LINUX, 1, [Linux implementations])
AC_SUBST(OS_LINUX)
AM_CONDITIONAL(OS_LINUX, test "x$os" = xlinux)
os="linux"
;;
*-mingw*)
AC_MSG_RESULT([ (Windows back-end, using MinGW)])
AC_DEFINE(OS_WINDOWS, 1, [Windows implementations])
AC_SUBST(OS_WINDOWS)
os="windows"
;;
*-cygwin*)
AC_MSG_RESULT([ (Windows back-end, using Cygwin)])
AC_DEFINE(OS_LINUX, 1, [Linux implementations])
AC_SUBST(OS_LINUX)
os="linux"
;;
*)
AC_MSG_ERROR([Moefel is not (yet) supported on $host.])
esac

AM_CONDITIONAL(OS_LINUX, test "x$os" = xlinux)
AM_CONDITIONAL(OS_WINDOWS, test "x$os" = xwindows)

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([MOEFEL], [libcurl libzip sqlite3 openssl])

AC_OUTPUT
