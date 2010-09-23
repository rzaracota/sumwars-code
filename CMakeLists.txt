CMAKE_MINIMUM_REQUIRED( VERSION 2.6.0)

cmake_policy(SET CMP0011 NEW)

IF( NOT CMAKE_BUILD_TYPE )
SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

OPTION(NO_RAKNET "Avoid use of Raknet Lib" OFF)
PROJECT(SUMWARS)

# for unknown reasons, sumwars target was always built first, leading to errors
# SET(RakNet_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Raknet2_0/libRakNet.a)
# ADD_SUBDIRECTORY(${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Raknet2_0)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "5")
SET(VERSION_PATCH "2")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

FIND_PACKAGE(Lua51 REQUIRED)
FIND_PACKAGE(OGRE 1.6 REQUIRED)
FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(CEGUI REQUIRED)
FIND_PACKAGE(CEGUIOGRE REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED)
FIND_PACKAGE(Enet REQUIRED)
FIND_PACKAGE(Pthreads REQUIRED)
#if(NOT NO_RAKNET)
#	FIND_PACKAGE(RakNet REQUIRED)
#else()
#	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -DNO_RAKNET)
#endif()
FIND_PACKAGE(ALUT REQUIRED)
FIND_PACKAGE(OggVorbis REQUIRED)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tinyxml ${PROJECT_SOURCE_DIR}/src/core ${PROJECT_SOURCE_DIR}/src/gui)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/./Dependencies/nlfg )
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CEGUIOGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDES})
INCLUDE_DIRECTORIES(${ENET_INCLUDES})
INCLUDE_DIRECTORIES(${PTHREADS_INCLUDE_DIR})

#if(NOT NO_RAKNET)
#	INCLUDE_DIRECTORIES(${RakNet_INCLUDES})
#endif()

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
  ./src/gui/minimapwindow.cpp
  ./src/gui/worldmap.cpp
  ./src/gui/mainmenu.cpp
  ./src/gui/ogg.cpp
  ./src/gui/charcreate.cpp
  ./src/gui/tooltip.cpp
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
  ./src/tinyxml/tinystr.cpp
  ./src/tinyxml/tinyxmlerror.cpp
  ./src/tinyxml/tinyxml.cpp
  ./src/tinyxml/tinyxmlparser.cpp
)

ADD_EXECUTABLE(sumwars ${SRCS})
TARGET_LINK_LIBRARIES(sumwars ${OGRE_LIBRARIES} ${OIS_LIBRARY_DBG} ${CEGUI_LIBRARIES} ${CEGUIOGRE_LIBRARIES} ${LUA_LIBRARIES} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} ${Enet_LIBRARY} ${PTHREADS_LIBRARY} ${OGGVORBIS_LIBRARIES})

INSTALL(TARGETS sumwars
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

FILE(GLOB_RECURSE DATA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/data/" "*.*" EXCLUDE ".svn")
FILE(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/" "*.*" EXCLUDE ".svn")

INSTALL(FILES ${DATA_FILES} DESTINATION share/sumwars/data)
INSTALL(FILES ${RESOURCE_FILES} DESTINATION share/sumwars/resources)
INSTALL(FILES ogre.cfg plugins.cfg DESTINATION share/sumwars/)
