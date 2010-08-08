#!/bin/bash

#echo "UPDATE  test.pot  test_de.po"
#xgettext -d sumwars -s -o test-new.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
#msgmerge -v -s -U test.pot test-new.pot
#msgmerge -v -s -U test_de.po test-new.pot

echo "CREATE new potfiles"
xgettext -d sumwars -o potfiles/sumwars-sumwars.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
../../tests/xmlgettext/xmlgettext ../data/
xgettext --from-code utf-8 --language=Python -d events -o potfiles/sumwars-events.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml

echo "MERGE new pots with pos"
echo "German sumwars xml events"
msgmerge -v -U de/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
msgmerge -v -U de/sumwars-xml.po potfiles/sumwars-xml.pot
msgmerge -v -U de/sumwars-events.po potfiles/sumwars-events.pot

echo "Italian sumwars xml events"
msgmerge -v -U it/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
msgmerge -v -U it/sumwars-xml.po potfiles/sumwars-xml.pot
msgmerge -v -U it/sumwars-events.po potfiles/sumwars-events.pot

echo "Polish sumwars xml events"
msgmerge -v -U pl/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
msgmerge -v -U pl/sumwars-xml.po potfiles/sumwars-xml.pot
msgmerge -v -U pl/sumwars-events.po potfiles/sumwars-events.pot

#msgmerge -v -U en/sumwars-sumwars.po potfiles/sumwars-sumwars.pot
#msgmerge -v -U en/sumwars-xml.po potfiles/sumwars-xml.pot
#msgmerge -v -U en/sumwars-events.po potfiles/sumwars-events.pot

