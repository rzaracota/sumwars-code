#!/bin/bash

# copy newest xmlgettext version
cp ./xmlgettext/xmlgettext ./potfiles/


echo "CREATE new potfiles"
xgettext -d menu -o potfiles/menu.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
cd potfiles ; ./xmlgettext ../../data/ ; cd ..
#../../tests/xmlgettext/xmlgettext ../data/
#xgettext --from-code utf-8 --language=Python -d events -o potfiles/sumwars-events.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml


ls potfiles/*.pot | sed "s/.pot//;s/potfiles\///" > domains


echo "German translation"
msgmerge -v -U de/defend_dwarfenwall.po potfiles/defend_dwarfenwall.pot
msgmerge -v -U de/fortify_dwarfenwall.po potfiles/fortify_dwarfenwall.pot
msgmerge -v -U de/herb_collector.po potfiles/herb_collector.pot
msgmerge -v -U de/joringsbridge.po potfiles/joringsbridge.pot
msgmerge -v -U de/maylons_grave.po potfiles/maylons_grave.pot
msgmerge -v -U de/menu.po potfiles/menu.pot
msgmerge -v -U de/rescue_willard.po potfiles/rescue_willard.pot
msgmerge -v -U de/rob_the_robber.po potfiles/rob_the_robber.pot
msgmerge -v -U de/sumwars.po potfiles/sumwars.pot
msgmerge -v -U de/testquest.po potfiles/testquest.pot
msgmerge -v -U de/the_ambush_of_the_undead.po potfiles/the_ambush_of_the_undead.pot
msgmerge -v -U de/the_ambush.po potfiles/the_ambush.pot
msgmerge -v -U de/the_besieged_farm.po potfiles/the_besieged_farm.pot
msgmerge -v -U de/the_goblinslayer.po potfiles/the_goblinslayer.pot
msgmerge -v -U de/the_poolmonster.po potfiles/the_poolmonster.pot
msgmerge -v -U de/the_trader.po potfiles/the_trader.pot
msgmerge -v -U de/tutorial.po potfiles/tutorial.pot

echo "Italian translation"
msgmerge -v -U it/defend_dwarfenwall.po potfiles/defend_dwarfenwall.pot
msgmerge -v -U it/fortify_dwarfenwall.po potfiles/fortify_dwarfenwall.pot
msgmerge -v -U it/herb_collector.po potfiles/herb_collector.pot
msgmerge -v -U it/joringsbridge.po potfiles/joringsbridge.pot
msgmerge -v -U it/maylons_grave.po potfiles/maylons_grave.pot
msgmerge -v -U it/menu.po potfiles/menu.pot
msgmerge -v -U it/rescue_willard.po potfiles/rescue_willard.pot
msgmerge -v -U it/rob_the_robber.po potfiles/rob_the_robber.pot
msgmerge -v -U it/sumwars.po potfiles/sumwars.pot
msgmerge -v -U it/testquest.po potfiles/testquest.pot
msgmerge -v -U it/the_ambush_of_the_undead.po potfiles/the_ambush_of_the_undead.pot
msgmerge -v -U it/the_ambush.po potfiles/the_ambush.pot
msgmerge -v -U it/the_besieged_farm.po potfiles/the_besieged_farm.pot
msgmerge -v -U it/the_goblinslayer.po potfiles/the_goblinslayer.pot
msgmerge -v -U it/the_poolmonster.po potfiles/the_poolmonster.pot
msgmerge -v -U it/the_trader.po potfiles/the_trader.pot
msgmerge -v -U it/tutorial.po potfiles/tutorial.pot

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

