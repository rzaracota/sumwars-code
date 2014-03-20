# - Try to find Particle Universe
#  	Once done this will define
#
#	On Windows, this script will look for Particle Universe using the environment variable PUDIR
#	The variable should point to the install directory
#  
#	PARTICLE_UNIVERSE_FOUND - System has ParticleUniverse
#	PARTICLE_UNIVERSE_INCLUDE_DIRS - The ParticleUniverse include directories
#	PARTICLE_UNIVERSE_LIBRARIES - The libraries needed to use Particle Universe
#	PARTICLE_UNIVERSE_LIBRARIES_REL - The libraries needed to use Particle Universe
#	PARTICLE_UNIVERSE_LIBRARIES_DEBUG - The libraries needed to use Particle Universe
#	PARTICLE_UNIVERSE_DEFINITIONS - Compiler switches required for using Particle Universe

include(FindPkgMacros)
include(PreprocessorUtils)
findpkg_begin(ParticleUniverse)

STRING(REGEX REPLACE "\\\\" "/" PU_INSTALL_DIR $ENV{PUDIR})

find_path(PARTICLE_UNIVERSE_INCLUDE_DIR ParticleUniversePlugin.h
			HINTS ${PU_INSTALL_DIR}/include
)

find_library(PARTICLE_UNIVERSE_LIBRARY_REL NAMES Plugin_ParticleUniverse libPlugin_ParticleUniverse
             HINTS ${PC_PARTICLE_UNIVERSE_LIBDIR} ${PC_PARTICLE_UNIVERSE_LIBRARY_DIRS} ${PU_INSTALL_DIR}/lib/Release ${PU_INSTALL_DIR}/lib/RelWithDebInfo ${PU_INSTALL_DIR}/lib/MinSizeRel )
			 
find_library(PARTICLE_UNIVERSE_LIBRARY_DEBUG NAMES Plugin_ParticleUniverse_d libPlugin_ParticleUniverse_d
             HINTS ${PC_PARTICLE_UNIVERSE_LIBDIR} ${PC_PARTICLE_UNIVERSE_LIBRARY_DIRS} ${PU_INSTALL_DIR}/lib/Debug)

set(PARTICLE_UNIVERSE_LIBRARIES ${PARTICLE_UNIVERSE_LIBRARY_REL} ${PARTICLE_UNIVERSE_LIBRARY_DEBUG})
set(PARTICLE_UNIVERSE_LIBRARIES_REL ${PARTICLE_UNIVERSE_LIBRARY_REL}  )
set(PARTICLE_UNIVERSE_LIBRARIES_DEBUG ${PARTICLE_UNIVERSE_LIBRARY_DEBUG} )
set(PARTICLE_UNIVERSE_INCLUDE_DIRS ${PARTICLE_UNIVERSE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set PARTICLE_UNIVERSE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(ParticleUniverse  DEFAULT_MSG
                                  PARTICLE_UNIVERSE_LIBRARY_REL PARTICLE_UNIVERSE_LIBRARY_DEBUG PARTICLE_UNIVERSE_INCLUDE_DIR)

mark_as_advanced(PARTICLE_UNIVERSE_INCLUDE_DIR PARTICLE_UNIVERSE_LIBRARY_REL PARTICLE_UNIVERSE_LIBRARY_DEBUG )