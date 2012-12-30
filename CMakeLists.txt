# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.6)
project(sumwars)

MESSAGE(STATUS "")
MESSAGE(STATUS "### Welcome to Summoning Wars build system ###")
MESSAGE(STATUS "**********************************************")
MESSAGE(STATUS "  http://sumwars.org/wiki/Build_instructions  ")
MESSAGE(STATUS "")

OPTION(SUMWARS_NO_TINYXML "Do not use bundled, in-source TinyXML Lib" OFF)
OPTION(SUMWARS_NO_ENET "Do not use bundled, in-source ENET Lib" OFF)
OPTION(SUMWARS_BUILD_TOOLS "Builds editor tools (not needed for gameplay, only useful for content authors)" ON)
OPTION(SUMWARS_BUILD_WITH_ONLINE_SERVICE "Builds enables the online services)" ON)
OPTION(SUMWARS_UPDATE_HG_REVISION "Option to enable or disable automatic revision checking" ON)
OPTION(SUMWARS_POST_BUILD_COPY "After a build, copy the executable to the project folder (Win32)" OFF)
OPTION(SUMWARS_STANDALONE_MODE "(Highly recommended for deployment on Windows and MacOSX) In case this is enabled, all paths (share, doc, bin, plugins) are set to be relative" OFF)
OPTION(SUMWARS_PORTABLE_MODE "In case this is enabled, save files and config files are kept in application directory instead of home folders / user folders. On Windows this also means that all dynamic libraries will be relative to the executable" OFF)
OPTION(SUMWARS_RANDOM_REGIONS "Regions are randomized if enabled. All regions are always the same if you leave this disabled (useful for developers)." ON)

IF(SUMWARS_UPDATE_HG_REVISION)
    FIND_PROGRAM(Mercurial_EXECUTABLE hg)

    SET(Project_WC_REVISION "unknown")

    IF(Mercurial_EXECUTABLE)
        EXECUTE_PROCESS(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                        COMMAND ${Mercurial_EXECUTABLE} log -l 1 --template "{node|short}"
                        OUTPUT_VARIABLE Project_WC_REVISION
        )
    ENDIF()
ENDIF()

SET(SUMWARS_VERSION_MAJOR "0")
SET(SUMWARS_VERSION_MINOR "5")
SET(SUMWARS_VERSION_PATCH "7")
SET(SUMWARS_VERSION_REVISION ${Project_WC_REVISION})
SET(SUMWARS_SHORT_VERSION "${SUMWARS_VERSION_MAJOR}.${SUMWARS_VERSION_MINOR}.${SUMWARS_VERSION_PATCH}")
SET(SUMWARS_VERSION "${SUMWARS_SHORT_VERSION}.hg-${SUMWARS_VERSION_REVISION}")
CONFIGURE_FILE("src/version.h.in" "src/version.h")

MESSAGE(STATUS "Version: ${SUMWARS_VERSION}")
MESSAGE(STATUS "Short Version: ${SUMWARS_SHORT_VERSION}")

if(UNIX)
  set(SUMWARS_EXECUTABLE_DEFAULT_DIR "bin")
else()
  set(SUMWARS_EXECUTABLE_DEFAULT_DIR "")
endif()

SET(SUMWARS_EXECUTABLE_DIR "${SUMWARS_EXECUTABLE_DEFAULT_DIR}" CACHE STRING "Path that represents the directory with sumwars executable, CMAKE_INSTALL_PREFIX gets prepended! (Only applies when SUMWARS_STANDALONE_MODE is OFF)")

if(UNIX)
  set(SUMWARS_SHARE_DEFAULT_DIR "share/sumwars")
else()
  set(SUMWARS_SHARE_DEFAULT_DIR "share")
endif()

SET(SUMWARS_SHARE_DIR "${SUMWARS_SHARE_DEFAULT_DIR}" CACHE STRING "Path that represents the directory with sumwars data, CMAKE_INSTALL_PREFIX gets prepended! (Only applies when SUMWARS_STANDALONE_MODE is OFF)")

if(UNIX)
  set(SUMWARS_DOC_DEFAULT_DIR "share/doc/sumwars-${SUMWARS_SHORT_VERSION}")
else()
  set(SUMWARS_DOC_DEFAULT_DIR "doc")
endif()

