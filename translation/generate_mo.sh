#!/bin/bash

echo "German sumwars xml events"
msgfmt -c -v -o de/LC_MESSAGES/sumwars-sumwars.mo de/sumwars-sumwars.po
msgfmt -c -v -o de/LC_MESSAGES/sumwars-xml.mo de/sumwars-xml.po
msgfmt -c -v -o de/LC_MESSAGES/sumwars-events.mo de/sumwars-events.po

echo "Italian sumwars xml events"
msgfmt -c -v -o it/LC_MESSAGES/sumwars-sumwars.mo it/sumwars-sumwars.po
msgfmt -c -v -o it/LC_MESSAGES/sumwars-xml.mo it/sumwars-xml.po
msgfmt -c -v -o it/LC_MESSAGES/sumwars-events.mo it/sumwars-events.po

echo "Russian sumwars xml events"
msgfmt -c -v -o ru/LC_MESSAGES/sumwars-sumwars.mo ru/sumwars-sumwars.po
#msgfmt -c -v -o ru/LC_MESSAGES/sumwars-xml.mo ru/sumwars-xml.po
#msgfmt -c -v -o ru/LC_MESSAGES/sumwars-events.mo ru/sumwars-events.po

#echo "Polish sumwars xml events"
#msgfmt -c -v -o pl/LC_MESSAGES/sumwars-sumwars.mo pl/sumwars-sumwars.po
#msgfmt -c -v -o pl/LC_MESSAGES/sumwars-xml.mo pl/sumwars-xml.po
#msgfmt -c -v -o pl/LC_MESSAGES/sumwars-events.mo pl/sumwars-events.po

# English
#echo "en mo"
#msgfmt -c -v -o en/LC_MESSAGES/sumwars-sumwars.mo en/sumwars-sumwars.po
#msgfmt -c -v -o en/LC_MESSAGES/sumwars-xml.mo en/sumwars-xml.po
#msgfmt -c -v -o en/LC_MESSAGES/sumwars-events.mo en/sumwars-events.po

