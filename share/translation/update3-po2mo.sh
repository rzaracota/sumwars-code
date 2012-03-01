#!/bin/bash

skipdirectories="potfiles xmlgettext"
langs=$(ls -d */)

for l in $skipdirectories
do
	langs=${langs//$l\//}
done

for l in $langs
do
	# get all po files from the directory and strip directory prefix
	pos=$(ls $l/*.po)
	pos=${pos//$l\//}
	[ -d "$l/LC_MESSAGES" ] || mkdir "$l/LC_MESSAGES"
	for p in $pos
	do
		echo -n "$l$p "
		msgfmt -c -v -o "$l/LC_MESSAGES/${p/%po/mo}" "$l/$p"
	done
done
echo "Complete."