SET(SUMWARS_DOC_DIR "${SUMWARS_DOC_DEFAULT_DIR}" CACHE STRING "Path that represents the directory with sumwars documentation, CMAKE_INSTALL_PREFIX gets prepended! (Only applies when SUMWARS_STANDALONE_MODE is OFF)")

set(SUMWARS_LANGUAGES_DEFAULT "*")
SET(SUMWARS_LANGUAGES "${SUMWARS_LANGUAGES_DEFAULT}" CACHE STRING "Language codes of the translations which will be copied upon 
install ('*' matches all available translations).")

IF( NOT SUMWARS_LANGUAGES )
  set(SUMWARS_LANGUAGES ${SUMWARS_LANGUAGES_DEFAULT})
ENDIF()

IF( NOT CMAKE_BUILD_TYPE )
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.

  SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
# separating newline before configuration really happens
MESSAGE(STATUS "")

IF( SUMWARS_STANDALONE_MODE )
  MESSAGE("Standalone mode enabled, I am overriding all paths!")
  
  # In case of a standalone mode, we override all paths to our hardcoded, arbitrarily chosen values.
  # There is no point in making these customisable, I think.
  
  #SET(CMAKE_INSTALL_PREFIX "./")
  SET(SUMWARS_PREFIX "./")
  
  SET(SUMWARS_EXECUTABLE_DIR "./")
  SET(SUMWARS_SHARE_DIR "./share")
  SET(SUMWARS_DOC_DIR "./")
  
  # We recommend enable post build copy since that will make it run immediately after building
  IF ( NOT SUMWARS_POST_BUILD_COPY )
    MESSAGE(STATUS "You might be interested in turning SUMWARS_POST_BUILD_COPY ON to make sumwars run immediately after building from the source folder.")
  ENDIF()
  
  MESSAGE(STATUS "")
ELSE()
  SET(SUMWARS_PREFIX ${CMAKE_INSTALL_PREFIX})
ENDIF()

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

IF(WIN32)
  SET(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
ENDIF(WIN32)

IF(UNIX)
  SET(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
ENDIF(UNIX)

SET(CMAKE_DEBUG_POSTFIX "_d")

if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")
endif()

FIND_PACKAGE(OGRE REQUIRED)

IF("${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
  set(SUMWARS_OGRE_SINGLETON "ms_Singleton")
ELSEIF("${OGRE_VERSION_NAME}" STREQUAL "Byatis")
  set(SUMWARS_OGRE_SINGLETON "msSingleton")
ELSE()
  MESSAGE(SEND_ERROR "You need Ogre 1.7 Cthugha or 1.8 Byatis to build this.")
ENDIF()

SET(SUMWARS_OGRE_PLUGIN_DIR_REL ${OGRE_PLUGIN_DIR_REL}) 
SET(SUMWARS_OGRE_PLUGIN_DIR_DBG ${OGRE_PLUGIN_DIR_DBG}) 

IF( WIN32 )# On Windows the plugins are distributed with the executable.
  IF( SUMWARS_PORTABLE_MODE OR SUMWARS_STANDALONE_MODE)
    SET(SUMWARS_OGRE_PLUGIN_DIR_REL "./") 
	SET(SUMWARS_OGRE_PLUGIN_DIR_DBG "./") 
  ENDIF()
ENDIF()



# add our build include directory
include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/src/gui"
)

IF (WIN32)
 FIND_PACKAGE(BoostOGRE REQUIRED)
 FIND_PACKAGE(GNUWIN REQUIRED)
ENDIF (WIN32)
FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(CEGUIOGRE REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(Ogg REQUIRED)
FIND_PACKAGE(Vorbis REQUIRED)
FIND_PACKAGE(PhysFS REQUIRED)
IF(SUMWARS_BUILD_TOOLS OR SUMWARS_BUILD_WITH_ONLINE_SERVICE)
  FIND_PACKAGE(POCO REQUIRED)
ENDIF()
IF(SUMWARS_NO_TINYXML)
  FIND_PACKAGE(TinyXML REQUIRED)
ENDIF()
IF(SUMWARS_NO_ENET)
  FIND_PACKAGE(Enet REQUIRED)
ENDIF()

IF (UNIX AND NOT APPLE)
  FIND_PACKAGE(X11 REQUIRED)
ENDIF()

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/core)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui/contenteditor)
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OGRE_RTShaderSystem_INCLUDE_DIR})
IF (WIN32)
  Message(STATUS "Adding boost include: ${BoostOGRE_INCLUDE_DIR}")
  INCLUDE_DIRECTORIES(${BoostOGRE_INCLUDE_DIR})
  Message(STATUS "Adding gnu include: ${GNUWIN_INCLUDE_DIR}")
  INCLUDE_DIRECTORIES(${GNUWIN_INCLUDE_DIR})
