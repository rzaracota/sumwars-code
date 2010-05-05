AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = sumwars
sumwars_CPPFLAGS = -DDEBUG_LEVEL=5 -Wall -g -I./src/core -I./src/gui -I./src/tinyxml -I./src/raknet/Include
sumwars_CXXFLAGS = $(DEPS_CFLAGS)
sumwars_LDFLAGS = -lm -lrt
sumwars_LDADD = $(DEPS_LIBS)


sumwars_SOURCES = src/core/world.cpp \
	src/core/gameobject.cpp \
	src/core/charconv.cpp \
	src/core/objectfactory.cpp \
	src/core/objectloader.cpp \
	src/core/templateloader.cpp \
	src/core/itemlist.cpp \
	src/core/gridunit.cpp \
	src/core/servernetwork.cpp \
	src/core/clientnetwork.cpp \
	src/core/worldobject.cpp \
	src/core/networkstruct.cpp \
	src/core/projectile.cpp \
	src/core/party.cpp \
	src/core/debug.cpp \
	src/core/logger.cpp \
	src/core/item.cpp \
	src/core/dropitem.cpp \
	src/core/creaturestruct.cpp \
	src/core/creature.cpp \
	src/core/pathfind.cpp \
	src/core/fixedobject.cpp \
	src/core/playerbase.cpp \
	src/core/player.cpp \
	src/core/monster.cpp \
	src/core/itemfactory.cpp \
	src/core/itemloader.cpp \
	src/core/damage.cpp \
	src/core/random.cpp \
	src/core/region.cpp \
	src/core/spawnpoint.cpp \
	src/core/document.cpp \
	src/core/network.cpp \
	src/core/action.cpp \
	src/core/command.cpp \
	src/core/geometry.cpp \
	src/core/mapgenerator.cpp \
	src/core/unionfindtree.cpp \
	src/core/event.cpp \
	src/core/eventsystem.cpp \
	src/core/worldloader.cpp \
	src/core/elementattrib.cpp \
	src/core/dialogue.cpp \
	src/core/quest.cpp \
	src/core/minimap.cpp \
	src/core/waypoint.cpp \
	src/core/gettext.cpp \
	src/core/scriptobject.cpp \
	src/core/fraction.cpp \
	src/core/treasure.cpp \
	src/gui/main_gui.cpp \
	src/gui/mainwindow.cpp \
	src/gui/scene.cpp \
	src/gui/graphicobject.cpp \
	src/gui/graphicobjectrenderinfo.cpp \
	src/gui/graphicmanager.cpp \
	src/gui/application.cpp \
	src/gui/charinfo.cpp \
	src/gui/controlpanel.cpp \
	src/gui/inventory.cpp \
	src/gui/itemwindow.cpp \
	src/gui/skilltree.cpp \
	src/gui/mainmenu.cpp \
	src/gui/savegamelist.cpp \
	src/gui/chatline.cpp \
	src/gui/partyinfo.cpp \
	src/gui/charcreate.cpp \
	src/gui/questinfo.cpp \
	src/gui/minimapwindow.cpp \
	src/gui/tradewindow.cpp \
	src/gui/optionswindow.cpp\
	src/gui/networkwindows.cpp \
	src/gui/worldmap.cpp \
	src/gui/tooltip.cpp \
	src/gui/messageboxes.cpp \
	src/gui/dialoguewindow.cpp \
	src/gui/sound.cpp \
	src/gui/ogg.cpp \
	src/gui/music.cpp \
	src/tinyxml/tinyxml.cpp \
	src/tinyxml/tinyxmlparser.cpp \
	src/tinyxml/tinyxmlerror.cpp \
	src/tinyxml/tinystr.cpp

