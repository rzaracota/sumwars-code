#!/bin/bash

# New *.po files (resulting from new domains) need to be added by hand.


echo "Update translations ..."

# copy newest xmlgettext version
cp ./xmlgettext/xmlgettext ./potfiles/


echo -n "Create new potfiles"
xgettext -d menu -o potfiles/menu.pot ../src/core/*.cpp ../src/core/*.h ../src/gui/*.cpp ../src/gui/*.h
cd potfiles ; ./xmlgettext ../../data/ ; cd ..
#../../tests/xmlgettext/xmlgettext ../data/
#xgettext --from-code utf-8 --language=Python -d events -o potfiles/sumwars-events.pot ../data/npc/*.xml ../data/quests/*.xml ../data/world/*.xml


ls potfiles/*.pot | sed "s/.pot//;s/potfiles\///" > domains

while read LINE; do
	LANGUAGE="de"
	echo "+++ "$LINE" - GERMAN +++"
	echo "[ PO ]"
	msgmerge -v -U $LANGUAGE/$LINE.po potfiles/$LINE.pot
	echo "[ MO ]"
	msgfmt -c -v -o $LANGUAGE/LC_MESSAGES/$LINE.mo $LANGUAGE/$LINE.po

	LANGUAGE="it"
	echo "+++ "$LINE" - ITALIAN +++"
	echo "[ PO ]"
	msgmerge -v -U $LANGUAGE/$LINE.po potfiles/$LINE.pot
	echo "[ MO ]"
	msgfmt -c -v -o $LANGUAGE/LC_MESSAGES/$LINE.mo $LANGUAGE/$LINE.po

	LANGUAGE="ru"
	echo "+++ "$LINE" - RUSSIAN +++"
	echo "[ PO ]"
	msgmerge -v -U $LANGUAGE/$LINE.po potfiles/$LINE.pot
	echo "[ MO ]"
	msgfmt -c -v -o $LANGUAGE/LC_MESSAGES/$LINE.mo $LANGUAGE/$LINE.po
done < domains


echo "... Finished"