ENDIF (WIN32)
INCLUDE_DIRECTORIES(${OIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUIOGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OGG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${VORBIS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${PHYSFS_INCLUDE_DIR})
IF(SUMWARS_NO_TINYXML)
  INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
ELSE()
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tinyxml)
ENDIF()
IF(SUMWARS_NO_ENET)
  INCLUDE_DIRECTORIES(${ENET_INCLUDE_DIR})
ELSE()
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/enet/include)
ENDIF()
IF(SUMWARS_BUILD_TOOLS OR SUMWARS_BUILD_WITH_ONLINE_SERVICE)
  IF(SUMWARS_BUILD_TOOLS)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui/debugpanel)
    INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui/debugpanel/filebrowser)
  ENDIF()
  INCLUDE_DIRECTORIES(${POCO_INCLUDE_DIR})
  if(SUMWARS_BUILD_WITH_ONLINE_SERVICE)
    INCLUDE_DIRECTORIES(${POCO_Net_INCLUDE_DIR})
  ENDIF(SUMWARS_BUILD_WITH_ONLINE_SERVICE)

ENDIF()

IF(WIN32)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/dirent)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/gettext/include/)
  ADD_DEFINITIONS( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4800 /wd4244 /wd4305 /nologo" )
  MESSAGE("WARNING: Compiler warnings 4800, 4244 and 4305 disabled!")
ELSE()
  INCLUDE_DIRECTORIES(${PTHREADS_INCLUDE_DIR})
ENDIF()

IF(UNIX AND NOT APPLE)
  INCLUDE_DIRECTORIES(${X11_X11_INCLUDE_PATH})
  INCLUDE_DIRECTORIES(${X11_Xrandr_INCLUDE_PATH})
ENDIF()

FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/AUTHORS SUMWARS_AUTHORS_CONTENT)
STRING(REPLACE \n \\n SUMWARS_AUTHORS_CONTENT "${SUMWARS_AUTHORS_CONTENT}")

CONFIGURE_FILE("src/config.h.in" "src/config.h")
CONFIGURE_FILE("src/gui/default_resources.cfg.inc.in" "src/gui/default_resources.cfg.inc")
CONFIGURE_FILE("src/gui/creditswindow_content.inc.in" "src/gui/creditswindow_content.inc")

