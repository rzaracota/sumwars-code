#/*
#-----------------------------------------------------------------------------
#Filename:    CMakeLists.txt
#-----------------------------------------------------------------------------
#
#This source file is part of the
#   ___                 __    __ _ _    _ 
#  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
# //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
#/ \_// (_| | | |  __/  \  /\  /| |   <| |
#\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
#      |___/                              
#	  Tutorial Framework
#	  http://www.ogre3d.org/tikiwiki/
#-----------------------------------------------------------------------------
#*/
cmake_minimum_required(VERSION 2.6)


IF( NOT CMAKE_BUILD_TYPE )
SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

OPTION(NO_RAKNET "Avoid use of Raknet Lib" OFF)
PROJECT(SUMWARS)

project(sumwars)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "5")
SET(VERSION_PATCH "2")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")


	
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

IF(WIN32)
	set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH}")
ENDIF(WIN32)

IF(UNIX)
	SET(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
ENDIF(UNIX)

IF (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  SET(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
ENDIF ()

SET(CMAKE_DEBUG_POSTFIX "_d")

SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

FIND_PACKAGE(OGRE REQUIRED)
 
IF(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
  MESSAGE(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
ENDIF()

FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(CEGUIOGRE REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Enet REQUIRED)
FIND_PACKAGE(Pthreads REQUIRED)
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(Ogg REQUIRED)
FIND_PACKAGE(Vorbis REQUIRED)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tinyxml)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/core)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui/debugpanel)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Dependencies/nlfg )
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CEGUIOGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDES})
INCLUDE_DIRECTORIES(${ENET_INCLUDES})
INCLUDE_DIRECTORIES(${PTHREADS_INCLUDE_DIR})
IF(WIN32)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/gettext/include/)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/)
	ADD_DEFINITIONS( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4800 /wd4244 /wd4305 /nologo" )
	MESSAGE("WARNING: Compiler warnings 4800, 4244 and 4305 disabled!")
ENDIF()




SET(SRCS
  ./Dependencies/nlfg/nlfg.c
  ./src/core/worldobject.cpp
  ./src/core/itemloader.cpp
  ./src/core/itemlist.cpp
  ./src/core/region.cpp
  ./src/core/objectloader.cpp
  ./src/core/player.cpp
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
  ./src/gui/debugpanel/debugpanel.cpp
  ./src/gui/debugpanel/guidebugtab.cpp
  ./src/gui/debugpanel/luascripttab.cpp
  ./src/gui/debugpanel/textfileeditwindow.cpp
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
  ./src/gui/optionswindow.cpp
  ./src/gui/debugpanel/debugpanel.cpp
  ./src/gui/debugpanel/guidebugtab.cpp
  ./src/gui/debugpanel/luascripttab.cpp
  ./src/gui/debugpanel/textfileeditwindow.cpp
  ./src/tinyxml/tinystr.cpp
  ./src/tinyxml/tinyxmlerror.cpp
  ./src/tinyxml/tinyxml.cpp
  ./src/tinyxml/tinyxmlparser.cpp
)
 


ADD_EXECUTABLE(sumwars ${HDRS} ${SRCS})

SET(LINK_LIBS
	${OGRE_LIBRARIES} 
	${OIS_LIBRARIES} 
	${CEGUI_LIBRARY} 
	${CEGUIOGRE_LIBRARY} 
	${LUA_LIBRARIES} 
	${OPENAL_LIBRARY} 
	${ALUT_LIBRARY} 
	${RakNet_LIBRARY} 
	${VORBIS_LIBRARIES} 
	${VORBIS_LIBRARIES}
	${Enet_LIBRARY}
)

IF(WIN32)
	SET(LINK_LIBS
		${LINK_LIBS}	
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/gettext/libgettextlib.lib
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/gettext/libintl.lib
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/pthreadVC2.lib
		#${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/lib/libboost_thread-vc90-mt-gd-1_42.lib
		#${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/lib/libboost_date_time-vc90-mt-gd-1_42.lib
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/lib/libboost_thread-vc90-mt-1_42.lib
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/lib/libboost_date_time-vc90-mt-1_42.lib
		ws2_32.lib )
ENDIF()

TARGET_LINK_LIBRARIES(sumwars ${LINK_LIBS})
























