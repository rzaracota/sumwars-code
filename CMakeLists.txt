CMAKE_MINIMUM_REQUIRED( VERSION 2.8.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")
include(FindPkgMacros)
include(PreprocessorUtils)

PROJECT(SUMWARS)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "6")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

add_subdirectory(${PROJECT_SOURCE_DIR}/Dependencies/Raknet2_0)

FIND_PACKAGE(OGRE REQUIRED) # Environment variable OGRE_BYATIS
FIND_PACKAGE(OIS REQUIRED)
FIND_PACKAGE(OpenAL REQUIRED) # Windows users: Create an environment variable named "OPENALDIR" and set the value to you OpenAL SDK Folder (e.g. "C:\Program Files\OpenAL 1.1 SDK")
FIND_PACKAGE(ALUT REQUIRED) # Windows users: should work when you use the dependency package from our website or when you install the ALUT SDK into the OpenAL folder (put the libs and includes in the same folder)
FIND_PACKAGE(CEGUI REQUIRED) # Dependencie Package
FIND_PACKAGE(CEGUIOGRE REQUIRED) #Dependencie Package 
FIND_PACKAGE(Lua51 REQUIRED) # Env variable "LUA_DIR"

IF (UNIX)
  FIND_PACKAGE(OggVorbis REQUIRED)
ELSE(WIN32)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Dependencies/libvorbis-1.3.1/include)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Dependencies/gettext/share/gettext)
  INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Dependencies/gettext/include)

  find_library(OGG_LIBRARY ${PROJECT_SOURCE_DIR}Dependencies/libvorbis-1.3.1/win32/VS2008/Win32/Release ogg)
  find_library(VORBIS_LIBRARY ${PROJECT_SOURCE_DIR}Dependencies/libvorbis-1.3.1/win32/VS2008/Win32/Release vorbis)
  find_library(VORBISFILE_LIBRARY ${PROJECT_SOURCE_DIR}Dependencies/libvorbis-1.3.1/win32/VS2008/Win32/Release vorbisfile)
  find_library(VORBISENC_LIBRARY ${PROJECT_SOURCE_DIR}Dependencies/libvorbis-1.3.1/win32/VS2008/Win32/Release vorbisenc)
ENDIF(UNIX)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/tinyxml)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/core)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/gui ${PROJECT_SOURCE_DIR}/src/gui/debugpanel)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/Dependencies/Raknet2_0/Include)
INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OIS_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LUA_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CEGUIOGRE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENAL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${ALUT_INCLUDE_DIR})

SET(SRCS
  ./src/core/worldobject.cpp
  ./src/core/itemloader.cpp
  ./src/core/itemlist.cpp
  ./src/core/region.cpp
  ./src/core/objectloader.cpp
  ./src/core/player.cpp
  ./src/core/network.cpp
  ./src/core/monster.cpp
  ./src/core/itemfactory.cpp
  ./src/core/event.cpp
  ./src/core/charconv.cpp
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
  ./src/core/pathfind.cpp
  ./src/core/damage.cpp
  ./src/core/waypoint.cpp
  ./src/core/document.cpp
  ./src/core/debug.cpp
  ./src/core/unionfindtree.cpp
  ./src/core/world.cpp
  ./src/core/clientnetwork.cpp
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
  ./src/tinyxml/tinystr.cpp
  ./src/tinyxml/tinyxmlerror.cpp
  ./src/tinyxml/tinyxml.cpp
  ./src/tinyxml/tinyxmlparser.cpp
)

SET(RakNet_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/Raknet2_0/libRakNet.a)

ADD_EXECUTABLE(sumwars ${SRCS})
TARGET_LINK_LIBRARIES(sumwars ${OGRE_LIBRARIES} ${OIS_LIBRARY_DBG} ${CEGUI_LIBRARIES} ${CEGUIOGRE_LIBRARIES} ${LUA_LIBRARIES} ${OPENAL_LIBRARY} ${ALUT_LIBRARY} ${RakNet_LIBRARY} ${OGGVORBIS_LIBRARIES})