FILE(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

IF(SUMWARS_NO_TINYXML)
  SET(SRCS_TINYXML "")
ELSE()
  SET(SRCS_TINYXML
    ./src/tinyxml/tinystr.cpp
    ./src/tinyxml/tinyxmlerror.cpp
    ./src/tinyxml/tinyxml.cpp
    ./src/tinyxml/tinyxmlparser.cpp)
ENDIF()

IF(SUMWARS_NO_ENET)
  SET(SRCS_ENET "")
ELSE()
  SET(SRCS_ENET
    ./src/enet/callbacks.c
    ./src/enet/compress.c
    ./src/enet/host.c
    ./src/enet/list.c
    ./src/enet/packet.c
    ./src/enet/peer.c
    ./src/enet/protocol.c
    ./src/enet/unix.c
    ./src/enet/win32.c
    )
ENDIF()

SET(SRCS
  ${SRCS_TINYXML}
  ${SRCS_ENET}
  ${HEADER_FILES}
  ./src/core/nlfg.cpp
  ./src/core/worldobject.cpp
  ./src/core/itemloader.cpp
  ./src/core/itemlist.cpp
  ./src/core/region.cpp
  ./src/core/objectloader.cpp
  ./src/core/player.cpp
  ./src/core/monsterbase.cpp
  ./src/core/monster.cpp
  ./src/core/itemfactory.cpp
  ./src/core/translatablestring.cpp
  ./src/core/event.cpp
  ./src/core/charconv.cpp
  ./src/core/stdstreamconv.cpp
  ./src/core/gameobject.cpp
  ./src/core/projectile.cpp
  ./src/core/treasure.cpp
  ./src/core/dialogue.cpp
  ./src/core/playerbase.cpp
  ./src/core/objectfactory.cpp
  ./src/core/gettext.cpp
  ./src/core/dropitem.cpp
  ./src/core/party.cpp
  ./src/core/minimap.cpp
  ./src/core/logger.cpp
  ./src/core/item.cpp
  ./src/core/networkstruct.cpp
  ./src/core/gridunit.cpp
  ./src/core/quest.cpp
  ./src/core/eventsystem.cpp
  ./src/core/creature.cpp
  ./src/core/random.cpp
  ./src/core/action.cpp
  ./src/core/creaturestruct.cpp
  ./src/core/servernetwork.cpp
  ./src/core/nlfgservernetwork.cpp
  ./src/core/nlfgclientnetwork.cpp
  ./src/core/nlfgnetworkpacket.cpp
  ./src/core/pathfind.cpp
  ./src/core/damage.cpp
  ./src/core/waypoint.cpp
  ./src/core/document.cpp
  ./src/core/debug.cpp
  ./src/core/unionfindtree.cpp
  ./src/core/world.cpp
  ./src/core/fixedbase.cpp
  ./src/core/fixedobject.cpp
  ./src/core/templateloader.cpp
  ./src/core/elementattrib.cpp
  ./src/core/worldloader.cpp
  ./src/core/geometry.cpp
  ./src/core/scriptobject.cpp
  ./src/core/spawnpoint.cpp
  ./src/core/fraction.cpp
  ./src/core/command.cpp
  ./src/core/mapgenerator.cpp
  ./src/core/options.cpp
  ./src/core/sumwarshelper.cpp
  ./src/gui/minimapwindow.cpp
  ./src/gui/worldmap.cpp
  ./src/gui/mainmenu.cpp
  ./src/gui/ogg.cpp
  ./src/gui/charcreate.cpp
  ./src/gui/ceguiutility.cpp
  ./src/gui/tooltipmanager.cpp
  ./src/gui/music.cpp
  ./src/gui/controlpanel.cpp
  ./src/gui/inventory.cpp
  ./src/gui/tradewindow.cpp
  ./src/gui/dialoguewindow.cpp
  ./src/gui/partyinfo.cpp
  ./src/gui/networkwindows.cpp
  ./src/gui/creditswindow.cpp
  ./src/gui/mainwindow.cpp
  ./src/gui/application.cpp
  ./src/gui/skilltree.cpp
  ./src/gui/chatline.cpp
  ./src/gui/charinfo.cpp
  ./src/gui/itemwindow.cpp
  ./src/gui/savegamelist.cpp
  ./src/gui/scene.cpp
  ./src/gui/graphicmanager.cpp
  ./src/gui/main_gui.cpp
  ./src/gui/messageboxes.cpp
  ./src/gui/graphicobjectrenderinfo.cpp
  ./src/gui/graphicobject.cpp
  ./src/gui/questinfo.cpp
  ./src/gui/sound.cpp
  ./src/gui/tooltip.cpp
  ./src/gui/optionswindow.cpp
  ./src/gui/clipboard.cpp
#   ./src/gui/ShaderManager.cpp
#   ./src/gui/ShadowCameraSetup.cpp
)
 
IF(SUMWARS_BUILD_TOOLS)
  SET(SRCS
    ${SRCS}
    ${HEADER_FILES}
    ./src/gui/debugpanel/benchmarktab.cpp
    ./src/gui/debugpanel/debugpanel.cpp
    ./src/gui/debugpanel/debugcameratab.cpp
    ./src/gui/debugpanel/iconeditortab.cpp
    ./src/gui/debugpanel/guidebugtab.cpp
    ./src/gui/debugpanel/luascripttab.cpp
    ./src/gui/debugpanel/textfileeditwindow.cpp
    ./src/gui/debugpanel/filebrowser/filebrowser.cpp
    #./src/gui/debugpanel/reloadtab.cpp
    ./src/gui/contenteditor/contenteditor.cpp
    ./src/gui/contenteditor/contenteditortab.cpp
    ./src/gui/contenteditor/fixedobjecteditor.cpp
    ./src/gui/contenteditor/renderinfoeditor.cpp
    ./src/gui/contenteditor/itemeditor.cpp
    ./src/gui/contenteditor/monstereditor.cpp
    )
ENDIF()

IF(SUMWARS_BUILD_WITH_ONLINE_SERVICE)
  SET(SRCS
    ${SRCS}
    ${HEADER_FILES}
    ./src/core/onlineservicesmanager.cpp
    )
ENDIF()

# If building for windows based systems, the resource file (containing 
# the manifest, icon and other resources) should also be included
IF (WIN32)
  SET(SRCS 
    ${SRCS}
    ./src/winicon/Sumwars.rc
    )
  SET(HEADER_FILES
    ${HEADER_FILES}
    ./src/winicon/resource.h
    )
ENDIF (WIN32)

ADD_EXECUTABLE(sumwars ${SRCS})


#${CEGUILUA_LIBRARY}


SET(LINK_LIBS
  ${OGRE_LIBRARIES}
  ${OGRE_Terrain_LIBRARY}
  ${OGRE_RTShaderSystem_LIBRARY}
  ${OIS_LIBRARIES} 
  ${CEGUI_LIBRARY} 
  ${CEGUIOGRE_LIBRARY}
  ${LUA_LIBRARIES} 
  ${OPENAL_LIBRARY} 
  ${ALUT_LIBRARY} 
  ${OGG_LIBRARY}
  ${VORBIS_LIBRARIES} 
  ${PHYSFS_LIBRARY}
  )
IF(SUMWARS_NO_TINYXML)
  SET(LINK_LIBS ${LINK_LIBS} ${TINYXML_LIBRARIES})
ENDIF()
IF(SUMWARS_NO_ENET)
  SET(LINK_LIBS ${LINK_LIBS} ${Enet_LIBRARY})
ENDIF()

IF(SUMWARS_BUILD_TOOLS OR SUMWARS_BUILD_WITH_ONLINE_SERVICE)
  SET(LINK_LIBS
    ${LINK_LIBS}
    ${POCO_LIBRARIES}
    )
  IF(SUMWARS_BUILD_TOOLS)
    ADD_DEFINITIONS(-DSUMWARS_BUILD_TOOLS)
  ENDIF()
  IF(SUMWARS_BUILD_WITH_ONLINE_SERVICE)
    ADD_DEFINITIONS(-DSUMWARS_BUILD_WITH_ONLINE_SERVICES)
    SET(LINK_LIBS
      ${LINK_LIBS}
      ${POCO_Net_LIBRARY}
      )
  ENDIF()
ENDIF()

IF(WIN32)
  SET(LINK_LIBS
    ws2_32.lib
    winmm.lib
    ${LINK_LIBS}
    ${GNUWIN_LIBRARY}
    ${BoostOGRE_LIBRARIES}
    )
ELSEIF(UNIX AND NOT APPLE)
  SET(LINK_LIBS
    ${X11_X11_LIB}
    ${X11_Xrandr_LIB}
    ${LINK_LIBS}
    )
ENDIF()

TARGET_LINK_LIBRARIES(sumwars ${LINK_LIBS})

IF(SUMWARS_POST_BUILD_COPY)
  # Due to the fact that CMake can't yet target properly subconfigurations in multi-configuration environments
  # (such as Visual Studio), make a special case using Visual Studio macros.
  IF (MSVC)
    ADD_CUSTOM_COMMAND(TARGET sumwars
      POST_BUILD
      COMMAND copy $(OutDir)$(TargetFileName) $(ProjectDir)$(TargetFileName)
    )
  ELSE ()
    ADD_CUSTOM_COMMAND(TARGET sumwars
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/sumwars${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_SOURCE_DIR}
    )
  ENDIF (MSVC)
ENDIF()

# install the application
install(TARGETS sumwars
  RUNTIME DESTINATION "${SUMWARS_EXECUTABLE_DIR}")

# freedesktop.org icons and desktop files
# TODO: How do we do this on Windows? Installing start menu items.
#if (UNIX)
#  install(FILES "packaging/sumwars.desktop"
#    DESTINATION "share/applications")
#    
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_16x16.png"
#    DESTINATION "share/icons/hicolor/16x16/"
#    RENAME "sumwars.png")
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_24x24.png"
#    DESTINATION "share/icons/hicolor/24x24/"
#    RENAME "sumwars.png")
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_32x32.png"
#    DESTINATION "share/icons/hicolor/32x32/"
#    RENAME "sumwars.png")
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_48x48.png"
#    DESTINATION "share/icons/hicolor/48x48/"
#    RENAME "sumwars.png")
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_64x64.png"
#    DESTINATION "share/icons/hicolor/64x64/"
#    RENAME "sumwars.png")
#  install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/share/icon/SumWarsIcon_128x128.png"
#    DESTINATION "share/icons/hicolor/128x128/"
#    RENAME "sumwars.png")
#endif()

install(FILES "AUTHORS" "COPYING" "README" # we intentionally avoid installing "INSTALL" here
  DESTINATION "${SUMWARS_DOC_DIR}")
  
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/data"
  DESTINATION "${SUMWARS_SHARE_DIR}"
  PATTERN ".hg" EXCLUDE)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/resources"
  DESTINATION "${SUMWARS_SHARE_DIR}"
  PATTERN ".hg" EXCLUDE
  PATTERN "icon" EXCLUDE)
