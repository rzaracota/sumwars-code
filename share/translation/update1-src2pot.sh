#!/bin/bash

echo "Building XMLGettext..."
make -C xmlgettext
cp ./xmlgettext/xmlgettext ./potfiles/

echo "Creating new potfiles..."
xgettext -d menu -o potfiles/menu.pot ../../src/core/*.cpp ../../src/core/*.h ../../src/gui/*.cpp ../../src/gui/*.h
xgettext -d tools -o potfiles/tools.pot ../src/gui/contenteditor/*.cpp ../src/gui/contenteditor/*.h ../src/gui/debugpanel/*.cpp ../src/gui/debugpanel/*.h ../src/gui/debugpanel/filebrowser/*.cpp ../src/gui/debugpanel/filebrowser/*.h
cd potfiles
./xmlgettext ../../data/
cd ..

echo "Complete."
