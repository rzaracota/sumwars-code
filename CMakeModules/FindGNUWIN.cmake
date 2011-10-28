# - Locate GnuWin32
# This module defines
#  GNUWIN_LIBRARY
#  GNUWIN_INCLUDE_DIR, where to find the headers
#
# Created by Augustin Preda. This is mainly intended to be used on Windows, where you would typically find GnuWin32.
# (last change: 2011.10.28)
#
INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(GNUWIN_INCLUDE_DIR libintl.h
  PATHS
  $ENV{GNUWINDIR}
  $ENV{PROGRAMFILES}
  PATH_SUFFIXES include
)

FIND_LIBRARY(GNUWIN_LIBRARY
  NAMES libintl.lib
  PATHS $ENV{GNUWINDIR}
  $ENV{PROGRAMFILES}
  PATH_SUFFIXES lib libs
)

# Handle the REQUIRED argument
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GNUWIN DEFAULT_MSG
    GNUWIN_LIBRARY
    GNUWIN_INCLUDE_DIR
)

# HANDLE_LIBRARY_TYPES(GNUWIN)

MARK_AS_ADVANCED(
    GNUWIN_INCLUDE_DIR
    GNUWIN_LIBRARY
)
