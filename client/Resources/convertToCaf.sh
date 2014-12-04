#!/bin/bash

function getName {
	echo ${1%.*}
}

for i in *.ogg; do
	name=$(getName $i)
	rm -rf $name.wav $name.caf
	ffmpeg -i $i $name.wav > /dev/null 2>&1
	afconvert -f caff -d ima4 -c 1 $name.wav
	mv -f $name.caf sounds/ios/
	rm -rf $name.wav
done
