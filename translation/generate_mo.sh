#!/bin/bash

# German
echo "German sumwars-sumwars"
msgfmt -c -v -o de/LC_MESSAGES/sumwars-sumwars.mo de/sumwars-sumwars.po
echo "German sumwars-xml"
msgfmt -c -v -o de/LC_MESSAGES/sumwars-xml.mo de/sumwars-xml.po
echo "German sumwars-events"
msgfmt -c -v -o de/LC_MESSAGES/sumwars-events.mo de/sumwars-events.po

# English
#msgfmt -c -v -o en/LC_MESSAGES/sumwars_sumwars.mo sumwars_sumwars_en.po
#msgfmt -c -v -o en/LC_MESSAGES/sumwars_events.mo sumwars_events_en.po

