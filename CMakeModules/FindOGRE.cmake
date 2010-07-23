# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIR
#  OGRE_LIBRARY, the library to link against to use OGRE.
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright © 2007, Matt Williams
# Modified by Nicolas Schlumberger to make it work on the Tardis-Infrastucture
# of the ETH Zurich (removed later on)
#
# Redistribution and use is allowed according to the terms of the BSD license.
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
# Version checking by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(DetermineVersion)
INCLUDE(FindPackageHandleAdvancedArgs)
INCLUDE(HandleLibraryTypes)

FIND_PATH(OGRE_INCLUDE_DIR Ogre.h
  PATHS $ENV{OGRE_HOME}
  PATH_SUFFIXES include include/OGRE Ogre.framework/Headers
)
FIND_LIBRARY(OGRE_LIBRARY_OPTIMIZED
  NAMES OgreMain Ogre
  PATHS $ENV{OGRE_HOME}
  PATH_SUFFIXES lib bin/Release bin/release Release release
)
FIND_LIBRARY(OGRE_LIBRARY_DEBUG
  NAMES OgreMaind OgreMain_d OgreMainD OgreMain_D Ogred Ogre_d OgreD Ogre_d
  PATHS $ENV{OGRE_HOME}
  PATH_SUFFIXES lib bin/Debug bin/debug Debug debug Versions/A
)

# Inspect OgrePrerquisites.h for the version number
DETERMINE_VERSION(OGRE ${OGRE_INCLUDE_DIR}/OgrePrerequisites.h)

# Handle the REQUIRED argument and set OGRE_FOUND
# Also check the version requirements
FIND_PACKAGE_HANDLE_ADVANCED_ARGS(OGRE DEFAULT_MSG ${OGRE_VERSION}
  OGRE_LIBRARY_OPTIMIZED
  OGRE_INCLUDE_DIR
)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(OGRE)

MARK_AS_ADVANCED(
  OGRE_INCLUDE_DIR
  OGRE_LIBRARY_OPTIMIZED
  OGRE_LIBRARY_DEBUG
)
