#!/bin/bash

skipdirectories="potfiles xmlgettext"
langs=$(ls -d */)
pots=$(ls potfiles/*.pot)
pots=${pots//potfiles\//}

for l in $skipdirectories
do
	langs=${langs//$l\//}
done

for l in $langs
do
	for p in $pots
	do
		po=$l${p%t}
		# create the file if it doesn't exist
		echo -ne "\E[1;31m$po \E[0m"
		if [ -f "$po" ] ; then
			msgmerge --color=auto --force-po --lang="$l" -F -U "$po" "potfiles/$p"
		else
			msginit --color=auto -l "${l%/}" -i "potfiles/$p" -o "$po"
		fi
		#unix2dos $po
	done
	echo
done
echo "Complete."
