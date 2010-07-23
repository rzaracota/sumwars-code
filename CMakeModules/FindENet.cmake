# - Try to find enet
# Once done this will define
#
#  ENET_FOUND - system has enet
#  ENet_INCLUDE_DIR - the enet include directory
#  ENet_LIBRARY - the library needed to link against enet
#
# $ENETDIR is an environment variable used for finding enet.
#
#  Borrowed from The Mana World
#  http://themanaworld.org/
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli and Reto Grieder
# Version checking by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(ENET_INCLUDE_DIR enet/enet.h
  PATHS $ENV{ENETDIR}
  PATH_SUFFIXES include
)
FIND_LIBRARY(ENET_LIBRARY_OPTIMIZED
  NAMES enet
  PATHS $ENV{ENETDIR}
  PATH_SUFFIXES lib
)
FIND_LIBRARY(ENET_LIBRARY_DEBUG
  NAMES enetd enet_d enet_D
  PATHS $ENV{ENETDIR}
  PATH_SUFFIXES lib
)

# Try to determine the version. Note that enet only stores the major
# version in the header file. So we check for existing functions.
# Hence the this script only distinguishes between 1.0, 1.1 and 1.2
FILE(STRINGS ${ENET_INCLUDE_DIR}/enet/enet.h _enet_header REGEX "ENET_")
IF(_enet_header MATCHES "ENET_VERSION[ \t]*=[ \t]*1")
  IF(_enet_header MATCHES "enet_socket_set_option")
    SET(ENET_VERSION 1.2)
  ELSEIF(_enet_header MATCHES "enet_peer_disconnect_later")
    SET(ENET_VERSION 1.1)
  ELSE()
    SET(ENET_VERSION 1.0)
  ENDIF()
ELSE()
  SET(ENET_VERSION 0) # Script doesn't support versions below 1.0
ENDIF()

# Handle the REQUIRED argument and set ENET_FOUND
# Also check the the version requirements
FIND_PACKAGE_HANDLE_ADVANCED_ARGS(ENet DEFAULT_MSG ${ENET_VERSION}
  ENET_LIBRARY_OPTIMIZED
  ENET_INCLUDE_DIR
)

# Collect optimized and debug libraries
IF(NOT LINK_ENET_DYNAMIC AND WIN32)
  # ENet is linked statically, hence we need to add some windows dependencies
  HANDLE_LIBRARY_TYPES(ENET ws2_32 winmm)
ELSE()
  HANDLE_LIBRARY_TYPES(ENET)
ENDIF()

MARK_AS_ADVANCED(
  ENET_INCLUDE_DIR
  ENET_LIBRARY_OPTIMIZED
  ENET_LIBRARY_DEBUG
)
