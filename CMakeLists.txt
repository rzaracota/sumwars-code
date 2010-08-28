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

IF(NOT WIN32)
	ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Raknet2_0) #For WIN32 RakNet comes with the dependency package included
ENDIF()
	
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
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(Ogg REQUIRED)
FIND_PACKAGE(Vorbis REQUIRED)

SET(HDRS
  # CORE FILES
  ./src/core/action.h
  ./src/core/charconv.h
  ./src/core/clientnetwork.h
  ./src/core/command.h
  ./src/core/creature.h
  ./src/core/creaturestruct.h
  ./src/core/damage.h
  ./src/core/debug.h
  ./src/core/dialogue.h
  ./src/core/document.h
  ./src/core/dropitem.h
  ./src/core/dropslot.h
  ./src/core/elementattrib.h
  ./src/core/event.h
  ./src/core/eventsystem.h
  ./src/core/fixedbase.h
  ./src/core/fixedobject.h
  ./src/core/fraction.h
  ./src/core/gameobject.h
  ./src/core/geometry.h
  ./src/core/gettext.h
  ./src/core/gridunit.h
  ./src/core/hybridimplementation.h
  ./src/core/item.h
  ./src/core/itemfactory.h
  ./src/core/itemlist.h
  ./src/core/itemloader.h
  ./src/core/logger.h
  ./src/core/mapgenerator.h
  ./src/core/matrix2d.h
  ./src/core/mathhelper.h
  ./src/core/minimap.h
  ./src/core/monster.h
  ./src/core/monsterbase.h
  ./src/core/network.h
  ./src/core/networkstruct.h
  ./src/core/objectfactory.h
  ./src/core/objectloader.h
  ./src/core/options.h
  ./src/core/party.h
  ./src/core/pathfind.h
  ./src/core/player.h
  ./src/core/playerbase.h
  ./src/core/projectile.h
  ./src/core/projectilebase.h
  ./src/core/quest.h
  ./src/core/random.h
  ./src/core/region.h
  ./src/core/scriptobject.h
  ./src/core/servernetwork.h
  ./src/core/spawnpoint.h
  ./src/core/templateloader.h
  ./src/core/timer.h
  ./src/core/treasure.h
  ./src/core/treasurebase.h
  ./src/core/unionfindtree.h
  ./src/core/waypoint.h
  ./src/core/world.h
  ./src/core/worldloader.h
  ./src/core/worldobject.h
  # GUI FILES
  ./src/gui/debugpanel/debugpanel.h
  ./src/gui/debugpanel/debugtab.h
  ./src/gui/debugpanel/guidebugtab.h
  ./src/gui/debugpanel/luascripttab.h
  ./src/gui/debugpanel/textfileeditwindow.h
  ./src/gui/application.h
  ./src/gui/ceguiutility.h
  ./src/gui/charcreate.h
  ./src/gui/charinfo.h
  ./src/gui/chatline.h
  ./src/gui/controlpanel.h
  ./src/gui/creditswindow.h
  ./src/gui/dialoguewindow.h
  ./src/gui/graphicmanager.h
  ./src/gui/graphicobject.h
  ./src/gui/graphicobjectrenderinfo.h
  ./src/gui/inventory.h
  ./src/gui/itemwindow.h
  ./src/gui/listitem.h
  ./src/gui/mainmenu.h
  ./src/gui/mainwindow.h
  ./src/gui/messageboxes.h
  ./src/gui/minimapwindow.h
  ./src/gui/music.h
  ./src/gui/networkwindows.h
  ./src/gui/ogg.h
  ./src/gui/optionswindow.h
  ./src/gui/partyinfo.h
  ./src/gui/questinfo.h
  ./src/gui/savegamelist.h
  ./src/gui/scene.h
  ./src/gui/skilltree.h
  ./src/gui/sound.h
  ./src/gui/tooltipmanager.h
  ./src/gui/tradewindow.h
  ./src/gui/window.h
  ./src/gui/worldmap.h
  # TINYXML FILES
  ./src/tinyxml/tinystr.h
  ./src/tinyxml/tinyxml.h
)

SET(SRCS
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
  ./src/core/rakservernetwork.cpp
  ./src/core/rakclientnetwork.cpp
  ./src/core/raknetworkpacket.cpp
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
 
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tinyxml ${PROJECT_SOURCE_DIR}/src/core ${PROJECT_SOURCE_DIR}/src/gui ${PROJECT_SOURCE_DIR}/src/gui/debugpanel)
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUIOGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OGG_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${VORBIS_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Raknet2_0/Include)
IF(WIN32)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/boost_1_42/)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/gettext/include/)
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/pthreads/)
	ADD_DEFINITIONS( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4800 /wd4244 /wd4305 /nologo" )
	MESSAGE("WARNING: Compiler warnings 4800, 4244 and 4305 disabled!")
ENDIF()

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
		${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Raknet2_0/Lib/RakNetLibStatic.lib
		ws2_32.lib )
ENDIF()

TARGET_LINK_LIBRARIES(sumwars ${LINK_LIBS} /home/stefan/Dev/s07c/cegui07transition/build/Dependencies/Raknet2_0/libRakNet.a)
























