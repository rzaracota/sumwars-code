#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE(sumwars, 0.1)
AC_CONFIG_SRCDIR([src/core/pathfind.h])
AC_CONFIG_HEADERS([config.h])

# Check packages (DEPS_CFLAGS and DEPS_LIBS for Makefile.am)
PKG_CHECK_MODULES([DEPS], [OGRE >= 1.5.0 OIS CEGUI CEGUI-OGRE RakNet freealut openal lua5.1 vorbisfile])
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([libintl.h locale.h memory.h stddef.h stdlib.h string.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([floor gettimeofday memmove memset pow setlocale sqrt strchr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

