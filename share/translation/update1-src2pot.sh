#!/bin/bash

echo "Building XMLGettext..."
make -C xmlgettext
cp ./xmlgettext/xmlgettext ./potfiles/

echo "Creating new potfiles..."
xgettext -d menu -o potfiles/menu.pot ../../src/core/*.cpp ../../src/core/*.h ../../src/gui/*.cpp ../../src/gui/*.h
cd potfiles
./xmlgettext ../../data/
cd ..

echo "Complete."
