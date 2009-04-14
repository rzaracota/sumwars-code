#!/bin/bash

# German
echo "+++++++++++++ sumwars +++++++++++++"
msgfmt -c -v -o de/LC_MESSAGES/sumwars_sumwars.mo sumwars_sumwars_de.po
echo "+++++++++++++ xml +++++++++++++"
msgfmt -c -v -o de/LC_MESSAGES/sumwars_xml.mo sumwars_xml_de.po
echo "+++++++++++++ events +++++++++++++"
msgfmt -c -v -o de/LC_MESSAGES/sumwars_events.mo sumwars_events_de.po

# English
#msgfmt -c -v -o en/LC_MESSAGES/sumwars_sumwars.mo sumwars_sumwars_en.po
#msgfmt -c -v -o en/LC_MESSAGES/sumwars_events.mo sumwars_events_en.po

