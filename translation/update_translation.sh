#!/bin/bash

#echo "UPDATE  test.pot  test_de.po"
#xgettext -d sumwars -s -o test-new.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
#msgmerge -v -s -U test.pot test-new.pot
#msgmerge -v -s -U test_de.po test-new.pot

echo "+++++++++++++ sumwars +++++++++++++"
xgettext -d sumwars -s -o sumwars_sumwars-new.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
echo "UPDATE sumwars_sumwars.pot"
msgmerge -v -s -U sumwars_sumwars.pot sumwars_sumwars-new.pot
echo "UPDATE sumwars_sumwars_de.po"
msgmerge -v -s -U sumwars_sumwars_de.po sumwars_sumwars-new.pot
rm sumwars_sumwars-new.pot


echo "+++++++++++++ xml +++++++++++++"
../../tests/xmlgettext/xmlgettext ../data/
echo "UPDATE sumwars_xml.pot"
msgmerge -v -s -U sumwars_xml.pot sumwars_xml-new.pot
echo "UPDATE sumwars_xml_de.po"
msgmerge -v -s -U sumwars_xml_de.po sumwars_xml-new.pot
rm sumwars_xml-new.pot


echo "+++++++++++++ events +++++++++++++"
xgettext --language=Python -d events -s -o sumwars_events-new.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml
echo "UPDATE sumwars_events.pot"
msgmerge -v -s -U sumwars_events.pot sumwars_events-new.pot
echo "UPDATE sumwars_events_de.po"
msgmerge -v -s -U sumwars_events_de.po sumwars_events-new.pot
rm sumwars_events-new.pot

