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
		#[ ! -f $po ] && touch "$po"
		echo -n "$po "
		msgmerge --color=auto --force-po --lang=$l -v -F -U "$po" "potfiles/$p"
		#unix2dos $po
	done
done
echo "Complete."
