AUTOMAKE_OPTIONS = foreign subdir-objects

##LIBS = OGRE OIS CEGUI CEGUI-OGRE RakNet freealut openal
##LDFLAGS = $(shell pkg-config --libs $(LIBS))
##LOADLIBES =
##LDLIBS = $(LDFLAGS) -lm -lrt
##CFLAGS = -Wall -g
##CXXFLAGS = $(shell pkg-config --cflags $(LIBS)) $(CFLAGS)
##CXXFLAGS += -I./core -I./gui -I./tinyxml
##OBJS = $(shell cat ./Makefile.objs )

bin_PROGRAMS = sumwars

#LIBS = OGRE OIS CEGUI CEGUI-OGRE RakNet freealut openal
#LDFLAGS = $(shell pkg-config --libs $(LIBS))
LDFLAGS = $(shell pkg-config --libs OGRE OIS CEGUI CEGUI-OGRE RakNet freealut openal lua5.1)

sumwars_LDFLAGS = $(LDFLAGS) -lm -lrt

CFLAGS = -Wall -g

#AM_CXXFLAGS = -DDEBUG_LEVEL=5 $(shell pkg-config --cflags $(LIBS)) $(CFLAGS) -I./core -I./gui -I./tinyxml
AM_CXXFLAGS = -DDEBUG_LEVEL=5 $(shell pkg-config --cflags OGRE OIS CEGUI CEGUI-OGRE RakNet freealut openal lua5.1) $(CFLAGS) -I./core -I./gui -I./tinyxml

##AM_CXXFLAGS = -g -O3 -DDEBUG_LEVEL=5

sumwars_SOURCES = core/trade.cpp \
	core/world.cpp \
	core/charconv.cpp \
	core/objectfactory.cpp \
	core/objectloader.cpp \
	core/templateloader.cpp \
	core/itemlist.cpp \
	core/gridunit.cpp \
	core/servernetwork.cpp \
	core/clientnetwork.cpp \
	core/worldobject.cpp \
	core/networkstruct.cpp \
	core/projectile.cpp \
	core/party.cpp \
	core/debug.cpp \
	core/item.cpp \
	core/dropitem.cpp \
	core/creaturestruct.cpp \
	core/creature.cpp \
	core/pathfind.cpp \
	core/fixedobject.cpp \
	core/player.cpp \
	core/monster.cpp \
	core/itemfactory.cpp \
	core/itemloader.cpp \
	core/damage.cpp \
	core/random.cpp \
	core/region.cpp \
	core/spawnpoint.cpp \
	core/document.cpp \
	core/network.cpp \
	core/action.cpp \
	core/command.cpp \
	core/geometry.cpp \
	core/mapgenerator.cpp \
	core/unionfindtree.cpp \
	core/variable.cpp \
	core/event.cpp \
	core/eventsystem.cpp \
	core/worldloader.cpp \
	core/dialogue.cpp \
	core/quest.cpp \
	gui/main_gui.cpp \
	gui/mainwindow.cpp \
	gui/scene.cpp \
	gui/application.cpp \
	gui/charinfo.cpp \
	gui/controlpanel.cpp \
	gui/inventory.cpp \
	gui/itemwindow.cpp \
	gui/skilltree.cpp \
	gui/mainmenu.cpp \
	gui/savegamelist.cpp \
	gui/chatline.cpp \
	gui/partyinfo.cpp \
	gui/sound.cpp \
	tinyxml/tinyxml.cpp \
	tinyxml/tinyxmlparser.cpp \
	tinyxml/tinyxmlerror.cpp \
	tinyxml/tinystr.cpp

