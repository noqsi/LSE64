#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([lse64],[0.3],[jpd@noqsi.com])
AC_CONFIG_MACRO_DIR([libltdl/m4])
AC_CONFIG_AUX_DIR([libltdl/config])
AC_CONFIG_SRCDIR([src/lse64.c])
AM_CONFIG_HEADER([src/config.h])
AM_INIT_AUTOMAKE

# Libtool
LT_CONFIG_LTDL_DIR([libltdl])
LT_INIT([dlopen])
LTDL_INIT
_LT_CONFIG_LTDL_DIR([m4_default([], [libltdl])])
_LTDL_CONVENIENCE
_LT_SET_OPTION([LT_INIT],[dlopen])
m4_warn([obsolete],[AC_LIBTOOL_DLOPEN: Remove this warning and the call to _LT_SET_OPTION when you
put the 'dlopen' option into LT_INIT's first parameter.])

LT_INIT

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
#_LT_SET_OPTION([LT_INIT],[dlopen])
m4_warn([obsolete],[AC_LIBTOOL_DLOPEN: Remove this warning and the call to _LT_SET_OPTION when you
put the 'dlopen' option into LT_INIT's first parameter.])


# Checks for libraries.
AC_CHECK_LIB([comedi], [comedi_data_write], 
	     [AC_CHECK_HEADER([comedi.h],
	         [COMEDILIBS="-lcomedi"; AC_DEFINE([HAVE_LIBCOMEDI], [], [Have the comedi libraries available])],
	     	 AC_MSG_WARN([** COMEDI header files not found - comedi support disabled]))],
	     AC_MSG_WARN([** COMEDI library not found - comedi support disabled]))
AC_SUBST(COMEDILIBS)
AM_CONDITIONAL(HAVE_COMEDI, [test "x$COMEDILIBS" != x])
AC_CHECK_LIB([m], [fmod])

# Checks for header files.
m4_warn([obsolete],
[The preprocessor macro `STDC_HEADERS' is obsolete.
  Except in unusual embedded environments, you can safely include all
  ISO C90 headers unconditionally.])dnl
# Autoupdate added the next two lines to ensure that your configure
# script's behavior did not change.  They are probably safe to remove.
AC_CHECK_INCLUDES_DEFAULT
AC_PROG_EGREP

AC_CHECK_HEADERS([dlfcn.h math.h limits.h locale.h stdint.h stdlib.h string.h sys/param.h unistd.h])

# Parallel Port/Sequencer stuff
AC_CHECK_HEADER([linux/lp.h],
	         [YES_SEQ="yes"; AC_DEFINE([HAVE_LP_H], [], [Have the linux parallel port/sequencer junk available])],
		 AC_MSG_WARN([** linux/lp.h header file not found - parallel port sequencer support disabled]))
AM_CONDITIONAL(HAVE_SEQUENCER, [test "x$YES_SEQ" = xyes])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_STRTOD
AC_CHECK_FUNCS([setlocale])

# Output Files
AC_CONFIG_FILES([Makefile src/Makefile src/modules/Makefile])
AC_OUTPUT