if ("${SUMWARS_LANGUAGES}" STREQUAL "*")
  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/translation"
    DESTINATION "${SUMWARS_SHARE_DIR}"
    FILES_MATCHING PATTERN "*.mo"
    PATTERN ".hg" EXCLUDE
    PATTERN "xmlgettext" EXCLUDE) # both pattern excludes are there to prevent the directories from being created
else()
  SEPARATE_ARGUMENTS(SUMWARS_LANGUAGES)
  foreach(i ${SUMWARS_LANGUAGES})
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/share/translation/${i}"
      DESTINATION "${SUMWARS_SHARE_DIR}/translation"
      FILES_MATCHING PATTERN "*.mo"
      PATTERN ".hg" EXCLUDE
      PATTERN "xmlgettext" EXCLUDE) # both pattern excludes are there to prevent the directories from being created
  endforeach()
endif()
  
if(WIN32)
	STRING(REGEX REPLACE "cegui/include" "bin" CEGUI_BIN_DIR ${CEGUI_INCLUDE_DIR})
	STRING(REGEX REPLACE ".lib" ".dll" LUA_BIN_DIR ${LUA_LIBRARY})
	STRING(REGEX REPLACE "alut.lib" "alut.dll" ALUT_BIN_DIR ${ALUT_LIBRARY_OPTIMIZED})
	STRING(REGEX REPLACE "lib/physfs.lib" "bin/physfs.dll" PHYSFS_BIN_DIR ${PHYSFS_LIBRARY})
	STRING(REGEX REPLACE "lib/PocoFoundation.lib" "bin/PocoFoundation.dll" POCO_BIN_DIR ${POCO_LIBRARY_REL})
	MESSAGE(${POCO_BIN_DIR})
	
