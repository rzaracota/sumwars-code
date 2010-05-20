#!/bin/bash

# German
echo "de mo"
#msgfmt -c -v -o de/LC_MESSAGES/sumwars-sumwars.mo de/sumwars-sumwars.po
#msgfmt -c -v -o de/LC_MESSAGES/sumwars-xml.mo de/sumwars-xml.po
msgfmt -c -v -o de/LC_MESSAGES/sumwars-events.mo de/sumwars-events.po

# English
echo "en mo"
msgfmt -c -v -o en/LC_MESSAGES/sumwars-sumwars.mo en/sumwars-sumwars.po
msgfmt -c -v -o en/LC_MESSAGES/sumwars-xml.mo en/sumwars-xml.po
#msgfmt -c -v -o en/LC_MESSAGES/sumwars-events.mo en/sumwars-events.po

