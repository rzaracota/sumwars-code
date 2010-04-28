#!/bin/bash

#echo "UPDATE  test.pot  test_de.po"
#xgettext -d sumwars -s -o test-new.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
#msgmerge -v -s -U test.pot test-new.pot
#msgmerge -v -s -U test_de.po test-new.pot

echo "CREATE new potfiles"
xgettext -d sumwars -o potfiles/sumwars-sumwars-new.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
../../tests/xmlgettext/xmlgettext ../data/
xgettext --from-code utf-8 --language=Python -d events -o potfiles/sumwars-events-new.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml

echo "MERGE new pots with pots"
msgmerge -v -U potfiles/sumwars-sumwars.pot potfiles/sumwars-sumwars-new.pot
msgmerge -v -U potfiles/sumwars-xml.pot potfiles/sumwars-xml-new.pot
msgmerge -v -U potfiles/sumwars-events.pot potfiles/sumwars-events-new.pot

echo "MERGE new pots with pos"
msgmerge -v -U de/sumwars-sumwars.po potfiles/sumwars-sumwars-new.pot
msgmerge -v -U de/sumwars-xml.po potfiles/sumwars-xml-new.pot
msgmerge -v -U de/sumwars-events.po potfiles/sumwars-events-new.pot

echo "REMOVE temporary new pots"
rm potfiles/sumwars-sumwars-new.pot
rm potfiles/sumwars-xml-new.pot
rm potfiles/sumwars-events-new.pot

