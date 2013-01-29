# - Try to find ogg/vorbis
# Once done this will define
#
#  VORBIS_FOUND - system has vorbis
#  VORBIS_INCLUDE_DIR
#  VORBIS_LIBRARIES - vorbis and vorbisfile libraries
#
# $VORBISDIR is an environment variable used
# for finding vorbis.
#
# Several changes and additions by Fabian 'x3n' Landau
# Most of all rewritten by Adrian Friedli
# Debug versions and simplifications by Reto Grieder
#                 > www.orxonox.net <

INCLUDE(FindPackageHandleStandardArgs)
INCLUDE(HandleLibraryTypes)
INCLUDE(DetermineLibVorbisVersion)

FIND_PATH(VORBIS_INCLUDE_DIR vorbis/codec.h
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES include
)

FIND_PATH(VORBIS_SPEC_DIR libvorbis.spec
  PATHS $ENV{VORBISDIR} Dependencies/libvorbis131
  PATH_SUFFIXES src
)


Message (STATUS "Checking file ${VORBIS_SPEC_DIR}/libvorbis.spec")

# Check to see which Thread Provider is used by OGRE
DETERMINE_LIBVORBIS_VERSION(LIBVORBIS_VERSION ${VORBIS_SPEC_DIR}/libvorbis.spec)
Message (STATUS "Found LIBVORBIS_VERSION as: [${LIBVORBIS_VERSION}]")
# Set (LIBVORBIS_NUM_VERSION ${LIBVORBIS_VERSION})
String (REPLACE "." "0" LIBVORBIS_NUM_VERSION ${LIBVORBIS_VERSION})
Message (STATUS "Found LIBVORBIS_NUM_VERSION as: [${LIBVORBIS_NUM_VERSION}]")

IF(WIN32)
  #Make a special selection for the scenario of using visual studio 2010, since this will
  #be the most frequent at this stage.
  SET (TMP_SUFF Win32/Release)
  IF (MSVC10)
    SET (TMP_SUFF ${TMP_SUFF} VS2010/Win32/Release Win32/VS2010/Win32/Release )
  ENDIF (MSVC10)
  # Using static link in as preferred option
  # TODO: add option to allow selecting static or release as pre 
  FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
    NAMES libvorbis_static libvorbis-static-mt libvorbis
    PATHS $ENV{VORBISDIR}
    PATH_SUFFIXES Release ${TMP_SUFF}
  )
ELSE()
  FIND_LIBRARY(VORBIS_LIBRARY_OPTIMIZED
    NAMES vorbis
    PATH_SUFFIXES lib
  )
ENDIF(WIN32)
Message (STATUS "Got release vorbis: ${VORBIS_LIBRARY_OPTIMIZED}")

IF(WIN32)
#Make a special selection for the scenario of using visual studio 2010, since this will
#be the most frequent at this stage.
SET (TMP_SUFF Win32/Debug)
IF (MSVC10)
SET (TMP_SUFF ${TMP_SUFF} VS2010/Win32/Debug Win32/VS2010/Win32/Debug )
ENDIF (MSVC10)
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES libvorbis_static libvorbis vorbis_d vorbisD vorbis_D libvorbis-static-mt-debug
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug ${TMP_SUFF}
)
ELSE()
FIND_LIBRARY(VORBIS_LIBRARY_DEBUG
  NAMES vorbis vorbis_d vorbisD vorbis_D
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

IF(WIN32)
#Make a special selection for the scenario of using visual studio 2010, since this will
#be the most frequent at this stage.
SET (TMP_SUFF Win32/Release)
IF (MSVC10)
SET (TMP_SUFF ${TMP_SUFF} VS2010/Win32/Release Win32/VS2010/Win32/Release )
ENDIF (MSVC10)
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES libvorbisfile_static libvorbisfile
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Release ${TMP_SUFF}
)
ELSE()
FIND_LIBRARY(VORBISFILE_LIBRARY_OPTIMIZED
  NAMES vorbisfile
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

IF(WIN32)
#Make a special selection for the scenario of using visual studio 2010, since this will
#be the most frequent at this stage.
SET (TMP_SUFF Win32/Debug)
IF (MSVC10)
SET (TMP_SUFF ${TMP_SUFF} VS2010/Win32/Debug Win32/VS2010/Win32/Debug )
ENDIF (MSVC10)
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES libvorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATHS $ENV{VORBISDIR}
  PATH_SUFFIXES Debug ${TMP_SUFF}
)
ELSE()
FIND_LIBRARY(VORBISFILE_LIBRARY_DEBUG
  NAMES vorbisfile vorbisfile_d vorbisfileD vorbisfile_D
  PATH_SUFFIXES lib
)
ENDIF(WIN32)

# IF(NOT WIN32)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
    VORBIS_LIBRARY_OPTIMIZED
    VORBISFILE_LIBRARY_OPTIMIZED
    VORBIS_INCLUDE_DIR
  )
Else()
  # Starting with Libvorbis 1.3.2, vorbisfile needs to also be included in the windows build.
  IF(${LIBVORBIS_NUM_VERSION} GREATER 10301)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
      VORBIS_LIBRARY_OPTIMIZED
      VORBISFILE_LIBRARY_OPTIMIZED
      VORBIS_INCLUDE_DIR
    )
  ELSE()
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
      VORBIS_LIBRARY_OPTIMIZED
      VORBIS_INCLUDE_DIR
    )
  ENDIF(${LIBVORBIS_NUM_VERSION} GREATER 10301)

# Collect optimized and debug libraries
HANDLE_LIBRARY_TYPES(VORBIS)
  HANDLE_LIBRARY_TYPES(VORBISFILE)
SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY}) #WIN32 includes VORBISFILE as a static lib
  # Starting with Libvorbis 1.3.2, vorbisfile needs to also be included in the windows build.
  If(${LIBVORBIS_NUM_VERSION} GREATER 10301)
    HANDLE_LIBRARY_TYPES(VORBISFILE)
    SET(VORBIS_LIBRARIES ${VORBIS_LIBRARY} ${VORBISFILE_LIBRARY}) #WIN32 includes VORBISFILE as a static lib
  EndIf(${LIBVORBIS_NUM_VERSION} GREATER 10301)

MARK_AS_ADVANCED(
  VORBIS_INCLUDE_DIR
  VORBIS_LIBRARY_OPTIMIZED
  VORBIS_LIBRARY_DEBUG
  VORBISFILE_LIBRARY_OPTIMIZED
  VORBISFILE_LIBRARY_DEBUG
  Else()
    # Starting with Libvorbis 1.3.2, vorbisfile needs to also be included in the windows build.
    IF(${LIBVORBIS_NUM_VERSION} GREATER 10301)
      VORBISFILE_LIBRARY_OPTIMIZED
      VORBISFILE_LIBRARY_DEBUG
    ENDIF(${LIBVORBIS_NUM_VERSION} GREATER 10301)
)