install(DIRECTORY "${OGRE_PLUGIN_DIR_REL}/"
  DESTINATION "."
  FILES_MATCHING PATTERN "*.dll")
  
  install(FILES 
  "${CEGUI_BIN_DIR}/CEGUIBase.dll"
  "${CEGUI_BIN_DIR}/CEGUIOgreRenderer.dll"
  "${CEGUI_BIN_DIR}/CEGUIExpatParser.dll"
  "${CEGUI_BIN_DIR}/CEGUIFalagardWRBase.dll"
  "${LUA_BIN_DIR}"
  "${ALUT_BIN_DIR}"
  "${PHYSFS_BIN_DIR}"
  "${POCO_BIN_DIR}"
  DESTINATION ".")
endif()
  
# only CPack code below this line
SET(CPACK_PACKAGE_NAME "sumwars")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Summoning Wars Role-Playing Game")
SET(CPACK_PACKAGE_VENDOR "Sumwars team")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR "${SUMWARS_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${SUMWARS_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${SUMWARS_VERSION_PATCH}")

SET(CPACK_SOURCE_GENERATOR TBZ2 ZIP)
SET(CPACK_GENERATOR NSIS DEB)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Sumwars team")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libogremain-1.7.3, libcegui, libois-1.3.0, libalut0, liblua5.1-0, libopenal1, libvorbis0a, libfreeimage3, libphysfs1, libxrandr2")
SET(CPACK_DEBIAN_PACKAGE_SECTION "games")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_SET_DESTDIR "ON")


set(CPACK_SOURCE_PACKAGE_FILE_NAME "sumwars-${SUMWARS_SHORT_VERSION}" CACHE INTERNAL "tarball basename")

set(CPACK_SOURCE_IGNORE_FILES
 # repository history should not be in source tarballs
"\\\\.hg.*"

# the rest is modelled after .hgignore
"build/"
"~$"

"\\\\.project"
"\\\\.cproject"
"\\\\.pydevproject"

"sumwars$"
"sumwars.exe$"

"\\\\.sumwars"
)
INCLUDE(CPack)
