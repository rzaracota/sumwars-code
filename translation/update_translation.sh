#!/bin/bash

cp ./xmlgettext/xmlgettext ./potfiles/

echo "CREATE new potfiles"
xgettext -d menu -o potfiles/menu.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
#../../tests/xmlgettext/xmlgettext ../data/
cd potfiles ; ./xmlgettext ../../data/ ; cd ..
#xgettext --from-code utf-8 --language=Python -d events -o potfiles/sumwars-events.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml

ls potfiles/*.pot | sed "s/.pot//;s/potfiles\///" > domains



#echo "MERGE new pots with pos"
#echo "German sumwars xml events"
#msgmerge -v -U de/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
#msgmerge -v -U de/sumwars-xml.po potfiles/sumwars-xml.pot
#msgmerge -v -U de/sumwars-events.po potfiles/sumwars-events.pot

#echo "Italian sumwars xml events"
#msgmerge -v -U it/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
#msgmerge -v -U it/sumwars-xml.po potfiles/sumwars-xml.pot
#msgmerge -v -U it/sumwars-events.po potfiles/sumwars-events.pot

#echo "Polish sumwars xml events"
#msgmerge -v -U pl/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
#msgmerge -v -U pl/sumwars-xml.po potfiles/sumwars-xml.pot
#msgmerge -v -U pl/sumwars-events.po potfiles/sumwars-events.pot

echo "Russian Translation"
msgmerge -v -U ru/defend_dwarfenwall.po potfiles/defend_dwarfenwall.pot
msgmerge -v -U ru/fortify_dwarfenwall.po potfiles/fortify_dwarfenwall.pot
msgmerge -v -U ru/herb_collector.po potfiles/herb_collector.pot
msgmerge -v -U ru/joringsbridge.po potfiles/joringsbridge.pot
msgmerge -v -U ru/maylons_grave.po potfiles/maylons_grave.pot
msgmerge -v -U ru/menu.po potfiles/menu.pot
msgmerge -v -U ru/rescue_willard.po potfiles/rescue_willard.pot
msgmerge -v -U ru/rob_the_robber.po potfiles/rob_the_robber.pot
msgmerge -v -U ru/sumwars.po potfiles/sumwars.pot
msgmerge -v -U ru/testquest.po potfiles/testquest.pot
msgmerge -v -U ru/the_ambush_of_the_undead.po potfiles/the_ambush_of_the_undead.pot
msgmerge -v -U ru/the_ambush.po potfiles/the_ambush.pot
msgmerge -v -U ru/the_besieged_farm.po potfiles/the_besieged_farm.pot
msgmerge -v -U ru/the_goblinslayer.po potfiles/the_goblinslayer.pot
msgmerge -v -U ru/the_poolmonster.po potfiles/the_poolmonster.pot
msgmerge -v -U ru/the_trader.po potfiles/the_trader.pot
msgmerge -v -U ru/tutorial.po potfiles/tutorial.pot

